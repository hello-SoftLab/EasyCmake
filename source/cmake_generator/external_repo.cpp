#include "external_repo.h"

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

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextMultiline(("##includes" + GetHash()).c_str(), &m_IncludePaths);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Libraries");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextMultiline(("##libraries" + GetHash()).c_str(), &m_Libraries);

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
		return HelperFunctions::SplitString(m_IncludePaths, "\n").size();
	}
	if (name == "libraries") {
		return HelperFunctions::SplitString(m_Libraries, "\n").size();
	}

	return 0;
}
