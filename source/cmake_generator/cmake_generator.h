#pragma once
#include "../global.h"
#include "repository.h"
#include "library_settings.h"
#include "include_settings.h"


struct TargetGenerator {
	std::string name = "";
	bool isLibrary = "";
	std::string sourceFiles = "";
	std::string cppStandard = "C++20";
	std::vector<SettingsHolder> includes;
	std::vector<SettingsHolder> libraries;

	void ShowWidgets();
};


struct CmakeGeneratorProperties {
	std::string currentDirectory = std::filesystem::current_path().lexically_normal().string();
	bool shouldCreateLibrary = false;
	std::string projectName = "";
	std::vector<PointerHolder<TargetGenerator>> targets;
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
	static void GenerateCMakeLists();
	static bool ValidateInputs();
	static void AddSourceFiles(std::string& stringToAdd);
	static void AddIncludeDirectories(std::string& stringToAdd);
	static void AddInitialDetails(std::string& stringToAdd);
	static void AddExecutableAndSetDetails(std::string& stringToAdd);

	static inline CmakeGeneratorProperties m_Properties;
	


};

