#pragma once
#include "settings_descriptor.h"



struct LibrarySettings {

	std::string path = "";
	std::string debugPostfix = "";
	std::string access = "PUBLIC";

	static void ShowWidgets(std::vector<LibrarySettings>& libraries);

};



