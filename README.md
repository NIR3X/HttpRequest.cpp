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
	uint32_t statusCode = 0;
	auto response = CHttpRequest::Request(L"https://google.com", CHttpRequest::COptions(), statusCode);
	std::cout << "Response: " << std::string(response.begin(), response.end()) << std::endl;
	std::cout << "Status code: " << statusCode << std::endl;
}
```

This example makes a GET request to "https://google.com" and prints the response and the status code. The CHttpRequest::COptions() function is used to specify options for the request, such as the HTTP method, headers, and data.
