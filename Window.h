// Window.h : Header file for the Window class definition.
// Author: Jan Koeck
// Created: 2025/02/26
//

#pragma once

#include <unordered_map>
#include <windows.h>
#include <string>
#include "Menu.h"
#include "Component.h"
#include <memory>

/**
 * @class Window
 * @brief Encapsulates a Win32 application window, including class registration,
 *        creation, message loop, component management, and resizing.
 */
class Window {
public:
    /**
     * @brief Constructs a Window object.
     * @param hInstance Handle to the application instance.
     * @param className Name of the window class.
     * @param title Title of the window.
     * @param nCmdShow Flag that controls how the window is to be shown.
     * @param style Window style flags (default WS_OVERLAPPEDWINDOW).
     */
    Window(HINSTANCE hInstance,
        const std::wstring& className,
        const std::wstring& title,
        int nCmdShow,
        DWORD style = WS_OVERLAPPEDWINDOW,
        bool stopApplicationOnClose = false);
    virtual ~Window();

    virtual bool Init();
    void RunMessageLoop() const;
    HWND GetWindowHandle() const { return hWnd; }
    void Resize(int clientWidth, int clientHeight);
    // RegisterMenu(Menu* men) { menu = men; }
    void RegisterComponents();
    void AddComponent(std::shared_ptr<Component> component) { components[component->GetID()] = component; }

    /**
     * @brief Handles a WM_COMMAND event by routing to the file menu handler
     *        and any registered components.
     * @param wParam WPARAM from the message.
     * @param lParam LPARAM from the message.
     */
    void HandleCommandEvent(WPARAM wParam, LPARAM lParam);

protected:
    virtual void HandleFileMenuPress(UINT id) {};
    virtual LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
    void SetMenuResource(WORD resource) { menuResource = resource; }

private:
    /**
     * @brief Constructs a WNDCLASSEXW structure for registration.
     * @param hInstance Handle to the application instance.
     * @param className Class name to assign.
     * @return Configured WNDCLASSEXW structure.
     */
    WNDCLASSEXW CreateWindowClass(HINSTANCE hInstance, std::wstring className);

    /**
     * @brief Static window procedure that dispatches messages to the Window instance.
     * @param hWnd Handle to the window.
     * @param message Message identifier.
     * @param wParam WPARAM parameter.
     * @param lParam LPARAM parameter.
     * @return Result code to return from DefWindowProc if not handled.
     */
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    int width = -1;
    int height = -1;
	bool stopApplicationOnClose;                                     /**< Flag to stop the application on close. */
    HINSTANCE hInstance;                                             /**< Handle to the application instance. */
    int nCmdShow;                                                    /**< Show window flag. */
    HWND hWnd;                                                       /**< Handle to the window. */
    std::wstring szTitle;                                            /**< Window title text. */
    std::wstring szWindowClass;                                      /**< Window class name. */
    WORD menuResource = -1;                                          /**< Menu resource ID (Optional). */
    DWORD dwStyle;                                                   /**< Window style flags. */
    Menu* menu = nullptr;                                            /**< Optional menu object. */
    std::unordered_map<UINT, std::shared_ptr<Component>> components; /**< Registered child components. */
};
