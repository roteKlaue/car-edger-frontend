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

class Window {
public:
    Window(HINSTANCE hInstance,
        const std::wstring& className,
        const std::wstring& title,
        int nCmdShow,
        DWORD style = WS_OVERLAPPEDWINDOW);
    virtual ~Window();

    virtual bool Init();
    void RunMessageLoop() const;
    HWND GetWindowHandle() const { return hWnd; }
    void Resize(int clientWidth, int clientHeight) const;
    // RegisterMenu(Menu* men) { menu = men; }
    void RegisterComponents();
    void AddComponent(std::shared_ptr<Component> component) { components[component->GetID()] = component; }
	void HandleCommandEvent(WPARAM wParam, LPARAM lParam) {
        HandleFileMenuPress(LOWORD(wParam));
		auto it = components.find(LOWORD(wParam));
		if (it != components.end()) {
			it->second->HandleCommand(wParam, lParam);
		}
	}

protected:
    virtual void HandleFileMenuPress(UINT id) {};
    virtual LRESULT HandleMessage(HWND eventHandle, UINT message, WPARAM wParam, LPARAM lParam) = 0;
    void SetMenuResource(WORD resource) { menuResource = resource; }

private:
    WNDCLASSEXW CreateWindowClass(HINSTANCE hInstance, std::wstring className);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance; // Handle to the application instance.
    int nCmdShow;        // Flag that controls how the window is shown.
    HWND hWnd;           // Handle to the window. 
    std::wstring szTitle;
    std::wstring szWindowClass;
    WORD menuResource = -1;
    DWORD dwStyle;
    Menu* menu;
    std::unordered_map<UINT, std::shared_ptr<Component>> components;
};
