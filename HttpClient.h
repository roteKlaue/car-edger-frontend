#pragma once

#include <windows.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

class HttpClient
{
public:
    HttpClient();
    ~HttpClient();

    std::string get(const std::wstring& host, const std::wstring& path, INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT) const;
    std::string post(const std::wstring& host, const std::wstring& path, const std::string& body,
        const std::string& contentType = "application/json",
        INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT) const;

private:
    HINTERNET hSession;

    std::string sendRequest(const std::wstring& host, INTERNET_PORT port,
        LPCWSTR method, const std::wstring& path,
        const void* data, DWORD dataSize,
        LPCSTR contentType) const;
};

