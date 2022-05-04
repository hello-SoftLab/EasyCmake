#pragma once
#include "../global.h"


struct CmakeGeneratorProperties {
	std::string currentDirectory = std::filesystem::current_path().lexically_normal().string();
	bool shouldCreateLibrary = false;
	std::string targetName = "";
	std::string cppStandard = "C++20";
	std::string sourceFiles = "";
	std::string includeDirectories = "";

};

class CMakeGenerator {
public:


	static void ShowMainWindow();

private:
	static inline CmakeGeneratorProperties m_Properties;


};

