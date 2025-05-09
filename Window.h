#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "Frame.h"

using json = nlohmann::json;

class SubWindow;

class Window {
public:
    Window(HINSTANCE instance, int nCmdShow);
    virtual ~Window();

    bool Init();
    void Show() const;
    void Update() const;
    HWND GetWindowHandle() const { return windowHandle; }
    void RunMessageLoop() const;

    void LoadFrame(std::shared_ptr<Frame> frame, const json& options = {});

    void SetTitle(const std::wstring& t);
    void SetSize(int w, int h);
    void SetStyle(DWORD style);
    void SetMenuResource(int res);

    void RegisterSubWindow(std::shared_ptr<SubWindow> sw);
protected:
    virtual LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp);

private:
    HWND                 windowHandle = nullptr;
    HINSTANCE            instance;
    int                  nCmdShow;
    std::wstring         title = L"";
    int                  width = CW_USEDEFAULT;
    int                  height = CW_USEDEFAULT;
    DWORD                dwStyle = WS_OVERLAPPEDWINDOW;
    int                  menuResource = 0;
	bool                 initialized = false;    

    std::vector<std::shared_ptr<SubWindow>> subWindows;
    std::shared_ptr<Frame>                  currentlyLoadedFrame;

    static const std::wstring GENERIC_CLASS_NAME;
    static bool               classRegistered;
    static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);

    void RegisterWindowClass() const;
};
