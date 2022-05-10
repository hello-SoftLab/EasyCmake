#pragma once
#include "repository.h"





class ExternalRepository : public Repository {
public:
	


private:
	std::string m_RepoLocation="";
	std::string m_GitTag = "";
	bool m_ShouldBuild = false;
	std::string m_SourcesToAdd = "";
	std::string m_IncludePaths = "";
	std::string m_Libraries = "";
	std::string m_CmakeArgs = "";
 
	void SetupPopupWidgets() override;
	size_t GetNumberOf(std::string name) override;

};