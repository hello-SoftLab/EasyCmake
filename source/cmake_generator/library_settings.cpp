#include "library_settings.h"
#include "cmake_generator.h"

void LibrarySettings::ShowWidgets(std::vector<LibrarySettings>& libraries)
{
	CMakeGeneratorChangeableWidgetSetup setup(libraries);

	setup.mainTextStringFunc = [](LibrarySettings& lib) {return &lib.path; };

	setup.widgetsFunc = [](LibrarySettings& library) {

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
			ImGui::InputText(("##dpostfixForLibrary" + HelperFunctions::GenerateStringHash(&library)).c_str(), &library.debugPostfix);

			ImGui::EndTable();

		}

	};

	CMakeGenerator::SetupChangeableWidgets(setup);


}
