// car_edger.cpp : ClassRegisterer Class. Used to register window classes and remeber which have already been registered.
// Author: Jan K�ck
// Created: 2025/02/26
//

#pragma once

#include <set>
#include <string>
#include <unordered_map>

#include <windows.h>

class ClassRegisterer
{
private:
	static ClassRegisterer* instance;
	ClassRegisterer() = default;

	std::unordered_map<std::wstring, WNDCLASSEXW> classCache;
public:
	ClassRegisterer(const ClassRegisterer&) = delete;
	ClassRegisterer& operator=(const ClassRegisterer&) = delete;

	static ClassRegisterer* GetInstance();
	ATOM registerClass(const std::wstring classname, WNDCLASSEXW clazz);
	bool classExists(const std::wstring classname);
	WNDCLASSEXW getWindowClass(const std::wstring classname);
	bool unregisterClasses();
};
