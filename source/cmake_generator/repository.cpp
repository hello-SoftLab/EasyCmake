#include "repository.h"
#include "cmake_generator.h"


Repository::Repository()
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

