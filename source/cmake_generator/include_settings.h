#pragma once
#include "settings_descriptor.h"



struct IncludeSettings {
	std::string path = "";
	std::string access = "PUBLIC";

	YAML::Node Serialize();
	bool Deserialize(YAML::Node& node);


	static void ShowWidgets(std::vector<IncludeSettings>& includes);

};
