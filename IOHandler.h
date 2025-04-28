#pragma once

#include <string>

namespace IOHandler
{
	std::string GetCurrentWorkingDirectory();
	std::string GetUserSelectedDirectory(const std::string& basePath);
	std::string GetUserSelectedFile(const std::string& basePath);
	std::string ReadFile(const std::string& filePath);
	void WriteFile(const std::string& filePath, const std::string& content);
	void AppendToFile(const std::string& filePath, const std::string& content);
};

