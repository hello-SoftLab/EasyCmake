#include "library_settings.h"
#include "cmake_generator.h"

YAML::Node LibrarySettings::Serialize()
{
	YAML::Node node;
	HelperFunctions::SerializeVariable("path", path, node);
	HelperFunctions::SerializeVariable("debug_postfix", debugPostfix, node);
	HelperFunctions::SerializeVariable("is_variable", isVariableName, node);
	HelperFunctions::SerializeVariable("access", access, node);
	HelperFunctions::SerializeVariable("shared",shared, node);

	return node;
}

bool LibrarySettings::Deserialize(YAML::Node& node)
{
	HelperFunctions::DeserializeVariable("path", path, node);
	HelperFunctions::DeserializeVariable("debug_postfix", debugPostfix, node);
	HelperFunctions::DeserializeVariable("is_variable", isVariableName, node);
	HelperFunctions::DeserializeVariable("access", access, node);
	HelperFunctions::DeserializeVariable("shared", shared, node);

	return true;
}

void LibrarySettings::ShowWidgets(std::vector<LibrarySettings>& libraries)
{
	CMakeGeneratorChangeableWidgetSetup setup(libraries);

	setup.mainTextStringFunc = [](LibrarySettings& lib) {return &lib.path; };

	setup.widgetsFunc = [](LibrarySettings& library) {

		if (ImGui::BeginTable(("TableforLibrary##" + HelperFunctions::GenerateStringHash(&library)).c_str(), 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Borders)) {

			ImGui::TableNextColumn();

			ImGui::Text("Is Variable Name");

			ImGui::TableNextColumn();

			ImGui::Checkbox(("##CheckboxForLib" + HelperFunctions::GenerateStringHash(&library)).c_str(),&library.isVariableName);

			ImGui::TableNextColumn();

			if (!library.isVariableName) {

				ImGui::Text("Shared?");

				ImGui::TableNextColumn();

				ImGui::Checkbox(("##CheckboxForSharedLib" + HelperFunctions::GenerateStringHash(&library)).c_str(), &library.shared);

				ImGui::TableNextColumn();

			}
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


			if (!library.isVariableName) {
			ImGui::TableNextColumn();


				ImGui::Text("Debug Postfix");

				ImGui::TableNextColumn();

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::InputText(("##dpostfixForLibrary" + HelperFunctions::GenerateStringHash(&library)).c_str(), &library.debugPostfix);
			}

			ImGui::EndTable();

		}

	};

	CMakeGenerator::SetupChangeableWidgets(setup);


}
