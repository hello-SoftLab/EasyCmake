#pragma once
#include "repository.h"
#include "library_settings.h"
#include "include_settings.h"



class ExternalRepository : public Repository {
public:
	ExternalRepository();

	bool ShouldBuild();

private:
	std::string m_RepoLocation="";
	std::string m_GitTag = "";
	bool m_ShouldBuild = false;
	std::string m_CmakeArgs = "";
 
	bool OnDeserialize(YAML::Node& node) override;
	YAML::Node OnSerialize() override;
	std::string GetCMakeArgs();
	std::string GetCMakeListsString() override;
	void SetupPopupWidgets() override;
	size_t GetNumberOf(std::string name) override;
	bool IsRepoReady() override;
	bool CheckIfLocationAlreadyRegistered();

};