// Window.h : Header file for the Window class definition.
// Author: Jan Koeck
// Created: 2025/02/26
//

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
 *
 * The Window class encapsulates the Win32 API window functionality and augments it with a dynamic frame-loading system,
 * menu command handling, and asynchronous HTTP capabilities. It supports UI thread and background thread task posting,
 * dynamic resizing, subwindow registration, and modal dialog windows.
 */
class Window {
public:
    /**
     * @brief Constructs the Window instance with application HINSTANCE and show command.
     * @param instance The application instance handle.
     * @param nCmdShow The show command passed from WinMain.
     */
    Window(HINSTANCE instance, int nCmdShow);
	/**
	 * @brief Destructor for the Window class, cleans up resources.
	 */
    virtual ~Window();

    /**
     * @brief Initializes the window, registers the class, creates the handle.
     * @return true if initialization succeeds, false otherwise.
     */
    bool Init();
    /**
     * @brief Shows the window on the screen.
     */
    void Show() const;
	/**
	 * @brief Updates the window, invalidates the client area and redraws it.
	 */
    void Update() const;
	/**
	 * @brief Cleans up resources and unregisters the window class.
	 */
    virtual void Cleanup();
	/**
	 * @brief Gets the title of the window.
	 * @return The current title of the window.
	 */
    HWND GetWindowHandle() const { return windowHandle; }
    /**
     * @brief Starts the standard Win32 message loop.
     */
    void RunMessageLoop() const;

    /**
     * @brief Loads a new frame into the window, replacing the current one.
     * @param frame The frame instance to load.
     * @param options Optional JSON configuration for the frame.
     */
    void LoadFrame(std::shared_ptr<Frame> frame, const json& options = {});

    /**
     * @brief Sets the window title.
     * @param t Wide string title to display.
     */
    virtual void SetTitle(const std::wstring& t) = 0;

    /**
     * @brief Sets the window size.
     * @param w Width in pixels.
     * @param h Height in pixels.
     */
    void SetSize(int w, int h);
    /**
     * @brief Sets the Win32 window style (e.g., WS_OVERLAPPEDWINDOW).
     * @param style The new style bitmask.
     */
    void SetStyle(DWORD style);
    /**
     * @brief Sets the menu resource ID to be loaded and attached.
     * @param res The resource identifier.
     */
    void SetMenuResource(int res);
    /**
     * @brief Registers a subwindow (e.g., dialog or overlay panel).
     * @param sw Shared pointer to the subwindow.
     */
    void RegisterSubWindow(std::shared_ptr<SubWindow> sw);
    /**
     * @brief Registers a menu button click callback.
     * @param id Resource ID of the menu item.
     * @param func The callback function with access to this Window.
     */
    virtual void RegisterMenuButton(UINT id, std::function<void(Window*)> func);

    /**
     * @brief Executes the function on a background thread.
     */
    virtual void RunOnBackgroundThread(std::function<void()> func) = 0;
    /**
     * @brief Posts the function to the UI thread to run asynchronously.
     */
    virtual void PostToUIThread(std::function<void()> func) = 0;
    /**
     * @brief Asserts the current thread is the background thread.
     */
	virtual void AssertBGThread() const = 0;
    /**
     * @brief Asserts the current thread is the UI thread.
     */
    virtual void AssertUIThread() const = 0;
    /**
     * @brief Returns the current window height.
     */
	virtual int  GetHeight() const;
    /**
     * @brief Returns the current window width.
     */
    virtual int  GetWidth() const;

    using Callback = std::function<void(const json&)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    /**
     * @brief Executes an HTTP call in the background and returns results to UI thread.
     * @param host Server host.
     * @param path URL path.
     * @param body POST body.
     * @param contentType HTTP Content-Type header.
     * @param port Target port.
     * @param onSuccess JSON callback on success.
     * @param onError Optional error callback.
     * @param usePost If true, uses POST; otherwise, GET.
     */
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
    /**
     * @brief Shows a modal dialog window as a SubWindow with a frame.
     * @param frame The frame to load in the dialog.
     * @return Pointer to the modal SubWindow.
     */
    std::shared_ptr<Window> ShowDialog(std::shared_ptr<Frame> frame);

    /**
     * @brief Called after window creation. Override to implement custom setup.
     */
    virtual void OnInit() {};
    /**
     * @brief Called before window destruction. Override to handle teardown.
     */
	virtual void OnClose() {};

protected:
    /**
     * @brief Handles raw Win32 messages. Can be overridden.
     */
    virtual LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp);
    /** Indicates whether Init() has been successfully called. */
    bool initialized = false;
    /** The title of the window. */
    std::wstring title = L"";
    /** Win32 window handle. */
    HWND windowHandle = nullptr;

private:
    /** The application instance (HINSTANCE). */
    HINSTANCE            instance;
    /** The nCmdShow parameter from WinMain. */
    int                  nCmdShow;
    /** Current window width. */
    int                  width = CW_USEDEFAULT;
    /** Current window height. */
    int                  height = CW_USEDEFAULT;
    /** Window style flags. */
    DWORD                dwStyle = WS_OVERLAPPEDWINDOW;
    /** Menu resource ID to be used with LoadMenu. */
    int                  menuResource = 0;

    /** List of registered subwindows. */
    std::vector<std::shared_ptr<SubWindow>>         subWindows;
    /** Currently active frame inside the window. */
    std::shared_ptr<Frame>                          currentlyLoadedFrame;
    /** Maps menu button IDs to their callbacks. */
    std::unordered_map<UINT, std::function<void(Window*)>> menuButtons;

    /** The window class name used for RegisterClassEx. */
    static const std::wstring GENERIC_CLASS_NAME;
    /** Whether the window class has already been registered. */
    static bool               classRegistered;
    /**
     * @brief Static Win32 window procedure, delegates to instance HandleMessage.
     */
    static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);

    /**
     * @brief Responsible for background HTTP communication.
     */
    std::unique_ptr<HttpClient> httpClient = std::make_unique<HttpClient>();

    /**
     * @brief Registers the Win32 window class.
     */
    void RegisterWindowClass() const;
};
