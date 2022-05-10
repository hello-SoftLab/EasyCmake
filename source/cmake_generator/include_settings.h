#pragma once
#include "settings_descriptor.h"



struct IncludeSettings {
	std::string path;
	std::string access;

	static void ShowWidgets(std::vector<IncludeSettings>& includes);

};
