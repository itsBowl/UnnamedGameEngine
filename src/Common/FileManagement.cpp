#include "PCH.hpp"
#include "FileManagement.hpp"

namespace EngineCore::FileManagement
{
	bool getFilesInFolder(std::vector<std::string>* loc, std::string folder)
	{
		if (loc == nullptr || folder == "") return false;
		std::filesystem::directory_iterator fsIt = std::filesystem::directory_iterator(folder);

		for (const auto& entry : std::filesystem::directory_iterator(fsIt))
		{
			auto path = entry.path().string();
			loc->push_back(path);
		}
		return true;
	}

	bool getFilesInFolder(std::vector<std::pair<std::string, std::string>>* loc, std::string folder)
	{
		if (loc == nullptr || folder == "") return false;
		std::filesystem::directory_iterator fsIt = std::filesystem::directory_iterator(folder);

		for (const auto& entry : std::filesystem::directory_iterator(fsIt))
		{
			auto path = entry.path().string();
			auto extention = entry.path().extension().string();
			loc->push_back(std::make_pair(path, extention));
		}
		return true;
	}

	std::filesystem::path getFile(std::string name, std::string path)
	{
		std::vector<std::pair<std::string, std::string>> files;
		getFilesInFolder(&files, path);
		for (auto& i : files)
		{
			if (i.first == name)
			{
				return i.first;
			}
		}
		return "";
	}

	bool readFile(const std::string& fileName, std::vector<char>* dst)
	{
		std::ifstream FILE(fileName, std::ios::ate | std::ios::binary);
		if (!FILE.is_open()) return false;
		size_t fileSize = (size_t)FILE.tellg();
		if (fileSize == 0) return false;
		dst->resize(fileSize + 1);

		FILE.seekg(std::ios::beg);
		FILE.read(dst->data(), fileSize);
		FILE.close();
		(*dst)[fileSize] = '\0';

		return true;
	}
}