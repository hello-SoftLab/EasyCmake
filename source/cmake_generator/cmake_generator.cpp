#include "cmake_generator.h"
#include "external_repo.h"
#include <format>
#include "../../vendor/dialog/include/nfd.hpp"
#include "cmake_serializer.h"
#include "installed_package.h"

void CMakeGenerator::ShowMainWindow()
{
	//ImGui::ShowDemoWindow();

	ImGui::Begin("CmakeWindow",0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoBringToFrontOnFocus );

	ImGui::BeginChild("CmakeWindowNormalSettings",ImVec2(0,ImGui::GetContentRegionAvail().y - 25),true);

	if (ImGui::BeginTable("CmakeWindowTable", 2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableSetupColumn("##firstColumn", 0, 2.0f);

		ImGui::TableNextColumn();

		ImGui::TextWrapped(("Target Directory: " + m_Properties.currentDirectory).c_str());

		ImGui::TableNextColumn();

		if (ImGui::Button("Modify",ImVec2(ImGui::GetContentRegionAvail().x,0))) {
			// initialize NFD
			NFD::Guard nfdGuard;

			// auto-freeing memory
			NFD::UniquePath outPath;

			nfdresult_t result = NFD::PickFolder(outPath,m_Properties.currentDirectory.c_str());

			if (result == NFD_OKAY) {
				/*
				CustomPopupProperties prop;
				prop.title = "Warning";
				prop.initialSize = ImVec2(300,100);
				std::string path = outPath.get();
				CMakeGenerator::ShowCustomPopup(prop, [=]() {
					
					ImGui::TextWrapped("Do you wish to transfer your current work to the new directory?");
					
					if (ImGui::Button("Yes")) {
						CMakeSerializer::SaveCurrentToCache();
						//CMakeGenerator::ClearCurrentSettings();
						m_Properties.currentDirectory = path;
						CMakeSerializer::LoadCurrentFromCache();
						CMakeGenerator::CloseCustomPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("No")) {
						CMakeSerializer::SaveCurrentToCache();
						CMakeGenerator::ClearCurrentSettings();
						m_Properties.currentDirectory = path;
						CMakeSerializer::LoadCurrentFromCache();
						CMakeGenerator::CloseCustomPopup();
					}
				
				});
				*/



				CMakeSerializer::SaveCurrentToCache();
				CMakeGenerator::ClearCurrentSettings();
				m_Properties.currentDirectory = outPath.get();
				
				if (CMakeSerializer::HasDirectoryBeenUsedBefore()) {
					CustomPopupProperties prop;
					prop.title = "Warning";
					prop.initialSize = ImVec2(300, 200);
					prop.widgetFunc = [=]() {

						ImGui::TextWrapped("A CMakeLists.txt was already created on this directory with EasyCmake. Do you wish to load the last used configurations?");


						ImGui::SetCursorPos(ImVec2(5,ImGui::GetWindowSize().y - (ImGui::CalcTextSize("A").y * 2 - 1)));
						if (ImGui::Button("Yes") || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
							CMakeSerializer::LoadCurrentFromSavedDirectories();
							CMakeGenerator::CloseCustomPopup();
						}

						

						ImGui::SameLine();
						if (ImGui::Button("No") || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
							CMakeSerializer::LoadCurrentFromCache();
							CMakeGenerator::CloseCustomPopup();
						}

					};
					CMakeGenerator::ShowCustomPopup(prop);
				}
				
				//CMakeSerializer::LoadCurrentFromCache();
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

					if (!ShowRepoCreateMenu()) {
						it = m_Properties.repositories.end();
						ImGui::EndPopup();
						continue;
					}

					if (ImGui::MenuItem("Modify")) {
						//m_Properties.tempRepo = repo;
						//m_Properties.tempRepo.Get()->OpenPopup();
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
			CMakeSerializer::SaveCurrentToSavedDirectories();
			CustomPopupProperties prop;
			prop.title = "Success!";
			prop.widgetFunc = []() {

				ImGui::Text("Successfully written to CMakeLists.txt!");

				if (ImGui::Button("Ok") || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_Escape)) {
					ImGui::CloseCurrentPopup();
				}


			};
			CMakeGenerator::ShowModalPopup(prop);
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

	if (m_CustomModalPopupProperties.shouldShow) {
		ImGui::OpenPopup((m_CustomModalPopupProperties.title + "##" + HelperFunctions::GenerateStringHash(&m_Properties)).c_str());
		m_CustomModalPopupProperties.shouldShow = false;
	}

	bool isModalOpen = true;
	if (ImGui::BeginPopupModal((m_CustomModalPopupProperties.title + "##" + HelperFunctions::GenerateStringHash(&m_Properties)).c_str(), &isModalOpen)) {

		m_CustomModalPopupProperties.widgetFunc();

		ImGui::EndPopup();
	}


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

	
	if (m_CustomPopupProperties.shouldShow) {
		bool shouldOpen = true;
		
		ImGui::SetNextWindowSize(m_CustomPopupProperties.initialSize,ImGuiCond_Once);
		if (ImGui::Begin((m_CustomPopupProperties.title + "##Custom" + HelperFunctions::GenerateStringHash(&m_Properties)).c_str(), &shouldOpen, m_CustomPopupProperties.flags )) {



			m_CustomPopupProperties.widgetFunc();


		}
		ImGui::End();

		if (!shouldOpen) {
			m_CustomPopupProperties.shouldShow = false;
		}
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





void CMakeGenerator::ShowModalPopup(CustomPopupProperties prop)
{
	m_CustomModalPopupProperties = prop;
	m_CustomModalPopupProperties.shouldShow = true;
}

void CMakeGenerator::ShowErrorPopup(std::string errorMsg)
{
	m_ErrorPopupMsg = errorMsg;
	m_ShouldShowErrorPopup = true;
}



void CMakeGenerator::ShowCustomPopup(CustomPopupProperties prop)
{
	
	m_CustomPopupProperties = prop;
	m_CustomPopupProperties.shouldShow = true;
}



void CMakeGenerator::CloseCustomPopup()
{
	m_CustomPopupProperties.shouldShow = false;
}

void CMakeGenerator::ClearCurrentSettings()
{
	std::string currentDir = m_Properties.currentDirectory;
	m_Properties = CmakeGeneratorProperties();
	m_Properties.currentDirectory = currentDir;
}

RepositoryHandle CMakeGenerator::FindAliasInRepositories(std::string alias)
{
	for (auto& repo : Repositories()) {
		if (!repo) {
			continue;
		}

		if (repo.Get()->m_Alias == alias) {
			auto repository = RepositoryHandle();
			repository.WatchPointer(repo);
			return repository;
		}

	}
	return {};
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

			ImGui::Text("Alias*");

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
						CMakeSerializer::AddRepoToRecent(newRepo);
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

bool CMakeGenerator::ShowRepoCreateMenu()
{

	bool returnVal = true;

	if (ImGui::BeginMenu("Create")) {

		if (ImGui::MenuItem("Git Repo")) {
			m_Properties.tempRepo.HoldType<ExternalRepository>();
			m_Properties.tempRepo.Get()->OpenPopup();
		}

		if (ImGui::MenuItem("Installed Package")) {
			m_Properties.tempRepo.HoldType<InstalledPackage>();
			m_Properties.tempRepo.Get()->OpenPopup();
		}

		if (CMakeSerializer::GetRecentRepositories().size() != 0) {
			if (ImGui::BeginMenu("Recent")) {
				for (auto repo : CMakeSerializer::GetRecentRepositories()) {

					if (ImGui::MenuItem(repo.first.as<std::string>().c_str())) {
						RepositoryHandle& handle = m_Properties.repositories.emplace_back();
						if (!handle.LoadFromSave(repo.second)) {
							m_Properties.repositories.pop_back();
						}
						returnVal = false;

					}
					if (ImGui::IsItemHovered()) {
						std::string tooltipData;

						tooltipData += Repository::GetStringRepresentation(repo.second);

						ImGui::SetTooltip(tooltipData.c_str());
					}

				}
				ImGui::EndMenu();
			}

		}

		ImGui::EndMenu();
	}

	return returnVal;
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

		ImGui::Dummy(ImVec2(0,65));

		if (ImGui::Button("Add Files")) {
			NFD::Guard nfdGuard;

			// auto-freeing memory
			NFD::UniquePathSet outPath;

			nfdfilteritem_t filterItem[2] = { {"Source code", "c,cpp,cc"}};


			nfdresult_t result = NFD::OpenDialogMultiple(outPath,filterItem,1, CMakeGenerator::Settings().currentDirectory.c_str());

			if (result == NFD_OKAY) {
				nfdpathsetsize_t numPaths;
				NFD::PathSet::Count(outPath, numPaths);

				nfdpathsetsize_t i;
				for (i = 0; i < numPaths; ++i) {
					NFD::UniquePathSetPath path;
					NFD::PathSet::GetPath(outPath, i, path);
					std::string correctPath = path.get();
					std::filesystem::path relativePath = std::filesystem::relative(correctPath, { CMakeGenerator::Settings().currentDirectory });
					correctPath = relativePath.string();
					std::replace(correctPath.begin(), correctPath.end(), '\\', '/');
					HelperFunctions::StringReplace(correctPath,R"(\)","/");
					sourceFiles += correctPath;
					sourceFiles += "\n";
				}

			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Add All From Directory")) {
			NFD::Guard nfdGuard;

			// auto-freeing memory
			NFD::UniquePath outPath;

			nfdresult_t result = NFD::PickFolder(outPath, CMakeGenerator::Settings().currentDirectory.c_str());

			if (result == NFD_OKAY) {
				for (auto file : std::filesystem::recursive_directory_iterator(outPath.get())) {
					if (file.path().extension() == ".c" || file.path().extension() == ".cpp" || file.path().extension() == ".cc") {
						std::filesystem::path path = std::filesystem::relative(file.path(), {CMakeGenerator::Settings().currentDirectory});
						std::string correctPath = path.string();
						std::replace(correctPath.begin(), correctPath.end(), '\\', '/');
						sourceFiles += correctPath;
						sourceFiles += "\n";
					}
				}
			}

		}

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
					if (ImGui::Checkbox(("##RepoCheckbox" + HelperFunctions::GenerateStringHash((void*) & repo)).c_str(), &check)) {
						if (!check) {
							externalRepos.erase(repoIt);
						}
						else {
							externalRepos.emplace_back().WatchPointer(repo);
						}
					}

					ImGui::SameLine();

					

					ImGui::Text((repo.Get()->GetAlias() + " ( " 
					+ std::to_string(repo.Get()->GetSources().size()) + " source file" + (repo.Get()->GetSources().size() == 1 ? "" : "s") + ", "
					+ std::to_string(repo.Get()->GetIncludes().size()) + " include" + (repo.Get()->GetIncludes().size() == 1 ? "" : "s") + ", "
					+ std::to_string(repo.Get()->GetIncludes().size()) + " librar" + (repo.Get()->GetIncludes().size() == 1 ? "y" : "ies") + " )"
					).c_str());

				}

				ImGui::EndTable();
			}

			
		}

		ImGui::EndTable();
	}
}
