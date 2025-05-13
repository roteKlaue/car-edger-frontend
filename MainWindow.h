#pragma once

#include "Window.h"
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <mutex>

class MainWindow : 
	public Window
{
public:
    MainWindow(HINSTANCE instance, int nCmdShow);
	void SetTitle(const std::wstring& title) override;
    
    void RunOnBackgroundThread(std::function<void()> func) override;
    void PostToUIThread(std::function<void()> func) override;
	void Cleanup() override;
    void AssertBGThread() const override;
    void AssertUIThread() const override;
protected:
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;

private:
    void BackgroundLoop();
    void StopBackgroundThread();

    std::thread backgroundThread;
    std::atomic<bool> running{false};

    std::mutex backlogMutex;
    std::vector<std::function<void()>> backlog;

    std::mutex uiCallbackMutex;
    std::vector<std::function<void()>> uiCallbacks;

    std::thread::id uiThreadId;
    std::thread::id bgThreadId;
};
