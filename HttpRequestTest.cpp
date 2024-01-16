#include "HttpRequest.h"
#include <iostream>

/**
 * @brief The main function of the program.
 * 
 * This function sends an HTTP request to "https://google.com" using the CHttpRequest::Request method.
 * It prints the response and the status code to the console.
 */
int main() {
	uint32_t statusCode = 0;
	auto response = CHttpRequest::Request(L"https://google.com", CHttpRequest::COptions(), statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;
}
