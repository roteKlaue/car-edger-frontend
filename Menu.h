// Menu.h : Menu bar menu handling class.
// Author: Jan Koeck
// Created: 2025/03/01
//

#pragma once

#include <windows.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class Menu;
class Option;
class Window;

enum class MenuType {
    MainMenu,
    PopupMenu,
    SystemMenu
};

class Menu {
public:
    explicit Menu(MenuType type = MenuType::MainMenu);
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;
    Menu& AddSubMenu(Menu* submenu);
    Menu& AddOption(Option* option);
    Menu& AddDivider();
    Menu& SetLabel(const std::wstring& menuLabel);
    HMENU GetHandle() const { return hMenu; }
    void Register(Window* window, Menu *parent);
    void PropagateClick(UINT id);

private:
    MenuType type;
    HMENU hMenu;
    std::vector<Menu*> subMenus;
    std::unordered_map<UINT, Option*> options;
    std::wstring label;
    UINT id;
};

class Option {
public:
    Option(std::wstring label);
    void Init(Window *window, Menu *parent);
    std::wstring label;
    virtual void OnClick() {};
    UINT id;

protected:
    Window* window;
    Menu* menu;
};
