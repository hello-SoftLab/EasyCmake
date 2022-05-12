#pragma once
#include "settings_descriptor.h"



struct IncludeSettings {
	std::string path = "";
	std::string access = "PUBLIC";

	static void ShowWidgets(std::vector<IncludeSettings>& includes);

};
