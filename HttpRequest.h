#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <winhttp.h>

class CHttpRequest {
public:
	/**
	 * @brief Represents an HTTP request URL.
	 */
	class CUrl {
	public:
		std::wstring host, path; /**< The host and path components of the URL. */
		INTERNET_PORT port; /**< The port number of the URL. */
		DWORD winHttpFlags; /**< Additional flags for WinHTTP. */
	};

	/**
	 * @brief Represents options for an HTTP request.
	 */
	class COptions {
	public:
		std::wstring method; /**< The HTTP method to be used for the request. */
		std::wstring proxyList; /**< The list of proxy servers to be used for the request. */
		std::wstring userAgent; /**< The user agent string to be used for the request. */
		std::unordered_map<std::wstring, std::wstring> headers; /**< The headers to be included in the request. */
		std::vector<uint8_t> data; /**< The data to be sent in the request body. */
		bool insecure; /**< Indicates whether to ignore SSL certificate errors. */

		/**
		 * @brief Default constructor for COptions.
		 */
		COptions();
	};

	/**
	 * @brief Parses the given URL and returns a CUrl object.
	 * @param url The URL to be parsed.
	 * @return The parsed CUrl object.
	 */
	static CUrl ParseUrl(std::wstring url);

	/**
	 * @brief Sends an HTTP request to the specified URL with the given options.
	 * @param url The URL to send the request to.
	 * @param options The options for the request.
	 * @param status [out] The HTTP status code of the response.
	 * @return The response data as a vector of bytes.
	 */
	static std::vector<uint8_t> Request(const std::wstring& url, COptions options, uint32_t& statusCode);
};
