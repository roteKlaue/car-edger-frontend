#include "pch.h"
#include "HttpClient.h"
#include <wininet.h>
#include <stdexcept>
#include <vector>

#pragma comment(lib, "wininet.lib")

HttpClient::HttpClient() {
    hSession = InternetOpen(L"HttpClient/1.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) throw std::runtime_error("InternetOpen failed");
}

HttpClient::~HttpClient() {
    if (hSession) InternetCloseHandle(hSession);
}

std::string HttpClient::get(const std::wstring& host, const std::wstring& path, INTERNET_PORT port) const {
    return sendRequest(host, port, L"GET", path, nullptr, 0, nullptr);
}

std::string HttpClient::post(const std::wstring& host, const std::wstring& path, const std::string& body,
    const std::string& contentType, INTERNET_PORT port) const {
    return sendRequest(host, port, L"POST", path, body.data(), static_cast<DWORD>(body.size()), contentType.c_str());
}

std::string HttpClient::sendRequest(const std::wstring& host, INTERNET_PORT port,
    LPCWSTR method, const std::wstring& path,
    const void* data, DWORD dataSize,
    LPCSTR contentType) const {
    HINTERNET hConnect = InternetConnect(hSession, host.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) throw std::runtime_error("InternetConnect failed");

    HINTERNET hRequest = HttpOpenRequest(hConnect, method, path.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        throw std::runtime_error("HttpOpenRequest failed");
    }

    std::string headers;
    if (contentType) {
        headers = "Content-Type: ";
        headers += contentType;
    }

    BOOL sent = HttpSendRequestA(hRequest,
        contentType ? headers.c_str() : NULL,
        contentType ? static_cast<DWORD>(headers.size()) : 0,
        (LPVOID)data, dataSize);

    if (!sent) {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        throw std::runtime_error("HttpSendRequest failed");
    }

    std::string response;
    char buffer[4096];
    DWORD bytesRead;

    while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead) {
        buffer[bytesRead] = '\0';
        response.append(buffer, bytesRead);
    }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    return response;
}
