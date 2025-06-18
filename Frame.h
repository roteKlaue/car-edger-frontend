// Frame.h : Header file for the Frame class definition.
// Author: Jan Koeck
// Created: 2025/05/06
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <memory.h>

class Component;
class Window;
using json = nlohmann::json;

/**
 * @brief Abstract base class for UI frames containing GUI components.
 *
 * A Frame serves as a container for multiple GUI components and is responsible
 * for managing their lifecycle, layout, and event forwarding. Concrete implementations
 * define the visual and functional layout by registering components.
 *
 * Each Frame is associated with a parent Window and is intended to be attached
 * to it during application runtime.
 */
class Frame {
public:
    /// Return value indicating that a message was not handled.
    static constexpr LRESULT NOT_HANDLED = -1;
    /// Virtual destructor for safe cleanup in derived classes.
    virtual ~Frame() = default;

    /**
     * @brief Returns a globally unique identifier string for this Frame.
     *
     * Used to differentiate between loaded frames at runtime (e.g., for switching views).
     * @return Unique string identifier.
     */
    virtual std::string GetUniqueIdentifier() const = 0;

    /**
     * @brief Attaches this Frame to a Window and prepares its components.
     *
     * Creates and shows all registered components. Should be called once after the Frame
     * is ready to be displayed.
     *
     * @param win Pointer to the parent Window.
     * @param options Optional JSON object with configuration values.
     */
    virtual void WakeUp(Window* win, const json& options);

    /**
     * @brief Unloads this Frame and destroys all its components.
     *
     * Frees all GUI elements and resets the component list.
     * @param win Pointer to the parent Window (unused).
     */
    virtual void Unload(Window* /*win*/);

    /**
     * @brief Handles a Windows message and optionally forwards it to components.
     *
     * Default implementation handles `WM_COMMAND` messages and forwards them
     * to matching components by ID.
     *
     * @param win Pointer to the parent Window.
     * @param msg Message identifier.
     * @param wp WPARAM value.
     * @param lp LPARAM value.
     * @return `0` if handled, `NOT_HANDLED` otherwise.
     */
    virtual LRESULT HandleMessage(Window* /*win*/, UINT msg, WPARAM wp, LPARAM lp);

       
    /**
      * @brief Registers a component with this Frame.
      *
      * The component will be managed, resized, and drawn by the Frame.
      *
      * @param comp Shared pointer to the component to register.
      */
    void RegisterComponent(std::shared_ptr<Component> comp);

    /**
     * @brief Called during WM_PAINT to allow drawing text-only components.
     * Only `Text` components are currently supported for direct painting.
     *
     * @param hdc Handle to device context.
     */
    virtual void OnPaint(HDC hdc);

    /**
     * @brief Returns the native window handle associated with this Frame.
     * @return HWND of the parent window or `nullptr` if not attached.
     */
    HWND GetWindowHandle() const;

    /**
     * @brief Called when the parent window is resized.
     *
     * Forwards resize info to all registered components.
     *
     * @param width New width of the parent window.
     * @param height New height of the parent window.
     */
    void OnParentResize(unsigned int width, unsigned int height);

    /**
     * @brief Forwards a WM_NOTIFY message to the correct component.
     *
     * Used for components such as ListViews or TreeViews that use NMHDR.
     *
     * @param nmhdr Pointer to the notification header.
     */
    void HandleNotify(NMHDR* nmhdr);

protected:
    Window* win = nullptr;                              ///< Pointer to the parent window.
    std::vector<std::shared_ptr<Component>> components; ///< List of all registered GUI components.
};
