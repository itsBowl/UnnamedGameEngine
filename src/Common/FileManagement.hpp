#pragma once
#include <vector>
#include <filesystem>
#include <string>
#include <utility>
#include <fstream>

namespace EngineCore::FileManagement
{
	bool getFilesInFolder(std::vector<std::string>* loc, std::string folder);

	bool getFilesInFolder(std::vector<std::pair<std::string, std::string>>* loc, std::string folder);
	
	std::filesystem::path getFile(std::string, std::string);

	bool readFile(const std::string& fileName, std::vector<char>* dst);
}