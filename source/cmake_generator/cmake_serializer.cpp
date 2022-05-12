#include "cmake_serializer.h"
#include "external_repo.h"
#include <fstream>
#include "cmake_generator.h"

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



	stringToAdd += fmt::format(R"(project("{}")\n)", prop.projectName).c_str();


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


			if (repo.IsHoldingType<ExternalRepository>()) {
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
add_dependencies()" + fmt::format("{} ",target.Get()->name) + fmt::format("{})\n",dep);
			}
		}

		if (target.Get()->libraries.size() > 0 || externalRepoLibraries.size() > 0) {
			stringToAdd += R"(
#adding libraries...

)";

			for (auto& library : target.Get()->libraries) {
				stringToAdd += R"(
target_link_libraries()" + fmt::format("{} ",target.Get()->name) + fmt::format("{} ",library.access);

				if (library.isTargetName) {
					stringToAdd += fmt::format("{})\n",library.path);
				}
				else{
					stringToAdd += "${PROJECT_SOURCE_DIR}/" + fmt::format("{})\n", library.path);
				}

			}

			for (auto& [name,library] : externalRepoLibraries) {
				stringToAdd += R"(
target_link_libraries()" + fmt::format("{} ", target.Get()->name) + fmt::format("{} ", library.access);

				if (library.isTargetName) {
					stringToAdd += fmt::format("{})\n", library.path);
				}
				else {
					stringToAdd += "${PROJECT_SOURCE_DIR}/vendor/" + fmt::format("{}/",name) + fmt::format("{})\n", library.path);
				}
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

bool CMakeSerializer::SerializeToSave(std::string name)
{
	if (m_SavedConfigs[name].IsDefined()) {
		CMakeGenerator::ShowErrorPopup(fmt::format(R"("{}" is already defined as a save!)",name));
		return false;
	}

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
			libNode["is_alias"] = library.isTargetName;
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

	m_SavedConfigs[name] = mainNode;

	return true;

}

bool CMakeSerializer::DeserializeSavedConfig(std::string name)
{
	if (m_SavedConfigs[name].IsNull()) {
		return false;
	}


}

const YAML::Node& CMakeSerializer::GetSavedConfigs()
{
	return m_SavedConfigs;
}

