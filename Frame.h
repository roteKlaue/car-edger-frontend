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

class Frame {
public:
    static constexpr LRESULT NOT_HANDLED = -1;
    virtual ~Frame() = default;

    virtual std::string GetUniqueIdentifier() const = 0;

    virtual void WakeUp(Window* win, const json& options);
    virtual void Unload(Window* /*win*/);

    virtual LRESULT HandleMessage(Window* /*win*/, UINT msg, WPARAM wp, LPARAM lp);

    void RegisterComponent(std::shared_ptr<Component> comp);
    virtual void OnPaint(HDC hdc);

    HWND GetWindowHandle() const;
    void OnParentResize(unsigned int width, unsigned int height);
    void HandleNotify(NMHDR* nmhdr);

protected:
    Window* win = nullptr;
    std::vector<std::shared_ptr<Component>> components;
};