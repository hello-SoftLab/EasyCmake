#include "include_settings.h"

void IncludeSettings::ShowWidgets(std::vector<IncludeSettings>& includes)
{
	auto it = includes.begin();
	while (it != includes.end()) {
		auto& include = *it;
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("AAAa").x);
		ImGui::InputText(("##pathForinclude" + HelperFunctions::GenerateStringHash(&include)).c_str(), &include.path);

		if (ImGui::BeginPopupContextItem()) {

			if (ImGui::BeginTable(("TableforIncludes##" + HelperFunctions::GenerateStringHash(&include)).c_str(), 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders)) {

				ImGui::TableNextColumn();

				ImGui::Text("Acess");

				ImGui::TableNextColumn();

				std::vector<const char*> items = { "PUBLIC","PRIVATE" };


				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(("##comboForAccess" + HelperFunctions::GenerateStringHash(&include)).c_str(), include.access.c_str())) {

					for (auto& item : items) {

						const bool is_selected = item == include.access;

						if (ImGui::Selectable(item, is_selected)) {
							include.access = item;
						}
						if (is_selected) {
							ImGui::SetItemDefaultFocus();
						}


					}

					ImGui::EndCombo();
				}

				ImGui::EndTable();

			}

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button(("X##" + HelperFunctions::GenerateStringHash(&include)).c_str())) {
			it = includes.erase(it);
			continue;
		}

		it++;


	}


	if (ImGui::Button(("+##" + HelperFunctions::GenerateStringHash(&includes)).c_str())) {
		includes.emplace_back();
	}
}
