# HttpRequest - WinAPI HTTP Request Library

This is a C++ package that implements the `CHttpRequest` class for making HTTP requests using WinHTTP. This library provides a simple and efficient way to make HTTP requests in C++.

## Installation

To use this package, you can clone the repository and compile it using a C++ compiler:

```bash
git clone https://github.com/NIR3X/HttpRequest.cpp
cd HttpRequest.cpp
make
```

## Usage

Here is an example of how to use this package:

```cpp
#include "HttpRequest.h"
#include <iostream>

int main() {
	uint32_t statusCode;
	auto response = CHttpRequest::Request(L"https://example.com/index.html", CHttpRequest::COptions(), statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;

	statusCode = 0;
	CHttpRequest::COptions options;
	options.method = L"POST";
	options.headers[L"Content-Type"] = L"application/x-www-form-urlencoded";
	std::string data = "field1=value1&field2=value2";
	options.data = { data.begin(), data.end() };
	response = CHttpRequest::Request(L"https://example.com/test", options, statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;
}
```

This code example demonstrates making a GET request to "https://example.com/index.html" and printing the response along with the status code. It also shows how to make a POST request to "https://example.com/test" with custom options, including specifying the HTTP method, headers, and data. The response and status code for the POST request are also printed.
