#pragma once
#include "../global.h"
#include "repository.h"
#include "include_settings.h"


template<typename T>
struct CMakeGeneratorChangeableWidgetSetup {

	CMakeGeneratorChangeableWidgetSetup(std::vector<T>& vector) { vec = &vector; };

	std::vector<T>* vec;
	std::function<std::string* (T&)> mainTextStringFunc;
	std::function<void(T&)> widgetsFunc;
};

class CMakeGenerator {
public:



	template<typename T>
	static void SetupChangeableWidgets(CMakeGeneratorChangeableWidgetSetup<T>& setup) {
		auto it = setup.vec->begin();
		while (it != setup. vec->end()) {
			auto& item = *it;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("AAAa").x);
			ImGui::InputText(("##mainChoice" + HelperFunctions::GenerateStringHash(&item)).c_str(), setup.mainTextStringFunc(item));

			if (setup.widgetsFunc) {
				if (ImGui::BeginPopupContextItem()) {

					setup.widgetsFunc(item);

					ImGui::EndPopup();
				}
			}

			ImGui::SameLine();

			if (ImGui::Button(("X##" + HelperFunctions::GenerateStringHash(&item)).c_str())) {
				it = setup.vec->erase(it);
				continue;
			}

			it++;


		}


		if (ImGui::Button(("+##" + HelperFunctions::GenerateStringHash(setup.vec)).c_str())) {
			setup.vec->emplace_back();
		}
	}

	static void ShowErrorPopup(std::string errorMsg);

	static bool FindAliasInRepositories(std::string alias);
	static const std::vector<RepositoryHandle>& Repositories();
	static CmakeGeneratorProperties& Settings();
	static void ShowMainWindow();

private:
	static void ShowPopupForRepo(RepositoryHandle& handle);
	static void ValidateRepos();
	static void ShowRepoCreateMenu();
	
	static bool ValidateInputs();
	
	static inline bool m_ShouldShowErrorPopup = false;
	static inline std::string m_ErrorPopupMsg = "";
	static inline CmakeGeneratorProperties m_Properties;
	


};

