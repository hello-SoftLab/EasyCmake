#include "installed_package.h"
#include "cmake_generator.h"


InstalledPackage::InstalledPackage() : Repository(HelperFunctions::GetClassName<InstalledPackage>())
{
}

bool InstalledPackage::OnDeserialize(YAML::Node& node)
{

	HelperFunctions::DeserializeVariable("package_name",m_PackageName,node);
	HelperFunctions::DeserializeVariable("required",m_Required,node);

	return true;
}

YAML::Node InstalledPackage::OnSerialize()
{
	YAML::Node node;

	HelperFunctions::SerializeVariable("package_name",m_PackageName,node);
	HelperFunctions::SerializeVariable("required",m_Required,node);

	return node;
}



std::string InstalledPackage::GetCMakeListsString()
{
	std::string stringToAdd = "";

	stringToAdd += fmt::format(R"(
#find package settings for alias {}
)",m_Alias);

	stringToAdd += fmt::format(R"(
	find_package({})",m_PackageName);
	if (m_Required) {
		stringToAdd += " REQUIRED";
	}

	stringToAdd += ")\n";


	return stringToAdd;
}

void InstalledPackage::SetupPopupWidgets()
{
	if (ImGui::BeginTable(GetHash().c_str(),2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInner)) {

		ImGui::TableNextColumn();

		ImGui::Text("Package Name");

		ImGui::TableNextColumn();

		ImGui::InputText(("##packageName" + GetHash()).c_str(),&m_PackageName);

		ImGui::TableNextColumn();

		ImGui::Text("Required?");

		ImGui::TableNextColumn();

		ImGui::Checkbox(("##checkboxForPackage" + GetHash()).c_str(),&m_Required);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Include Paths");

		ImGui::TableNextColumn();

		IncludeSettings::ShowWidgets(m_Includes);

		ImGui::TableNextColumn();

		ImGui::TextWrapped("Libraries");

		ImGui::TableNextColumn();

		LibrarySettings::ShowWidgets(this->m_Libraries);

		

		ImGui::EndTable();
	}

}

size_t InstalledPackage::GetNumberOf(std::string name)
{
	return 0;
}

bool InstalledPackage::IsRepoReady()
{

	if (m_PackageName == "") {
		CMakeGenerator::ShowErrorPopup("Please provide a package name");
		return false;
	}
	

	return true;
}
