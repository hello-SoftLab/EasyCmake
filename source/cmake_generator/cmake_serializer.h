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

	static bool SaveCurrentToCache();
	static bool LoadCurrentFromCache();

private:

	static bool DeserializeFromNode(YAML::Node& node);
	static YAML::Node SerializeToNode();

	static inline YAML::Node m_Cache;
	static inline YAML::Node m_SavedConfigs;

};