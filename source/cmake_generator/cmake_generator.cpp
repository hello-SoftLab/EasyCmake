#include "cmake_generator.h"
#include "external_repo.h"
#include <format>
#include "../../vendor/dialog/include/nfd.hpp"
#include "cmake_serializer.h"

void CMakeGenerator::ShowMainWindow()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("CmakeWindow",0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize );

	ImGui::BeginChild("CmakeWindowNormalSettings",ImVec2(0,ImGui::GetContentRegionAvail().y - 25),true);

	if (ImGui::BeginTable("CmakeWindowTable", 2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableSetupColumn("##firstColumn", 0, 2.0f);

		ImGui::TableNextColumn();

		ImGui::TextWrapped(("Current Directory: " + m_Properties.currentDirectory).c_str());

		ImGui::TableNextColumn();

		if (ImGui::Button("Modify",ImVec2(ImGui::GetContentRegionAvail().x,0))) {
			// initialize NFD
			NFD::Guard nfdGuard;

			// auto-freeing memory
			NFD::UniquePath outPath;

			nfdresult_t result = NFD::PickFolder(outPath,m_Properties.currentDirectory.c_str());

			if (result == NFD_OKAY) {
				m_Properties.currentDirectory = outPath.get();
			}
		}

		ImGui::TableNextColumn();

		ImGui::Text("Project Name");
		
		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##ProjectNameInput", &m_Properties.projectName);

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::Text("Cmake version");

		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##", &m_Properties.cmakeVersion);
		
		ImGui::TableNextColumn();

		ImGui::Text("Subdirectories");

		ImGui::TableNextColumn();

		SubdirectorySettings::ShowWidgets(m_Properties.subdirectories);

		ImGui::TableNextColumn();

		ImGui::Text("External Repositories");

		ImGui::TableNextColumn();


		ImGui::PushStyleColor(ImGuiCol_ChildBg, Color(30, 30, 30).AsImVec4());
		ImGui::BeginChild("ExternalReposChild",ImVec2(ImGui::GetContentRegionAvail().x,100));
		ImGui::PopStyleColor();

		if (ImGui::BeginTable("ExternalReposTableInner",1,ImGuiTableFlags_SizingStretchSame)) {

			//ImGui::TableNextColumn();

			auto it = m_Properties.repositories.begin();
			while (it != m_Properties.repositories.end()) {

				auto& repo = *it;

				if (!repo) {
					it = m_Properties.repositories.erase(it);
					continue;
				}
				ImGui::TableNextColumn();
				
				if (ImGui::Button((repo.Get()->GetAlias() + " (" + repo.GetType() + ")" + repo.Get()->GetHash()).c_str(),ImVec2(ImGui::GetContentRegionAvail().x,0))) {

				}

				if (ImGui::BeginPopupContextItem(("##" + HelperFunctions::GenerateStringHash(repo.Get())).c_str())) {

					ShowRepoCreateMenu();

					if (ImGui::MenuItem("Modify")) {
						repo.Get()->OpenPopup();
					}

					if (ImGui::MenuItem("Delete")) {
						it = m_Properties.repositories.erase(it);
						ImGui::EndPopup();
						continue;
					}

					ImGui::EndPopup();
				}

				it++;
			}

			


			ImGui::EndTable();
		}

		if (ImGui::BeginPopupContextWindow("##ExternalReposChildWindowPopupID", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverExistingPopup)) {
			ShowRepoCreateMenu();
			ImGui::EndPopup();
		}



		ImGui::EndChild();
		
		ImGui::TableNextColumn();



		ImGui::EndTable();
	}

	
	ImGui::Text("Targets:");

	if (ImGui::BeginTabBar("TargetsTabBar", ImGuiTabBarFlags_Reorderable)) {
		int index = 1;
		auto it = m_Properties.targets.begin();
		while (it != m_Properties.targets.end()) {
			auto& target = *it;
			if (target) {
				
				bool open = true;

				if (ImGui::BeginTabItem(("Target " + std::to_string(index) + "##" + HelperFunctions::GenerateStringHash(&target)).c_str(),&open)) {
					
					target.Get()->ShowWidgets();

					ImGui::EndTabItem();
				}
				index++;

				if (!open) {
					it = m_Properties.targets.erase(it);
					continue;
				}
				
			}
			it++;
		}

		if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing)) {
			m_Properties.targets.emplace_back().HoldType<TargetGenerator>().name = "new Target";
		}

		ImGui::EndTabBar();
	}


	ImGui::EndChild();

	if (ImGui::Button("Generate")) {
		if (ValidateInputs()) {
			CMakeSerializer::GenerateCMakeLists(m_Properties);
		}
	}

	

	if (m_Properties.tempRepo) {
		
		ShowPopupForRepo(m_Properties.tempRepo);
		
	}


	for (auto& repo : m_Properties.repositories) {
		if (!repo) {
			continue;
		}
		
		ShowPopupForRepo(repo);
	}

	ValidateRepos();


	if (CMakeGenerator::m_ShouldShowErrorPopup) {
		ImGui::OpenPopup(("Error##" + HelperFunctions::GenerateStringHash(&m_Properties)).c_str());
		m_ShouldShowErrorPopup = false;
	}

	bool isPopupOpen = true;
	if (ImGui::BeginPopupModal(("Error##" + HelperFunctions::GenerateStringHash(&m_Properties)).c_str(), &isPopupOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(m_ErrorPopupMsg.c_str());

		if (ImGui::Button("Ok")) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (!isPopupOpen) {
		ImGui::CloseCurrentPopup();
	}


	
	ImGui::End();


}

const std::vector<RepositoryHandle>& CMakeGenerator::Repositories()
{
	return m_Properties.repositories;
}

CmakeGeneratorProperties& CMakeGenerator::Settings()
{
	return m_Properties;
}





void CMakeGenerator::ShowErrorPopup(std::string errorMsg)
{
	m_ErrorPopupMsg = errorMsg;
	m_ShouldShowErrorPopup = true;
}

bool CMakeGenerator::FindAliasInRepositories(std::string alias)
{
	for (auto& repo : Repositories()) {
		if (!repo) {
			continue;
		}
	}
	return false;
}

void CMakeGenerator::ShowPopupForRepo(RepositoryHandle& repo)
{
	if (!repo) {
		return;
	}

	if (!repo.Get()->IsPopupOpen()) {
		return;
	}

	std::string hash = repo.Get()->GetHash();
	
	
	
	if (ImGui::Begin(("Repository Settings" + hash).c_str(), &repo.Get()->m_ShouldOpenPopup,ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::BeginChild(("ChildWindowForPopupRepo" + hash).c_str(),ImVec2(0,500));

		
		if(ImGui::BeginTable(("InitialTableForRepoPopup" + hash).c_str(),2,ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingStretchSame)) {
			
			ImGui::TableNextColumn();

			ImGui::Text("Name*");

			ImGui::TableNextColumn();

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			ImGui::InputText(hash.c_str(), &repo.Get()->m_Alias);


			ImGui::EndTable();
		}

		repo.Get()->SetupPopupWidgets();

		ImGui::EndChild();

		if(ImGui::BeginTable(("EndTableForRepoPopup" + hash).c_str(), 4,ImGuiTableFlags_SizingStretchSame)) {

			
			ImGui::TableNextColumn();

			ImGui::Text("* -> Required");

			ImGui::TableNextColumn();
			ImGui::TableNextColumn();

			
			
			if (ImGui::Button("Ok",ImVec2(ImGui::GetContentRegionAvail().x,0))) {
				if (repo.Get()->CheckRepoValidity()) {
					repo.Get()->ClosePopup();
					if (repo == m_Properties.tempRepo) {
						RepositoryHandle& newRepo = m_Properties.repositories.emplace_back();
						newRepo = repo;
						m_Properties.tempRepo.ClearCurrentType();
					}
				}
			}

			ImGui::TableNextColumn();

			
			if (ImGui::Button("Cancel",ImVec2(ImGui::GetContentRegionAvail().x,0))) {
				repo.Get()->ClosePopup();
				if (repo == m_Properties.tempRepo) {
					m_Properties.tempRepo.ClearCurrentType();
				}

			}

			ImGui::TableNextColumn();

			

			ImGui::EndTable();
		}

	}

	
	ImGui::End();
}

void CMakeGenerator::ValidateRepos()
{
	auto it = m_Properties.repositories.begin();

	while (it != m_Properties.repositories.end()) {
		if (!(*it)) {
			it = m_Properties.repositories.erase(it);
			continue;
		}
		it++;
	}

}

void CMakeGenerator::ShowRepoCreateMenu()
{
	if (ImGui::BeginMenu("Create")) {

		if (ImGui::MenuItem("Git Repo")) {
			m_Properties.tempRepo.HoldType<ExternalRepository>();
			
			m_Properties.tempRepo.Get()->OpenPopup();
			
		}

		if (ImGui::MenuItem("Installed Package")) {

		}


		ImGui::EndMenu();
	}
}

bool CMakeGenerator::ValidateInputs()
{
	if (m_Properties.projectName == "") {
		CMakeGenerator::ShowErrorPopup("Please provide a project name!");
		return false;
	}

	if (m_Properties.cmakeVersion == "") {
		CMakeGenerator::ShowErrorPopup("Please provide a cmake version!");
		return false;
	}


	return true;
}





void TargetGenerator::ShowWidgets()
{
	if (ImGui::BeginTable(("TargetTableFor" + HelperFunctions::GenerateStringHash(this)).c_str(), 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {
		
		ImGui::TableNextColumn();
		
		ImGui::Text("Target name");

		ImGui::TableNextColumn();


		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##",&this->name);


		ImGui::TableNextColumn();

		ImGui::Text("Target Type");

		ImGui::TableNextColumn();


		std::vector<const char*> values = {"Executable","Library"};
		

		if (ImGui::BeginCombo(("##Combo" + HelperFunctions::GenerateStringHash(this)).c_str(), this->type.c_str())) {

			for (auto& item : values) {

				const bool is_selected = this->type == item;

				if (ImGui::Selectable(item, is_selected)) {
					this->type = item;
				}

				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}

			}


			ImGui::EndCombo();
		}

		ImGui::TableNextColumn();

		ImGui::Text("C++ Standard");

		ImGui::TableNextColumn();

		static std::vector<const char*> cppStandards = { "C++20","C++17","C++14","C++11","C++03","C++98" };


		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::BeginCombo("##CppStandardCombo", cppStandard.c_str())) {

			for (auto item : cppStandards) {
				const bool is_selected = cppStandard == item;

				if (ImGui::Selectable(item, is_selected)) {
					cppStandard = item;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus();
				}


			}

			ImGui::EndCombo();

		}
		ImGui::TableNextColumn();

		ImGui::Text("Source Files");

		ImGui::TableNextColumn();

		ImGui::InputTextMultiline("##SourceFilesMultiline", &sourceFiles, ImVec2(ImGui::GetContentRegionAvail().x, 0));

		ImGui::TableNextColumn();
		
		ImGui::Text("Includes");

		ImGui::TableNextColumn();

		IncludeSettings::ShowWidgets(includes);

		ImGui::TableNextColumn();

		ImGui::Text("Libraries");

		ImGui::TableNextColumn();

		LibrarySettings::ShowWidgets(this->libraries);

		if (CMakeGenerator::Repositories().size() > 0) {

			ImGui::TableNextColumn();

			ImGui::Text("External Repositories");

			ImGui::TableNextColumn();
			
			if (ImGui::BeginTable(("TableForRepos##" + HelperFunctions::GenerateStringHash(this)).c_str(), 1, ImGuiTableFlags_SizingStretchProp )) {


				for (auto& repo : CMakeGenerator::Repositories()) {
					ImGui::TableNextColumn();

					if (!repo) {
						continue;
					}
					auto repoIt = std::find(this->externalRepos.begin(), this->externalRepos.end(), repo);
					bool check = (repoIt != this->externalRepos.end());
					if (ImGui::Checkbox(("##RepoCheckbox" + HelperFunctions::GenerateStringHash(this)).c_str(), &check)) {
						if (!check) {
							externalRepos.erase(repoIt);
						}
						else {
							externalRepos.emplace_back().WatchPointer(repo);
						}
					}

					ImGui::SameLine();

					ImGui::Text((repo.Get()->GetAlias() + " ( " 
					+ std::to_string(repo.Get()->GetNumberOf("sources")) + " source file" + (repo.Get()->GetNumberOf("sources") == 1? "": "s") + ", "
					+ std::to_string(repo.Get()->GetNumberOf("includes")) + " include" + (repo.Get()->GetNumberOf("includes") == 1 ? "" : "s") + ", "
					+ std::to_string(repo.Get()->GetNumberOf("libraries")) + " librar" + (repo.Get()->GetNumberOf("libraries") == 1 ? "y" : "ies")  + " )"
					).c_str());

				}

				ImGui::EndTable();
			}

			
		}

		ImGui::EndTable();
	}
}
