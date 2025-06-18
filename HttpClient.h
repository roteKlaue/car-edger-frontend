#pragma once

#include <windows.h>
#include <wininet.h>
#include <string>

#pragma comment(lib, "wininet.lib")

/**
 * @brief Simple HTTP client using WinINet.
 *
 * Provides synchronous GET and POST requests over HTTP using the WinINet API.
 * Intended for lightweight background communication in Windows desktop applications.
 */
class HttpClient
{
public:
    /// Constructs and initializes the HTTP client session.
    HttpClient();
    /// Cleans up any open session handles.
    ~HttpClient();

    /**
     * @brief Performs an HTTP GET request.
     *
     * @param host The hostname (e.g., L"example.com").
     * @param path The path to the resource (e.g., L"/api/data").
     * @param port Optional port number (default: 80).
     * @return The server response body as a std::string.
     *
     * @throws std::runtime_error on failure.
     */
    std::string get(const std::wstring& host, const std::wstring& path, INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT) const;

    /**
     * @brief Performs an HTTP POST request.
     *
     * @param host The hostname (e.g., L"example.com").
     * @param path The path to the resource (e.g., L"/api/post").
     * @param body The request body (typically JSON or form-encoded).
     * @param contentType The content type of the request body (default: "application/json").
     * @param port Optional port number (default: 80).
     * @return The server response body as a std::string.
     *
     * @throws std::runtime_error on failure.
     */
    std::string post(const std::wstring& host, const std::wstring& path, const std::string& body,
        const std::string& contentType = "application/json",
        INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT) const;

private:
    ///< Handle to the WinINet session.
    HINTERNET hSession;

    /**
    * @brief Sends a raw HTTP request (internal utility).
    *
    * @param host The hostname.
    * @param port The port number.
    * @param method HTTP method (e.g., L"GET", L"POST").
    * @param path The URL path.
    * @param data Pointer to the request body data (can be null).
    * @param dataSize Size of the request body in bytes.
    * @param contentType Content-Type header (null for GET).
    * @return The response body as a std::string.
    *
    * @throws std::runtime_error on failure.
    */
    std::string sendRequest(const std::wstring& host, INTERNET_PORT port,
        LPCWSTR method, const std::wstring& path,
        const void* data, DWORD dataSize,
        LPCSTR contentType) const;
};

