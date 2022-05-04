#pragma once
#include "../global.h"
#include "repository.h"

struct CmakeGeneratorProperties {
	std::string currentDirectory = std::filesystem::current_path().lexically_normal().string();
	bool shouldCreateLibrary = false;
	std::string targetName = "";
	std::string cppStandard = "C++20";
	std::string sourceFiles = "";
	std::string includeDirectories = "";
	std::unordered_map<std::string, std::shared_ptr<Repository>> repositories;
};

class CMakeGenerator {
public:


	static void ShowMainWindow();

private:
	static void ShowRepoCreateMenu();
	static inline CmakeGeneratorProperties m_Properties;
	


};

