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

ExternalRepository::ExternalRepository() : Repository(HelperFunctions::GetClassName<ExternalRepository>())
{

}

bool ExternalRepository::OnDeserialize(YAML::Node& node)
{
	HelperFunctions::DeserializeVariable("location",m_RepoLocation,node);
	HelperFunctions::DeserializeVariable("git_tag",m_GitTag,node);
	HelperFunctions::DeserializeVariable("cmake_args",m_CmakeArgs,node);
	return true;
}

YAML::Node ExternalRepository::OnSerialize()
{
	YAML::Node node;
	node["location"] = m_RepoLocation;
	node["git_tag"] = m_GitTag;
	node["cmake_args"] = m_CmakeArgs;
	return node;
}

std::string ExternalRepository::GetCMakeArgs()
{
	std::string returnStr = "";
	returnStr += R"(
			CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}",m_Alias) + R"(
					   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
)";
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
#repository download and settings for alias )" + fmt::format("{}...\n\n",this->m_Alias);

	if (m_CmakeArgs != "") {
		stringToAdd += R"(
	#adding options
)";
	}

	for (auto& definition : HelperFunctions::SplitString(m_CmakeArgs, "\n")) {
		auto vec = HelperFunctions::SplitString(definition, "=");
		if (vec.size() < 2) {
			continue;
		}
		stringToAdd += fmt::format(R"(
	set({} {} CACHE INTERNAL "")
)", vec[0], vec[1]);
	}

	stringToAdd += R"(
	FetchContent_Declare()" + fmt::format("{}_project\n", m_Alias) + R"(
		GIT_REPOSITORY )" + fmt::format("{}\n", m_RepoLocation) + R"(
		GIT_TAG )" + fmt::format("{}\n", m_GitTag) + R"(
		SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/)" + fmt::format("{}\n", m_Alias) + R"(
	)

	list(APPEND ${PROJECT_NAME}_DEPENDENCIES )" + m_Alias + R"(_project)

)";

	
	return stringToAdd;
}

bool ExternalRepository::IsRepoReady()
{
	if (m_RepoLocation == "") {
		CMakeGenerator::ShowErrorPopup("Please provide repository github location!");
		return false;
	}

	if (CMakeGenerator::Settings().tempRepo) {
		
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
