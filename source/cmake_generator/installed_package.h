#pragma once
#include "repository.h"


class InstalledPackage : public Repository {
public:
	InstalledPackage();


private:
	std::string m_PackageName = "";
	bool m_Required = false;


	bool OnDeserialize(YAML::Node& node) override;
	YAML::Node OnSerialize() override;
	std::string GetCMakeListsString() override;
	void SetupPopupWidgets() override;
	size_t GetNumberOf(std::string name) override;
	bool IsRepoReady() override;


};