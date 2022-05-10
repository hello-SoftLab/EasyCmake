#include "library_settings.h"

void LibrarySettings::ShowWidgets(std::vector<LibrarySettings>& libraries)
{
	auto it = libraries.begin();
	while (it != libraries.end()) {
		auto& library = *it;
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("AAAa").x);
		ImGui::InputText(("##pathForLibrary" + HelperFunctions::GenerateStringHash(&library)).c_str(), &library.path);

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::BeginTable(("TableforLibrary##" + HelperFunctions::GenerateStringHash(&library)).c_str(), 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders)) {

				ImGui::TableNextColumn();

				ImGui::Text("Acess");

				ImGui::TableNextColumn();

				std::vector<const char*> items = { "PUBLIC","PRIVATE" };


				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(("##comboForAcess" + HelperFunctions::GenerateStringHash(&library)).c_str(), library.access.c_str())) {

					for (auto& item : items) {

						const bool is_selected = item == library.access;

						if (ImGui::Selectable(item, is_selected)) {
							library.access = item;
						}
						if (is_selected) {
							ImGui::SetItemDefaultFocus();
						}


					}

					ImGui::EndCombo();
				}

				ImGui::TableNextColumn();

				ImGui::Text("Debug Postfix");

				ImGui::TableNextColumn();

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::InputText(("##dpostfixForLibrary" + HelperFunctions::GenerateStringHash(&library)).c_str(), &library.path);

				ImGui::TableNextColumn();

				
				ImGui::EndTable();
				
			}

			ImGui::EndPopup();
		}
		
		ImGui::SameLine();

		if (ImGui::Button(("X##" + HelperFunctions::GenerateStringHash(&library)).c_str())) {
			it = libraries.erase(it);
			continue;
		}
		
		it++;


	}
	

	if (ImGui::Button(("+##" + HelperFunctions::GenerateStringHash(&libraries)).c_str())) {
		libraries.emplace_back();
	}

}
