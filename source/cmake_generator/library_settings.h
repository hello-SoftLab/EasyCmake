#pragma once
#include "settings_descriptor.h"



struct LibrarySettings {

	std::string path = "";
	std::string debugPostfix = "";
	bool isVariableName=false;
	std::string access = "PUBLIC";
	bool shared = false;

	YAML::Node Serialize();
	bool Deserialize(YAML::Node& node);

	static void ShowWidgets(std::vector<LibrarySettings>& libraries);

};



