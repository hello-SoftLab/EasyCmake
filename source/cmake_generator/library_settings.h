#pragma once
#include "settings_descriptor.h"


class LibrarySettings : public SettingsDescriptor {



private:
	std::string m_Path = "";
	std::string m_DebugPostfix = "";

};




