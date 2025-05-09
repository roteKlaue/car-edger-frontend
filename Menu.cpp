#include "pch.h"
// #include "Menu.h"
// #include "Window.h"
// #include <iostream>
// #include "Util.h"
// 
// Menu::Menu(MenuType type) : type(type), id(Util::GenerateId()) {
//     std::cout << "Creating menu of type: " << (int)type << std::endl;
// }
// 
// Menu& Menu::AddSubMenu(Menu* submenu) {
//     std::cout << "Adding submenu: " << submenu->id << std::endl;
//     subMenus.push_back(submenu);
//     return *this;
// }
// 
// Menu& Menu::AddOption(Option* option)
// {
//     if (!hMenu) return *this;
//     // AppendMenuW(hMenu, MF_STRING, option->id, option->label.c_str());
//     options[option->id] = option;
//     return *this;
// }
// 
// Menu& Menu::AddDivider() {
//     if (hMenu) AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
//     return *this;
// }
// 
// void Menu::Register(Window* window, Menu* parent)
// {
//     switch (type) {
//     case MenuType::MainMenu:
//         hMenu = CreateMenu();
//         if (!hMenu) {
//             std::cerr << "Failed to create MainMenu!" << std::endl;
//         }
//     case MenuType::PopupMenu:
//     case MenuType::SystemMenu:
//         hMenu = CreatePopupMenu();
//         if (!hMenu) {
//             std::cerr << "Failed to create PopupMenu or SystemMenu!" << std::endl;
//         }
//         break;
//     }
// 
//     std::cout << "Menu object address: " << this << std::endl;
//     std::cout << "Menu type: " << (int)type << std::endl;
//     std::cout << "Menu hMenu handle: " << hMenu << std::endl;
// 
//     std::cout << "Register - hMenu: " << hMenu << std::endl;
// 
//     if (window == nullptr || hMenu == nullptr) {
//         std::cout << "Missing window or hMenu" << std::endl;
//         return;
//     }
// 
//     HWND hwnd = window->GetWindowHandle();
// 
//     /*for (auto& pair : options)
//     {
//         std::cout << "Address of pair: " << &pair << std::endl;
//         if (pair.second != nullptr) {
//             std::cout << "Address of pair.second: " << pair.second << std::endl;
//             pair.second->Init(window, this);
//         } else {
//             std::cout << "Warning: pair.second is null!" << std::endl;
//         }
//     }*/
// 
//     for (Menu* men : subMenus) {
//         if (men == nullptr) continue;
//         men->Register(window, this);
//     }
// 
//     if (type == MenuType::MainMenu) {
//         SetMenu(hwnd, hMenu);
//         DrawMenuBar(hwnd);
//         return;
//     }
// 
//     // testing here
// 
//     if (!parent) {
//         std::cout << "Missing parent for NON MainMenu type menu" << std::endl;
//         return;
//     }
//     std::cout << "Missing parent for NON MainMenu type menu" << std::endl;
// 
//     AppendMenu(hMenu, MF_STRING, 1, L"&New");
//     AppendMenu(hMenu, MF_STRING, 2, L"&Open");
//     AppendMenu(hMenu, MF_STRING, 3, L"&Save");
//     AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
//     AppendMenu(hMenu, MF_STRING, 4, L"E&xit");
// 
//     AppendMenu(parent->GetHandle(), MF_POPUP, (UINT_PTR)hMenu, label.c_str());
// }
// 
// void Menu::PropagateClick(UINT id)
// {
//     auto found = options.find(id);
//     if (found != options.end()) {
//         found->second->OnClick();
//         return;
//     }
// 
//     for (Menu* men : subMenus) {
//         if (men == nullptr) continue;
//         men->PropagateClick(id);
//     }
// }
// 
// Menu& Menu::SetLabel(const std::wstring& menuLabel) {
//     label = menuLabel;
//     return *this;
// }
// 
// Option::Option(std::wstring label) : label(label), id(Util::GenerateId()) {}
// 
// void Option::Init(Window* wWindow, Menu* parent)
// {
//     menu = parent;
//     window = wWindow;
// }
// 