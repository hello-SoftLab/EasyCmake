#pragma once

#include "repository.h"

class CMakeSerializer {
public:
	static bool GenerateCMakeLists(const CmakeGeneratorProperties& prop);


	static void Init();
	
	
	static bool SerializeToSave(std::string name);
	static bool DeserializeSavedConfig(std::string name);
	static bool RemoveSave(std::string name);
	static const YAML::Node& GetSavedConfigs();

	static bool HasDirectoryBeenUsedBefore();

	static bool SaveCurrentToSavedDirectories();
	static bool LoadCurrentFromSavedDirectories();

	static bool SaveCurrentToCache();
	static bool LoadCurrentFromCache();

private:
	static bool SaveCurrentConfigsToFile(std::string fileName);
	static bool LoadConfigsFromFile(std::string fileName);

	static bool SaveCurrentSavedDiretoriesToFile(std::string fileName);
	static bool LoadCurrentSavedDirectoriesFromFile(std::string fileName);

	static bool DeserializeFromNode(YAML::Node& node);
	static YAML::Node SerializeToNode();

	static inline YAML::Node m_SavedDirectories;
	static inline YAML::Node m_Cache;
	static inline YAML::Node m_SavedConfigs;

};