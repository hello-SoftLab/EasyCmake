#include "cmake_generator.h"
#include "external_repo.h"
#include <format>

void CMakeGenerator::ShowMainWindow()
{
	ImGui::ShowDemoWindow();

	ImGui::Begin("CmakeWindow",0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::BeginChild("CmakeWindowNormalSettings",ImVec2(0,ImGui::GetContentRegionAvail().y - 25),true);

	if (ImGui::BeginTable("CmakeWindowTable", 2,ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableSetupColumn("##firstColumn", 0, 2.0f);

		ImGui::TableNextColumn();

		ImGui::TextWrapped(("Current Directory: " + m_Properties.currentDirectory).c_str());

		ImGui::TableNextColumn();

		if (ImGui::Button("Modify",ImVec2(ImGui::GetContentRegionAvail().x,0))) {

		}

		ImGui::TableNextColumn();

		ImGui::Text("Project Name");
		
		ImGui::TableNextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputText("##ProjectNameInput", &m_Properties.projectName);

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
			GenerateCMakeLists();
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

	//ImGui::ShowDemoWindow();


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
	
	bool shouldOpenErrorPopup = false;
	static std::string errorMsg = "";

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
				if (!repo.Get()->CheckRepoValidity(errorMsg)) {
					shouldOpenErrorPopup = true;
				}
				else {
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

	if (shouldOpenErrorPopup) {
		ImGui::OpenPopup(("Error##" + HelperFunctions::GenerateStringHash(&repo)).c_str());
		shouldOpenErrorPopup = false;
	}
	
	bool isPopupOpen = true;
	if (ImGui::BeginPopupModal(("Error##" + HelperFunctions::GenerateStringHash(&repo)).c_str(),&isPopupOpen,ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(errorMsg.c_str());

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

void CMakeGenerator::GenerateCMakeLists()
{
	std::string textFile = "";
	
	AddInitialDetails(textFile);
	/*
	if (m_Properties.sourceFiles != "") {
		AddSourceFiles(textFile);

		AddExecutableAndSetDetails(textFile);

		AddIncludeDirectories(textFile);
	}
	*/

	


	std::cout << textFile << std::endl;
	
}

bool CMakeGenerator::ValidateInputs()
{
	if (m_Properties.projectName == "") {
		return false;
	}
	return true;
}

void CMakeGenerator::AddSourceFiles(std::string& stringToAdd)
{
	stringToAdd += R"(

#adding general source files
)";
	/*
	for (auto& filePath : HelperFunctions::SplitString(m_Properties.sourceFiles, "\n")) {
		stringToAdd += R"(
list(APPEND ${PROJECT_NAME}_SOURCE_FILES ${PROJECT_SOURCE_DIR}/)";
		stringToAdd += fmt::format("{}\n", filePath);

	}  ADD LATER
	*/
}

void CMakeGenerator::AddIncludeDirectories(std::string& stringToAdd)
{

	

}

void CMakeGenerator::AddInitialDetails(std::string& stringToAdd)
{
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


#project name
)";

	stringToAdd += fmt::format(R"(project("{}"))", m_Properties.projectName).c_str();

	stringToAdd += R"(
#creating variables for ease of adding libraries
set(${PROJECT_NAME}_SOURCE_FILES)
)";

	if (m_Properties.repositories.size() > 0) {
		stringToAdd += "set(${PROJECT_NAME}_DEPS_TO_BUILD)\n";
		stringToAdd += "set(${ PROJECT_NAME }_LIBRARIES)\n";
		stringToAdd += "set(${PROJECT_NAME}_INCLUDES)\n\n\n";
	}


}

void CMakeGenerator::AddExecutableAndSetDetails(std::string& stringToAdd)
{
	
	

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

		ImGui::InputTextMultiline("##includesFilesMultiline", &includes, ImVec2(ImGui::GetContentRegionAvail().x, 0));

		ImGui::TableNextColumn();

		ImGui::Text("Libraries");

		ImGui::TableNextColumn();

		ImGui::InputTextMultiline("##LibraryFilesMultiline", &libraries, ImVec2(ImGui::GetContentRegionAvail().x, 0));

		ImGui::TableNextColumn();

		ImGui::Text("External Repositories");

		ImGui::TableNextColumn();

		if (CMakeGenerator::Repositories().size() > 0) {

			
			if (ImGui::BeginTable(("TableForRepos##" + HelperFunctions::GenerateStringHash(this)).c_str(), 1, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersOuter)) {


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
