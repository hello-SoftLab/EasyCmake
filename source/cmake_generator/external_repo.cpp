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
