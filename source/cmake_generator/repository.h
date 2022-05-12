#pragma once
#include "../global.h"
#include "pointer_holder.h"
#include "library_settings.h"
#include "include_settings.h"
#include "subdirectory_settings.h"


class Repository {
public:
	Repository();

	virtual void SetupPopupWidgets();
	bool CheckRepoValidity();

	virtual size_t GetNumberOf(std::string type);

	virtual std::string GetCMakeListsString();

	void OpenPopup();
	void ClosePopup();

	bool IsPopupOpen();




	std::string GetPopupID();

	std::string GetHash();

	std::string GetAlias();

	const std::vector<IncludeSettings>& GetIncludes() const;
	std::vector<std::string> GetSources() const;
	const std::vector<LibrarySettings>& GetLibraries() const;

protected:
	virtual bool IsRepoReady();

	std::string m_SourcesToAdd = "";
	std::vector<IncludeSettings> m_Includes;
	std::vector<LibrarySettings> m_Libraries;
	std::string m_Alias = "";
private:
	bool m_ShouldOpenPopup = false;

	friend class CMakeGenerator;
};

class RepositoryHandle : public PointerHolder<Repository> {

};

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
	std::vector<SubdirectorySettings> subdirectories;
	std::vector<PointerHolder<TargetGenerator>> targets;
	std::vector<RepositoryHandle> repositories;
	RepositoryHandle tempRepo;
};

