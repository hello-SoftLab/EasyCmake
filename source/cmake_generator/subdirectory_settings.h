#pragma once
#include "../global.h"



class SubdirectorySettings {
public:
	std::string subdir;

	static void ShowWidgets(std::vector<SubdirectorySettings>& vec);
};