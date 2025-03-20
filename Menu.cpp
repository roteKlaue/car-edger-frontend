#include "Menu.h"
#include "Window.h"

static UINT s_nextMenuId = 1;

static UINT GenerateId() {
    return s_nextMenuId++;
}

Menu::Menu(MenuType type) : type(type), id(GenerateId()) {
    switch (type) {
    case MenuType::MainMenu:
        hMenu = CreateMenu();
        break;
    case MenuType::PopupMenu:
    case MenuType::SystemMenu:
        hMenu = CreatePopupMenu();
        break;
    }
}

Menu& Menu::AddSubMenu(Menu *submenu) {
    if (!hMenu || !submenu->hMenu) {
        return *this;
    }

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)submenu->hMenu, submenu->label.c_str());
    subMenus.push_back(submenu);

    return *this;
}

Menu& Menu::AddOption(Option* option)
{
    if (!hMenu) return *this;
    AppendMenuW(hMenu, MF_STRING, option->id, option->label.c_str());
    options[option->id] = option;
    return *this;
}

Menu& Menu::AddDivider() {
    if (hMenu) AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    return *this;
}

void Menu::Register(Window* window, Menu *parent)
{
    if (!window || !hMenu) {
        return;
    }
    return;

    HWND hwnd = window->GetWindowHandle();

    if (type == MenuType::MainMenu) {
        SetMenu(hwnd, hMenu);
        DrawMenuBar(hwnd);
    }

    for (auto& pair : options)
    {
        pair.second->Init(window, this);
    }

    for (Menu *men : subMenus) {
        if (men == nullptr) continue;
        men->Register(window, this);
    }
}

void Menu::PropagateClick(UINT id)
{
    auto found = options.find(id);
    if (found != options.end()) {
        found->second->OnClick();
        return;
    }

    for (Menu* men : subMenus) {
        if (men == nullptr) continue;
        men->PropagateClick(id);
    }
}

Menu& Menu::SetLabel(const std::wstring& menuLabel) {
    label = menuLabel;
    return *this;
}

Option::Option(std::wstring label) : label(label), id(GenerateId()) {}

void Option::Init(Window* wWindow, Menu* parent)
{
    menu = parent;
    window = wWindow;
}
