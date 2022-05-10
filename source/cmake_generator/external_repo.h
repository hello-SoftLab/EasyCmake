#pragma once
#include "repository.h"
#include "library_settings.h"
#include "include_settings.h"



class ExternalRepository : public Repository {
public:
	


private:
	std::string m_RepoLocation="";
	std::string m_GitTag = "";
	bool m_ShouldBuild = false;
	std::string m_SourcesToAdd = "";
	std::vector<IncludeSettings> m_Includes;
	std::vector<LibrarySettings> m_Libraries;
	std::string m_CmakeArgs = "";
 
	void SetupPopupWidgets() override;
	size_t GetNumberOf(std::string name) override;
	bool IsRepoReady(std::string& errorMsg) override;
	bool CheckIfLocationAlreadyRegistered();

};