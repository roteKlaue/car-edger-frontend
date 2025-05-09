#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "Frame.h"
#include "HttpClient.h"

#define WM_RUN_ON_UI_THREAD (WM_USER + 1)

using json = nlohmann::json;

class SubWindow;

/**
 * @class Window
 * @brief A base class for managing a window and its contents, including frames, subwindows, and background operations.
 */
class Window {
public:
    Window(HINSTANCE instance, int nCmdShow);
    virtual ~Window();

    bool Init();
    void Show() const;
    void Update() const;
    virtual void Cleanup();
    HWND GetWindowHandle() const { return windowHandle; }
    void RunMessageLoop() const;

    void LoadFrame(std::shared_ptr<Frame> frame, const json& options = {});

    virtual void SetTitle(const std::wstring& t) = 0;
    void SetSize(int w, int h);
    void SetStyle(DWORD style);
    void SetMenuResource(int res);

    void RegisterSubWindow(std::shared_ptr<SubWindow> sw);

    virtual void RunOnBackgroundThread(std::function<void()> func) = 0;
    virtual void PostToUIThread(std::function<void()> func) = 0;

    using Callback = std::function<void(const json&)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    void LoadInBackground(
        const std::wstring& host,
        const std::wstring& path,
        const std::string& body,
        const std::string& contentType,
        int port,
        Callback onSuccess,
        ErrorCallback onError = nullptr,
        bool usePost = true
    );
protected:
    virtual LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp);
    bool                 initialized = false;
    std::wstring         title = L"";
    HWND                 windowHandle = nullptr;

private:
    HINSTANCE            instance;
    int                  nCmdShow;
    int                  width = CW_USEDEFAULT;
    int                  height = CW_USEDEFAULT;
    DWORD                dwStyle = WS_OVERLAPPEDWINDOW;
    int                  menuResource = 0;

    std::vector<std::shared_ptr<SubWindow>> subWindows;
    std::shared_ptr<Frame>                  currentlyLoadedFrame;

    static const std::wstring GENERIC_CLASS_NAME;
    static bool               classRegistered;
    static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);

    std::unique_ptr<HttpClient> httpClient;

    void RegisterWindowClass() const;
};
