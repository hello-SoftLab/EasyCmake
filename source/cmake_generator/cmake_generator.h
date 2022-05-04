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
	std::vector<RepositoryHandle> repositories;
	RepositoryHandle tempRepo;
};

class CMakeGenerator {
public:


	static void ShowMainWindow();

private:
	static void ShowPopupForRepo(RepositoryHandle& handle);
	static void ValidateRepos();
	static void ShowRepoCreateMenu();
	static inline CmakeGeneratorProperties m_Properties;
	


};

