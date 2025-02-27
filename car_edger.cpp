// car_edger.cpp : Defines the entry point for the application.
// Author: Jan K�ck
// Created: 2025/02/26
//

#include "framework.h"
#include "car_edger.h"
#include "Windows.h"
#include <iostream>
#include "ClassRegisterer.h"

static void AttachConsoleToStdout() {
    AllocConsole(); // Allocate a new console window
    // Redirect standard output to the console
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

static void PrintLastError() {
    DWORD error = GetLastError();
    LPWSTR messageBuffer = nullptr;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        0,
        (LPWSTR)&messageBuffer,
        0,
        nullptr);

    std::wcout << L"Error " << error << L": " << (messageBuffer ? messageBuffer : L"Unknown error") << std::endl;

    if (messageBuffer) LocalFree(messageBuffer);
}

// application entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // handle to the current instance
	    _In_opt_ HINSTANCE hPrevInstance,       // handle to the previous instance
	    _In_ LPWSTR    lpCmdLine,               // command line string
	    _In_ int       nCmdShow)                // show state of the window
{
	UNREFERENCED_PARAMETER(hPrevInstance);      // unused parameter
	UNREFERENCED_PARAMETER(lpCmdLine); 		    // unused parameter
    AttachConsoleToStdout();

    // Create a Window object
    MainWindow window(hInstance, nCmdShow);

    // Initialize the window
    if (!window.Init()) {
		PrintLastError();
        MessageBoxW(nullptr, L"Window initialization failed!", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Run the message loop
    window.RunMessageLoop();

    ClassRegisterer::GetInstance()->unregisterClasses();

    return 0;
}
