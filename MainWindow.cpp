#include "pch.h"
#include "MainWindow.h"
#include <chrono>
#include <iostream>

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow)
    : Window(hInstance, nCmdShow)
{
    running = true;
    backgroundThread = std::thread(&MainWindow::BackgroundLoop, this);
}

void MainWindow::SetTitle(const std::wstring& t) {
    title = t;
    if (!initialized || !windowHandle) return;
    SetWindowTextW(windowHandle, title.c_str());
}

void MainWindow::RunOnBackgroundThread(std::function<void()> func)
{
	if (!running) {
        std::cerr << "[ERROR] Background thread is not running." << std::endl;
        return;
	}
	
	std::lock_guard<std::mutex> lock(backlogMutex);
	backlog.push_back(func);
}

LRESULT MainWindow::HandleMessage(UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg) {
    case WM_DESTROY:
        StopBackgroundThread();
        PostQuitMessage(0);
        return 0;
	case WM_RUN_ON_UI_THREAD:
		{
			std::vector<std::function<void()>> localCallbacks;
			{
				std::lock_guard<std::mutex> lock(uiCallbackMutex);
				localCallbacks.swap(uiCallbacks);
			}

			for (auto& func : localCallbacks) {
				try {
					func();
				}
				catch (const std::exception& e) {
					std::cerr << "[ERROR] Exception in UI callback: " << e.what() << std::endl;
				}
			}
		}
		return 0;
	}

    return Window::HandleMessage(msg, wp, lp);
}

void MainWindow::BackgroundLoop()
{
	while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::vector<std::function<void()>> localBacklog;
		{
			std::lock_guard<std::mutex> lock(backlogMutex);
			localBacklog.swap(backlog);
		}

		for (auto& func : localBacklog) {
			try {
				std::cout << "[DEBUG] Running background task." << std::endl;
				func();
			}
			catch (const std::exception& e) {
				std::cerr << "[ERROR] Exception in background thread: " << e.what() << std::endl;
			}
			catch (...) {
				std::cerr << "[ERROR] Unknown exception in background thread." << std::endl;
			}
		}
	}
}

void MainWindow::StopBackgroundThread()
{
	running = false;
	if (backgroundThread.joinable()) {
		backgroundThread.join();
	}
}

void MainWindow::PostToUIThread(std::function<void()> func)
{
	{
		std::lock_guard<std::mutex> lock(uiCallbackMutex);
		uiCallbacks.push_back(std::move(func));
	}
	PostMessage(windowHandle, WM_RUN_ON_UI_THREAD, 0, 0);
}
