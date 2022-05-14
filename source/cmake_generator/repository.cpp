#include "repository.h"
#include "cmake_generator.h"


Repository::Repository(std::string type) : m_Type(type)
{

}



void Repository::SetupPopupWidgets()
{
}

bool Repository::CheckRepoValidity()
{

	if (m_Alias == "") {
		CMakeGenerator::ShowErrorPopup("Please add a valid alias to the repository!");
		return false;
	}

	if (CMakeGenerator::FindAliasInRepositories(m_Alias)) {
		CMakeGenerator::ShowErrorPopup("Alias " + m_Alias + " already exists!\nPlease choose another name for your repository.");
		return false;
	}

	return this->IsRepoReady();
}

bool Repository::OnDeserialize(YAML::Node& node)
{
	return false;
}

YAML::Node Repository::OnSerialize()
{
	return YAML::Node();
}

bool Repository::IsRepoReady()
{
	return false;
}

size_t Repository::GetNumberOf(std::string type)
{
	return 0;
}

std::string Repository::GetCMakeListsString()
{
	return std::string();
}

void Repository::OpenPopup()
{
	m_ShouldOpenPopup = true;
}

void Repository::ClosePopup()
{
	m_ShouldOpenPopup = false;
}

bool Repository::IsPopupOpen()
{
	return m_ShouldOpenPopup;
}

bool Repository::Deserialize(YAML::Node& node)
{
	HelperFunctions::DeserializeVariable("alias",m_Alias,node);
	HelperFunctions::DeserializeVariable("type",m_Type,node);
	HelperFunctions::DeserializeVariable("sources",m_SourcesToAdd,node);

	if (node["includes"]) {
		for (auto include_node : node["includes"]) {
			IncludeSettings include;
			HelperFunctions::DeserializeVariable("path",include.path,include_node);
			HelperFunctions::DeserializeVariable("access",include.access,include_node);
			m_Includes.push_back(include);
		}
	}

	if (node["libraries"]) {
		for (auto lib_node : node["libraries"]) {
			LibrarySettings library;
			HelperFunctions::DeserializeVariable("path",library.path,lib_node);
			HelperFunctions::DeserializeVariable("access",library.access,lib_node);
			HelperFunctions::DeserializeVariable("debug_postfix",library.debugPostfix,lib_node);
			HelperFunctions::DeserializeVariable("is_alias",library.isTargetName,lib_node);
			m_Libraries.push_back(library);
		}
	}

	if (node["derived"]) {
		YAML::Node derivedNode = node["derived"];
		this->OnDeserialize(derivedNode);
	}


	return true;

}

YAML::Node Repository::Serialize()
{
	YAML::Node node;
	node["alias"] = m_Alias;
	node["type"] = m_Type;
	node["sources"] = m_SourcesToAdd;
	for (auto& include : m_Includes) {
		YAML::Node includeNode;
		includeNode["path"] = include.path;
		includeNode["access"] = include.access;
		node["includes"].push_back(includeNode);
	}
	for (auto& library : m_Libraries) {
		YAML::Node libNode;
		libNode["path"] = library.path;
		libNode["access"] = library.access;
		libNode["is_alias"] = library.isTargetName;
		libNode["debug_postfix"] = library.debugPostfix;
		node["libraries"].push_back(libNode);
	}

	

	node["derived"] = this->OnSerialize();

	return node;
}


std::string Repository::GetPopupID()
{
	return ("PopupForRepo" + GetHash());
}


std::string Repository::GetHash()
{
	return "##" + HelperFunctions::GenerateStringHash(this);
}

std::string Repository::GetAlias()
{
	return m_Alias;
}

const std::vector<IncludeSettings>& Repository::GetIncludes() const
{
	return m_Includes;
}

std::vector<std::string> Repository::GetSources() const
{
	return HelperFunctions::SplitString(m_SourcesToAdd,"\n");
}

const std::vector<LibrarySettings>& Repository::GetLibraries() const
{
	return m_Libraries;
}

