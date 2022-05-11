#pragma once
#include "../global.h"
#include "pointer_holder.h"

struct TargetGenerator {
	std::string name = "";
	std::string type = "Executable";
	std::string sourceFiles = "";
	std::string cppStandard = "C++20";
	std::vector<IncludeSettings> includes;
	std::vector<LibrarySettings> libraries;
	std::vector<RepositoryHandle> externalRepos;

	void ShowWidgets();
};


struct CmakeGeneratorProperties {
	std::string currentDirectory = std::filesystem::current_path().lexically_normal().string();
	std::string projectName = "";
	std::string cmakeVersion = "";
	std::vector<PointerHolder<TargetGenerator>> targets;
	std::vector<RepositoryHandle> repositories;
	RepositoryHandle tempRepo;
};

class Repository {
public:
	Repository();

	virtual void SetupPopupWidgets();
	bool CheckRepoValidity(std::string& errorMsg);

	virtual size_t GetNumberOf(std::string type);

	virtual std::string GetCMakeListsString();

	void OpenPopup();
	void ClosePopup();

	bool IsPopupOpen();


	

	std::string GetPopupID();

	std::string GetHash();

	std::string GetAlias();

protected:
	virtual bool IsRepoReady(std::string& errorMsg);

	std::string m_Alias = "";
private:
	bool m_ShouldOpenPopup = false;
	
	friend class CMakeGenerator;
};

class RepositoryHandle : public PointerHolder<Repository> {

};