#pragma once

#include "repository.h"

class CMakeSerializer {
public:
	static bool GenerateCMakeLists(const CmakeGeneratorProperties& prop);
	static bool SaveCurrentConfigsToFile(std::string fileName);
	static bool LoadConfigsFromFile(std::string fileName);
	static bool SerializeToSave(std::string name);
	static bool DeserializeSavedConfig(std::string name);
	static bool RemoveSave(std::string name);
	static const YAML::Node& GetSavedConfigs();

private:
	static inline YAML::Node m_SavedConfigs;

};