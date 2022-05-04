#include "cmake_generator.h"

void CMakeGenerator::ShowMainWindow()
{
	//ImGui::Begin("CmakeWindow",0,ImGuiWindowFlags_NoTitleBar);

	ImGui::BeginChild("CmakeWindowNormalSettings",ImVec2(0,ImGui::GetContentRegionAvail().y - 25),true);

	if (ImGui::BeginTable("CmakeWindowTable", 2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableSetupColumn("##firstColumn", 0, 2.0f);

		ImGui::TableNextColumn();

		ImGui::TextWrapped(("Current Directory: " + m_Properties.currentDirectory).c_str());

		ImGui::TableNextColumn();

		if (ImGui::Button("Modify",ImVec2(ImGui::GetContentRegionAvail().x,0))) {

		}

		ImGui::TableNextColumn();

		ImGui::Text("Create Library");

		ImGui::TableNextColumn();
		
		if (ImGui::Checkbox("##LibraryCheckbox", &m_Properties.shouldCreateLibrary)) {

		}

		ImGui::TableNextColumn();

		ImGui::Text("Target Name");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##TargetNameInput",&m_Properties.targetName);

		ImGui::TableNextColumn();

		ImGui::Text("C++ Standard");

		ImGui::TableNextColumn();
		
		static std::vector<const char*> cppStandards = { "C++20","C++17","C++14","C++11","C++03","C++98" };
		

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::BeginCombo("##CppStandardCombo", m_Properties.cppStandard.c_str())) {

			for (auto item : cppStandards) {
				const bool is_selected = m_Properties.cppStandard == item;

				if (ImGui::Selectable(item, is_selected)) {
					m_Properties.cppStandard = item;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}


			}

			ImGui::EndCombo();

		}
		ImGui::TableNextColumn();

		ImGui::Text("Source Files*");

		ImGui::TableNextColumn();

		ImGui::TableNextColumn();

		ImGui::InputTextMultiline("##SourceFilesMultiline", &m_Properties.sourceFiles,ImVec2(ImGui::GetContentRegionAvail().x,0));

		ImGui::TableNextColumn();

		ImGui::TableNextColumn();

		ImGui::Text("Include Directories");

		ImGui::TableNextColumn();
		ImGui::TableNextColumn();

		ImGui::InputTextMultiline("##IncludeDirectoriesMultiline", &m_Properties.includeDirectories,ImVec2(ImGui::GetContentRegionAvail().x,0));

		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
		
		ImGui::Text("External Repositories");

		ImGui::TableNextColumn();


		ImGui::PushStyleColor(ImGuiCol_ChildBg, Color(30, 30, 30).AsImVec4());
		ImGui::BeginChild("ExternalReposChild",ImVec2(ImGui::GetContentRegionAvail().x,100));
		ImGui::PopStyleColor();

		
		static std::string currentHoveredRepo = "";
		for (auto& [name, repo] : m_Properties.repositories) {

			

			if (ImGui::Selectable(name.c_str())) {

			}

			if (ImGui::BeginPopupContextItem(("##" + std::to_string(std::hash<std::string>()(name))).c_str())) {

				ShowRepoCreateMenu();

				if (ImGui::MenuItem("Modify")) {
					repo.get()->ShowPopup();
				}

				if (ImGui::MenuItem("Delete")) {

				}

				ImGui::EndPopup();
			}


		}

		if (ImGui::BeginPopupContextWindow("##ExternalReposChildWindowPopupID", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup)) {
			ShowRepoCreateMenu();
			ImGui::EndPopup();
		}



		ImGui::EndChild();
		



		ImGui::EndTable();
	}

	ImGui::EndChild();

	if (ImGui::Button("Generate")) {

	}


	//ImGui::End();

}

void CMakeGenerator::ShowRepoCreateMenu()
{
	if (ImGui::BeginMenu("Create")) {

		if (ImGui::MenuItem("Git Repo")) {

		}

		if (ImGui::MenuItem("Installed Package")) {

		}


		ImGui::EndMenu();
	}
}
