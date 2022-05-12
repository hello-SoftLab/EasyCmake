#include "include_settings.h"
#include "cmake_generator.h"

void IncludeSettings::ShowWidgets(std::vector<IncludeSettings>& includes)
{
	CMakeGeneratorChangeableWidgetSetup setup(includes);

	setup.mainTextStringFunc = [](IncludeSettings& inc) {return &inc.path; };
	setup.widgetsFunc = [](IncludeSettings& include) {

		if (ImGui::BeginTable(("TableforIncludes##" + HelperFunctions::GenerateStringHash(&include)).c_str(), 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders)) {

			ImGui::TableNextColumn();

			ImGui::Text("Acess");

			ImGui::TableNextColumn();

			std::vector<const char*> items = { "PUBLIC","PRIVATE" };


			ImGui::SetNextItemWidth(ImGui::CalcTextSize("ComboPUBLIC").x);
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

	};


	CMakeGenerator::SetupChangeableWidgets(setup);

}
