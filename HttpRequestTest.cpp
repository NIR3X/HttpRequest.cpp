#include "HttpRequest.h"
#include <iostream>

/**
 * @brief The main function of the program.
 * 
 * This function demonstrates the usage of the CHttpRequest class by making HTTP requests to a server.
 * It first sends a GET request to "https://example.com/index.html" and prints the response and status code.
 * Then, it sends a POST request to "https://example.com/test" with some data and prints the response and status code.
 * 
 * @return int The exit code of the program.
 */
int main() {
	uint32_t statusCode = 0;
	auto response = CHttpRequest::Request(L"https://example.com/index.html", CHttpRequest::COptions {}, statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;

	statusCode = 0;
	CHttpRequest::COptions options = {};
	options.method = L"POST";
	options.headers[L"Content-Type"] = L"application/x-www-form-urlencoded";
	std::string data = "field1=value1&field2=value2";
	options.data = { data.begin(), data.end() };
	response = CHttpRequest::Request(L"https://example.com/test", options, statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;
}
