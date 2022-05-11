#pragma once
#include "../global.h"
#include "repository.h"
#include "library_settings.h"
#include "include_settings.h"





class CMakeGenerator {
public:

	static bool FindAliasInRepositories(std::string alias);
	static const std::vector<RepositoryHandle>& Repositories();
	static CmakeGeneratorProperties& Settings();
	static void ShowMainWindow();

private:
	static void ShowPopupForRepo(RepositoryHandle& handle);
	static void ValidateRepos();
	static void ShowRepoCreateMenu();
	
	static bool ValidateInputs();
	

	static inline CmakeGeneratorProperties m_Properties;
	


};

