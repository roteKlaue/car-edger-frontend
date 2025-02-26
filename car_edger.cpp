// car_edger.cpp : Defines the entry point for the application.
// Author: Jan Köck
// Created: 2025/02/26
//

#include "framework.h"
#include "car_edger.h"
#include "Window.h"

// application entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // handle to the current instance
	    _In_opt_ HINSTANCE hPrevInstance,       // handle to the previous instance
	    _In_ LPWSTR    lpCmdLine,               // command line string
	    _In_ int       nCmdShow)                // show state of the window
{
	UNREFERENCED_PARAMETER(hPrevInstance);      // unused parameter
	UNREFERENCED_PARAMETER(lpCmdLine); 		    // unused parameter

    // Initialize global strings
    WCHAR className[MAX_LOADSTRING] = L"WindowClass";
    WCHAR title[MAX_LOADSTRING] = L"Car Edger";

    // Create a Window object
    Window window(hInstance, className, title, nCmdShow);

    // Initialize the window
    if (!window.Init()) {
        MessageBoxW(nullptr, L"Window initialization failed!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Run the message loop
    window.RunMessageLoop();

    return 0;
}
