#include "external_repo.h"
#include "cmake_generator.h"

void ExternalRepository::SetupPopupWidgets()
{
	if (ImGui::BeginTable(GetHash().c_str(), 2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableNextColumn();

		ImGui::Text("Git Repo*");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText(("##repoLoc" + GetHash()).c_str(),&m_RepoLocation);

		ImGui::TableNextColumn();

		ImGui::Text("Git Tag");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText(("##tag" + GetHash()).c_str(),&m_GitTag);

		ImGui::TableNextColumn();

		ImGui::Text("Should Build");

		ImGui::TableNextColumn();

		ImGui::Checkbox(("##shouldbuild" + GetHash()).c_str(), &m_ShouldBuild);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Source Files To Add");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextMultiline(("##sources" + GetHash()).c_str(),&m_SourcesToAdd);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Include Paths");

		ImGui::TableNextColumn();

		IncludeSettings::ShowWidgets(m_Includes);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Libraries");

		ImGui::TableNextColumn();

		LibrarySettings::ShowWidgets(this->m_Libraries);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Cmake Args");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextMultiline(("##cmakeArgs" + GetHash()).c_str(), &m_CmakeArgs);

		ImGui::EndTable();

	}
	
}

std::string ExternalRepository::GetCMakeArgs()
{
	std::string returnStr = "";
	returnStr += R"(
		CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}",m_Alias) + R"(
				   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}\n)";
	for (auto& arg : HelperFunctions::SplitString(m_CmakeArgs, "\n")) {
		returnStr += R"(
				   -D)" + fmt::format("{}\n",arg);
	}

	return returnStr;
}

size_t ExternalRepository::GetNumberOf(std::string name)
{
	if (name == "sources") {
		return HelperFunctions::SplitString(m_SourcesToAdd, "\n").size();
	}
	if (name == "includes") {
		return m_Includes.size();
	}
	if (name == "libraries") {
		return m_Libraries.size();
	}

	return 0;
}

std::string ExternalRepository::GetCMakeListsString()
{
	std::string stringToAdd = "";

	stringToAdd += R"(
#repository download and settings for alias)" + fmt::format("{}...\n\n",this->m_Alias);

	stringToAdd += R"(
	dir_exists()" + fmt::format("{}_exists ", this->m_Alias) + "${PROJECT_SOURCE_DIR}/vendor/" + fmt::format("{})\n\n", m_Alias);

	stringToAdd += R"(
	if(NOT ${)" + fmt::format("{}_exists})\n",m_Alias);

	if (m_ShouldBuild) {
		stringToAdd += R"(
		ExternalProject_Add()" + fmt::format("{}\n", m_Alias) + R"(
			GIT_REPOSITORY )" + fmt::format("{}\n", m_RepoLocation) + R"(
			GIT_TAG )" + fmt::format("{}\n", m_GitTag) + GetCMakeArgs();
		if (m_Libraries.size() > 0) {
			for (auto& lib : m_Libraries) {
				stringToAdd += R"(
			BUILD_BYPRODUCTS ${PROJECT_SOURCE_DIR}/vendor)" + fmt::format("{}/", m_Alias) + "lib/${CMAKE_STATIC_LIBRARY_PREFIX}" +
					fmt::format("{}$<$<CONFIG:Debug>:{}",lib.path,lib.debugPostfix) + "${CMAKE_STATIC_LIBRARY_SUFFIX}\n";
			}
		}
		stringToAdd += R"(
		)
		
		list(APPEND ${PROJECT_NAME}_DEPENDENCIES_TO_BUILD )" + fmt::format("{})",m_Alias) + R"(

	endif()

)";
	}
	else {
		stringToAdd += R"(
		FetchContent_Declare()" + fmt::format("{}\n", m_Alias) + R"(
			GIT_REPOSITORY )" + fmt::format("{}\n", m_RepoLocation) + R"(
			GIT_TAG )" + fmt::format("{}\n",m_GitTag) + R"(
			SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}\n",m_Alias) + R"(
		)
		FetchContent_MakeAvailable()" + fmt::format("{})",m_Alias) + R"(
	endif()

)";
	}

	return stringToAdd;
}

bool ExternalRepository::IsRepoReady(std::string& errorMsg)
{
	if (m_RepoLocation == "") {
		errorMsg = "Please provide repository github location!";
		return false;
	}

	if (CheckIfLocationAlreadyRegistered()) {
		errorMsg = "Repository location '" + this->m_RepoLocation + "' was already registered!\nPlease choose another location.";
		return false;
	}
	return true;
}

bool ExternalRepository::CheckIfLocationAlreadyRegistered()
{
	for (auto& repo : CMakeGenerator::Repositories()) {
		if (!repo) {
			continue;
		}
		if (!repo.IsHoldingType<ExternalRepository>()) {
			continue;
		}

		if (repo.GetAs<ExternalRepository>()->m_RepoLocation == this->m_RepoLocation) {
			return true;
		}
		
	}
	return false;
}
