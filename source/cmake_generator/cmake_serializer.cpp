#include "cmake_serializer.h"
#include "external_repo.h"
#include <fstream>
#include "cmake_generator.h"
#include "../window/window.h"

bool CMakeSerializer::GenerateCMakeLists(const CmakeGeneratorProperties& prop)
{
	std::string stringToAdd = "";

	stringToAdd += R"(

#this CMakeLists was created with EasyCmake - V2 
#the repository can be found at https://github.com/knz13/EasyCmake_Cpp

)";

	stringToAdd += R"(
cmake_minimum_required(VERSION )" + fmt::format("{})\n\n", prop.cmakeVersion);

	stringToAdd += R"(

#adding useful functions

function(DIR_EXISTS variable dir_path)

file(GLOB ${variable}_check ${dir_path}/*)

list(LENGTH ${variable}_check ${variable}_len)

if(${${variable}_len} EQUAL 0)

set(${variable} FALSE PARENT_SCOPE)

else()

set(${variable} TRUE PARENT_SCOPE)

endif()

endfunction()

#adding extra cmake libs
include(GNUInstallDirs)
include(ExternalProject)
include(FetchContent)

#project name
)";



	stringToAdd += fmt::format(R"(project("{}")
)", prop.projectName).c_str();


	if (prop.subdirectories.size() > 0) {
		stringToAdd += R"(
#adding subdirectories...
)";

		for (auto& subdir : prop.subdirectories) {

			stringToAdd += fmt::format(R"(
add_subdirectory({})
)", subdir.subdir);

		}

		stringToAdd += "\n";
	}


	if (prop.repositories.size() > 0) {

		stringToAdd += R"(
# --------------------- Repository declarations ------------------------

)";



		for (auto& repo : prop.repositories) {
			if (!repo) {
				continue;
			}
			stringToAdd += repo.Get()->GetCMakeListsString();
		}
	}

	
	for (auto& target : prop.targets) {

		if (!target) {
			continue;
		}

		if (target.Get()->type == "Library") {
			stringToAdd += R"(
#creating library
add_library()" + fmt::format("{}\n", target.Get()->name);
		}

		if (target.Get()->type == "Executable") {
			stringToAdd += R"(
#creating executable
add_executable()" + fmt::format("{}\n\n",target.Get()->name);
		}

		for (auto& location : HelperFunctions::SplitString(target.Get()->sourceFiles, "\n")) {
			stringToAdd += R"(
	${PROJECT_SOURCE_DIR}/)" + fmt::format("{}\n",location);
		}

		std::vector<std::string> externalRepoCount;
		std::vector<std::pair<std::string,LibrarySettings>> externalRepoLibraries;
		std::vector<std::pair<std::string, IncludeSettings>> externalRepoIncludes;
		
		for (auto& repo : target.Get()->externalRepos) {
			if (!repo) {
				continue;
			}
			for (auto& location : repo.Get()->GetSources()) {
				stringToAdd += R"(
	${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}/",repo.Get()->GetAlias()) + fmt::format("{}",location);
			}
			if (repo.Get()->GetLibraries().size() > 0) {
				for (auto& library : repo.Get()->GetLibraries()) {
					externalRepoLibraries.push_back(std::make_pair(repo.Get()->GetAlias(),library));
				}
			}

			if (repo.Get()->GetIncludes().size() > 0) {
				for (auto& include : repo.Get()->GetIncludes()) {
					externalRepoIncludes.push_back(std::make_pair(repo.Get()->GetAlias(),include));
				}

				
			}


			if (repo.IsHoldingType<ExternalRepository>() && repo.GetAs<ExternalRepository>()->ShouldBuild()) {
				externalRepoCount.push_back(repo.Get()->GetAlias());
			}
		}

		stringToAdd += R"(
)

set_property(TARGET )" + fmt::format("{}", target.Get()->name) + R"( PROPERTY CXX_STANDARD )" + fmt::format("{})\n\n", target.Get()->cppStandard.substr(3));

		
		if (externalRepoCount.size() > 0) {
			stringToAdd += R"(
#setting dependencies...

)";

			for (auto& dep : externalRepoCount) {

				stringToAdd += R"(
if(NOT ${)" + fmt::format("{}_exists", dep) + R"(})
	add_dependencies()" + fmt::format("{} ", target.Get()->name) + fmt::format("{})", dep) + R"(
endif()
)";
			}
		}

		if (target.Get()->libraries.size() > 0 || externalRepoLibraries.size() > 0) {
			stringToAdd += R"(
#adding libraries...

)";

			for (auto& library : target.Get()->libraries) {
				stringToAdd += R"(
target_link_libraries()" + fmt::format("{} ",target.Get()->name) + fmt::format("{} ",library.access);

				
				stringToAdd += "${PROJECT_SOURCE_DIR}/" + fmt::format("{})\n", library.path);
				

			}

			for (auto& [name,library] : externalRepoLibraries) {
				stringToAdd += R"(
target_link_libraries()" + fmt::format("{} ", target.Get()->name) + fmt::format("{} ", library.access);

				std::string libraryName = library.path;

				if (size_t location = library.path.find_last_of('/'); location != std::string::npos) {
					libraryName = library.path.substr(0, location);
					libraryName += "${CMAKE_STATIC_LIBRARY_PREFIX}";
					libraryName += fmt::format("{}$<$<CONFIG:Debug>:{}>", library.path.substr(location), library.debugPostfix);
					libraryName += "${CMAKE_STATIC_LIBRARY_SUFFIX}";
				}

				stringToAdd += R"(${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}/", name) + libraryName + ")\n";
				
			}

		}

		if (target.Get()->includes.size() > 0 || externalRepoIncludes.size() > 0) {
			stringToAdd += R"(

#adding includes...

)";

			for (auto& include : target.Get()->includes) {
				stringToAdd += R"(
target_include_directories()" + fmt::format("{} ", target.Get()->name) + fmt::format("{} ", include.access) + R"(${PROJECT_SOURCE_DIR}/)" +
	fmt::format("{})\n",include.path);
			}

			for (auto& [name,include] : externalRepoIncludes) {
				stringToAdd += R"(
target_include_directories()" + fmt::format("{} ", target.Get()->name) + fmt::format("{} ", include.access) + R"(${PROJECT_SOURCE_DIR}/vendor/)" +
fmt::format("{}/",name) + fmt::format("{})\n", include.path);
			}
		}


	};
	std::string path = prop.currentDirectory;

	path += "\\CMakeLists.txt";


	std::ofstream stream(path);

	if (!stream.is_open()) {
		return false;
	}

	stream << stringToAdd;

	stream.close();


	return true;
}

bool CMakeSerializer::InitRecentRepositories()
{
	if (std::filesystem::exists("recent_repos.yaml")) {
		m_RecentRepositories = YAML::LoadFile("recent_repos.yaml");
	}

	Window::GetCurrentWindow().OnClosing().Connect([]() {
		std::ofstream stream;

		stream.open("recent_repos.yaml");

		if (!stream.is_open()) {
			return;
		}

		stream << m_RecentRepositories;

		stream.close();

	});
	return true;
}

bool CMakeSerializer::SaveCurrentConfigsToFile(std::string fileName)
{

	std::ofstream stream;
	stream.open(fileName);

	if (!stream.is_open()) {
		DEBUG_LOG("Could not save to " + fileName);
		return false;
	}

	stream << m_SavedConfigs;

	return true;
}

bool CMakeSerializer::LoadConfigsFromFile(std::string fileName)
{
	if (!std::filesystem::exists(fileName)) {
		return false;
	}

	m_SavedConfigs = YAML::LoadFile(fileName);

	return true;
}

bool CMakeSerializer::SaveCurrentSavedDiretoriesToFile(std::string fileName)
{
	std::ofstream stream;

	stream.open(fileName);

	if (!stream.is_open()) {
		return false;
	}

	stream << m_SavedDirectories;

	stream.close();

	return true;
}

bool CMakeSerializer::LoadCurrentSavedDirectoriesFromFile(std::string fileName)
{
	if (!std::filesystem::exists(fileName)) {
		return false;
	}

	m_SavedDirectories = YAML::LoadFile(fileName);

	return true;
}

void CMakeSerializer::Init()
{
	CMakeSerializer::LoadConfigsFromFile("save_files.yaml");
	CMakeSerializer::LoadCurrentSavedDirectoriesFromFile("saved_directories.yaml");

	CMakeSerializer::InitRecentRepositories();

	Window::GetCurrentWindow().OnClosing().Connect([]() {
		//DEBUG_LOG("Saving...");
		CMakeSerializer::SaveCurrentSavedDiretoriesToFile("saved_directories.yaml");
		CMakeSerializer::SaveCurrentConfigsToFile("save_files.yaml");
		


		});
}

bool CMakeSerializer::AddRepoToRecent(RepositoryHandle handle)
{
	if (!handle) {
		return false;
	}

	m_RecentRepositories[handle.Get()->GetAlias()] = handle.Get()->Serialize();

	return true;
}

bool CMakeSerializer::SerializeToSave(std::string name)
{
	if (m_SavedConfigs[name].IsDefined()) {
		CMakeGenerator::ShowErrorPopup(fmt::format(R"("{}" is already defined as a save!)",name));
		return false;
	}

	

	m_SavedConfigs[name] = SerializeToNode();

	return true;

}

bool CMakeSerializer::DeserializeSavedConfig(std::string name)
{
	if (m_SavedConfigs[name].IsNull() || !m_SavedConfigs[name].IsDefined()) {
		return false;
	}

	

	YAML::Node mainNode = m_SavedConfigs[name];
	
	return DeserializeFromNode(mainNode);


}

bool CMakeSerializer::RemoveSave(std::string name)
{
	if (m_SavedConfigs[name]) {
		m_SavedConfigs.remove(name);
		return true;
	}
	return false;
}

std::string CMakeSerializer::GetSaveStringRepresentation(std::string name)
{
	if (!m_SavedConfigs[name]) {
		return "";
	}
	
	std::string returnStr = "";

	YAML::Node node = m_SavedConfigs[name];
	
	if (node["project_name"]) {
		returnStr += fmt::format(R"(Project Name: {}
)",node["project_name"].as<std::string>());
	}

	if (node["cmake_version"]) {
		returnStr += fmt::format(R"(CMake Version: {})",node["cmake_version"].as<std::string>());
	}

	if (node["targets"]) {
		returnStr += R"(
Targets:)";
		for (auto target : node["targets"]) {
			returnStr += fmt::format(R"(
	-> {})",target["name"].as<std::string>());
		}
	}

	if (node["subdirectories"]) {
		returnStr += R"(
Subdirectories:)";
		for (auto subdir : node["subdirectories"]) {
			returnStr += fmt::format(R"(
	-> {})",subdir.as<std::string>());
		}
	}

	if (node["repositories"]) {
		returnStr += R"(
External Repos:)";
		for (auto repo : node["repositories"]) {
			returnStr += fmt::format(R"(
	-> {})", repo["alias"].as<std::string>());
		}
	}
	
	return returnStr;
}

const YAML::Node& CMakeSerializer::GetSavedConfigs()
{
	return m_SavedConfigs;
}

const YAML::Node& CMakeSerializer::GetRecentRepositories()
{
	return m_RecentRepositories;
}

bool CMakeSerializer::HasDirectoryBeenUsedBefore()
{
	return m_SavedDirectories[CMakeGenerator::Settings().currentDirectory].operator bool();
}

bool CMakeSerializer::SaveCurrentToSavedDirectories()
{
	m_SavedDirectories[CMakeGenerator::Settings().currentDirectory] = SerializeToNode();

	return true;
}

bool CMakeSerializer::LoadCurrentFromSavedDirectories()
{
	if (m_SavedDirectories[CMakeGenerator::Settings().currentDirectory]) {
		YAML::Node node = m_SavedDirectories[CMakeGenerator::Settings().currentDirectory];
		DeserializeFromNode(node);
		return true;
	}

	return false;
}

bool CMakeSerializer::SaveCurrentToCache()
{
	m_Cache[CMakeGenerator::Settings().currentDirectory] = SerializeToNode();

	return true;
}

bool CMakeSerializer::LoadCurrentFromCache()
{
	if (m_Cache[CMakeGenerator::Settings().currentDirectory]) {
		YAML::Node node = m_Cache[CMakeGenerator::Settings().currentDirectory];
		return DeserializeFromNode(node);

	}

	return false;
}

bool CMakeSerializer::DeserializeFromNode(YAML::Node& mainNode)
{
	CMakeGenerator::ClearCurrentSettings();

	HelperFunctions::DeserializeVariable<std::string>("cmake_version", CMakeGenerator::Settings().cmakeVersion, mainNode);
	HelperFunctions::DeserializeVariable<std::string>("project_name", CMakeGenerator::Settings().projectName, mainNode);

	if (mainNode["subdirectories"]) {
		for (auto node : mainNode["subdirectories"]) {
			std::string var = node.as<std::string>();
			CMakeGenerator::Settings().subdirectories.push_back({ var });
		}
	}


	if (mainNode["repositories"]) {
		for (auto repo_node : mainNode["repositories"]) {
			std::string type = "";
			HelperFunctions::DeserializeVariable("type", type, repo_node);

			if (type == "") {
				continue;
			}
			RepositoryHandle repo;

			if (type == HelperFunctions::GetClassName<ExternalRepository>()) {
				repo.HoldType<ExternalRepository>();
			}

			if (!repo) {
				continue;
			}

			repo.Get()->Deserialize(repo_node);

			CMakeGenerator::Settings().repositories.push_back(repo);

		}
	}

	if (mainNode["targets"]) {
		for (auto node : mainNode["targets"]) {

			auto& ptrHolder = CMakeGenerator::Settings().targets.emplace_back();
			TargetGenerator& newTarget = ptrHolder.HoldType<TargetGenerator>();

			HelperFunctions::DeserializeVariable("name", newTarget.name, node);

			HelperFunctions::DeserializeVariable("cpp_standard", newTarget.cppStandard, node);

			HelperFunctions::DeserializeVariable("type", newTarget.type, node);

			HelperFunctions::DeserializeVariable("sources", newTarget.sourceFiles, node);

			if (node["includes"]) {
				for (auto include_node : node["includes"]) {
					IncludeSettings include;
					HelperFunctions::DeserializeVariable("path", include.path, include_node);
					HelperFunctions::DeserializeVariable("access", include.access, include_node);
					newTarget.includes.push_back(include);
				}
			}

			if (node["libraries"]) {
				for (auto lib_node : node["libraries"]) {
					LibrarySettings library;
					HelperFunctions::DeserializeVariable("path", library.path, lib_node);
					HelperFunctions::DeserializeVariable("access", library.access, lib_node);
					HelperFunctions::DeserializeVariable("debug_postfix", library.debugPostfix, lib_node);
					//HelperFunctions::DeserializeVariable("is_alias", library.isTargetName, lib_node);
					newTarget.libraries.push_back(library);
				}
			}

			if (node["external_repositories"]) {
				for (auto repo_node : node["external_repositories"]) {
					if (auto repo = CMakeGenerator::FindAliasInRepositories(repo_node.as<std::string>()); repo) {
						newTarget.externalRepos.push_back(repo);
					}
				}
			}


		}
	}

	return true;
}

YAML::Node CMakeSerializer::SerializeToNode()
{
	YAML::Node mainNode;

	mainNode["cmake_version"] = CMakeGenerator::Settings().cmakeVersion;
	mainNode["project_name"] = CMakeGenerator::Settings().projectName;

	for (auto& subdir : CMakeGenerator::Settings().subdirectories) {
		mainNode["subdirectories"].push_back(subdir.subdir);
	}

	for (auto& repo : CMakeGenerator::Settings().repositories) {
		if (!repo) {
			continue;
		}
		mainNode["repositories"].push_back(repo.Get()->Serialize());
	}

	for (auto& target : CMakeGenerator::Settings().targets) {
		if (!target) {
			continue;
		}
		YAML::Node targetNode;
		targetNode["name"] = target.Get()->name;
		targetNode["cpp_standard"] = target.Get()->cppStandard;
		targetNode["type"] = target.Get()->type;

		if (target.Get()->sourceFiles != "") {
			targetNode["sources"] = target.Get()->sourceFiles;
		}

		for (auto& include : target.Get()->includes) {
			YAML::Node includeNode;
			includeNode["path"] = include.path;
			includeNode["access"] = include.access;
			targetNode["includes"].push_back(includeNode);
		}

		for (auto& library : target.Get()->libraries) {
			YAML::Node libNode;
			libNode["path"] = library.path;
			libNode["access"] = library.access;
			//libNode["is_alias"] = library.isTargetName;
			libNode["debug_postfix"] = library.debugPostfix;
			targetNode["libraries"].push_back(libNode);
		}

		for (auto& repo : target.Get()->externalRepos) {
			if (!repo) {
				continue;
			}
			targetNode["external_repositories"].push_back(repo.Get()->GetAlias());
		}

		mainNode["targets"].push_back(targetNode);

	}

	return mainNode;
}


