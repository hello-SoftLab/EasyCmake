#include "cmake_generator.h"
#include "external_repo.h"

void CMakeGenerator::ShowMainWindow()
{
	ImGui::Begin("CmakeWindow",0,ImGuiWindowFlags_NoTitleBar);

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
		



		ImGui::EndTable();
	}

	ImGui::EndChild();

	if (ImGui::Button("Generate")) {

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

	ImGui::End();

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
				repo.Get()->ClosePopup();
				if (repo == m_Properties.tempRepo) {
					RepositoryHandle& newRepo = m_Properties.repositories.emplace_back();
					newRepo = repo;
					m_Properties.tempRepo.ClearCurrentType();
					
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
