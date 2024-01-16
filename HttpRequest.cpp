#include "HttpRequest.h"
#include <algorithm>

#ifndef __GNUC__
	#pragma comment(lib, "winhttp")
#endif

CHttpRequest::COptions::COptions() :
	method(L"GET"), proxyList({}),
	userAgent(L"Mozilla/5.0 (Windows NT x.y; Win64; x64; rv:10.0) Gecko/20100101 Firefox/10.0"),
	headers({}), data({}), insecure(false) {
}

CHttpRequest::CUrl CHttpRequest::ParseUrl(std::wstring url) {
	CHttpRequest::CUrl httpRequestUrl = {};
	size_t index = url.find(L"://");
	if (index != std::wstring::npos) {
		std::wstring protocol = url.substr(0, index);
		if (protocol == L"https") {
			httpRequestUrl.winHttpFlags |= WINHTTP_FLAG_SECURE;
			httpRequestUrl.port = 443;
		} else if (protocol == L"http") {
			if ((httpRequestUrl.winHttpFlags & WINHTTP_FLAG_SECURE) == WINHTTP_FLAG_SECURE) {
				httpRequestUrl.winHttpFlags ^= WINHTTP_FLAG_SECURE;
			}
			httpRequestUrl.port = 80;
		}
		url = url.substr(index + 3);
		index = url.find(L"/");
		if (index == std::wstring::npos) {
			httpRequestUrl.host = url;
			httpRequestUrl.path = L"/";
		} else {
			httpRequestUrl.host = url.substr(0, index);
			httpRequestUrl.path = url.substr(index);
		}
		index = httpRequestUrl.host.find(L":");
		if (index != std::wstring::npos) {
			httpRequestUrl.port = std::wcstol(httpRequestUrl.host.substr(index + 1).c_str(), nullptr, 10);
			httpRequestUrl.host = httpRequestUrl.host.substr(0, index);
		}
	}
	return httpRequestUrl;
}

std::vector<uint8_t> CHttpRequest::Request(const std::wstring& url, CHttpRequest::COptions options, uint32_t& statusCode) {
	std::vector<uint8_t> result = {};
	CHttpRequest::CUrl httpRequestUrl = ParseUrl(url);
	DWORD statusCodeLen = sizeof(statusCode);

	bool useProxies = options.proxyList.length() > 0;
	HINTERNET hSession = WinHttpOpen(
		options.userAgent.c_str(),
		useProxies ? WINHTTP_ACCESS_TYPE_DEFAULT_PROXY
			: WINHTTP_ACCESS_TYPE_NO_PROXY,
		WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0
	);
	if (hSession) {
		bool ok = true;
		if (useProxies) {
			WINHTTP_PROXY_INFO proxy = {};
			proxy.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
			proxy.lpszProxy = &options.proxyList[0];
			ok = WinHttpSetOption(
				hSession, WINHTTP_OPTION_PROXY, &proxy, sizeof(proxy)
			);
		}
		HINTERNET hConn = WinHttpConnect(
			hSession, httpRequestUrl.host.c_str(), httpRequestUrl.port, 0
		);
		if (hConn) {
			HINTERNET hReq = WinHttpOpenRequest(
				hConn, options.method.c_str(), httpRequestUrl.path.c_str(),
				NULL, WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES, httpRequestUrl.winHttpFlags
			);
			if (hReq) {
				if (options.insecure) {
					DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
						SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
						SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
						SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
					ok = WinHttpSetOption(
						hReq, WINHTTP_OPTION_SECURITY_FLAGS,
						&dwFlags, sizeof(dwFlags)
					);
				}
				if (ok) {
					LPVOID data = WINHTTP_NO_REQUEST_DATA;
					DWORD dataLen = options.data.size();
					if (dataLen > 0) {
						data = (LPVOID)options.data.data();
					}
					for (auto& header : options.headers) {
						WinHttpAddRequestHeaders(
							hReq,
							(header.first + L": " + header.second).c_str(),
							(ULONG)-1L,
							WINHTTP_ADDREQ_FLAG_ADD
						);
					}
					ok = WinHttpSendRequest(
						hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
						data, dataLen, dataLen, 0
					);
					if (ok) {
						ok = WinHttpReceiveResponse(hReq, NULL);
						if (ok) {
							ok = WinHttpQueryHeaders(
								hReq,
								WINHTTP_QUERY_STATUS_CODE |
								WINHTTP_QUERY_FLAG_NUMBER,
								NULL,
								&statusCode,
								&statusCodeLen,
								0
							);
							if (ok) {
								CHAR buff[8192];
								std::fill_n(buff, sizeof(buff), 0);
								DWORD read = 0;
								do {
									ok = WinHttpReadData(hReq, buff, sizeof(buff), &read);
									if (!ok) {
										break;
									}
									result.insert(result.end(), buff, buff + read);
									std::fill_n(buff, read, 0);
								} while (read > 0);
							}
						}	
					}
				}
				WinHttpCloseHandle(hReq);
			}
			WinHttpCloseHandle(hConn);
		}
		WinHttpCloseHandle(hSession);
	}
	return result;
}
