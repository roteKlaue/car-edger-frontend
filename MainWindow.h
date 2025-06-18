#pragma once

#include "Window.h"
#include <thread>
#include <atomic>
#include <vector>
#include <functional>
#include <mutex>

/**
 * @class MainWindow
 * @brief Represents the main application window that supports thread-safe communication
 *        between a UI thread and a background worker thread.
 *
 * Inherits from `Window`. Manages the lifecycle of the GUI and provides mechanisms to run
 * background tasks and safely post callbacks to the UI thread.
 */
class MainWindow : 
	public Window
{
public:
    /**
     * @brief Constructs the main window and starts the background thread.
     * @param instance Handle to the current application instance.
     * @param nCmdShow Controls how the window is to be shown.
     */
    MainWindow(HINSTANCE instance, int nCmdShow);

    /**
     * @brief Sets the title of the window.
     * @param title New window title as a wide string.
     */
    void SetTitle(const std::wstring& title) override;
    
    /**
     * @brief Queues a task to be executed on the background thread.
     * @param func The function to run in the background.
     */
    void RunOnBackgroundThread(std::function<void()> func) override;


    /**
     * @brief Queues a task to be executed on the UI thread.
     * @param func The function to run on the UI thread.
     */
    void PostToUIThread(std::function<void()> func) override;

    /**
     * @brief Stops the background thread and performs cleanup.
     */
	void Cleanup() override;

    /**
     * @brief Ensures that the current thread is the UI thread.
     * @throws std::logic_error in debug builds if not on UI thread.
     */
    void AssertBGThread() const override;

    /**
     * @brief Ensures that the current thread is the background thread.
     * @throws std::logic_error in debug builds if not on background thread.
     */
    void AssertUIThread() const override;
protected:
    /**
     * @brief Handles Windows messages, including custom UI-thread callback dispatching.
     * @param msg Windows message ID.
     * @param wp WPARAM.
     * @param lp LPARAM.
     * @return LRESULT for message processing.
     */
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;

private:
    /// Main loop for executing background tasks.
    void BackgroundLoop();

    /// Stops and joins the background thread safely.
    void StopBackgroundThread();

    std::thread backgroundThread;                   ///< Background thread instance.
    std::atomic<bool> running{ false };             ///< Flag indicating if background loop is active.

    std::mutex backlogMutex;                        ///< Mutex for guarding background task queue.
    std::vector<std::function<void()>> backlog;     ///< Background task queue.

    std::mutex uiCallbackMutex;                     ///< Mutex for guarding UI callback queue.
    std::vector<std::function<void()>> uiCallbacks; ///< UI thread task queue.

    std::thread::id uiThreadId;                     ///< Thread ID of the UI thread.
    std::thread::id bgThreadId;                     ///< Thread ID of the background thread.
};
