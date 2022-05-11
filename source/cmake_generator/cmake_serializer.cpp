#include "cmake_serializer.h"

bool CMakeSerializer::GenerateCMakeLists(const CmakeGeneratorProperties& prop)
{
	std::string stringToAdd = "";

	stringToAdd += R"(

#this CMakeLists was created with EasyCmake - V2 
#the repository can be found at https://github.com/knz13/EasyCmake_Cpp

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

set(${PROJECT_NAME}_DEPENDENCIES_TO_BUILD)


#project name
)";

	stringToAdd += fmt::format(R"(project("{}"))", prop.projectName).c_str();

	for (auto& repo : prop.repositories) {
		if (!repo) {
			continue;
		}
		stringToAdd += repo.Get()->GetCMakeListsString();
	}





	return true;
}
