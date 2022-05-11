#include "repository.h"
#include "cmake_generator.h"


Repository::Repository()
{
}



void Repository::SetupPopupWidgets()
{
}

bool Repository::CheckRepoValidity(std::string& errorMsg)
{

	if (m_Alias == "") {
		errorMsg = "Please add a valid alias to the repository!";
		return false;
	}

	if (CMakeGenerator::FindAliasInRepositories(m_Alias)) {
		errorMsg = "Alias " + m_Alias + " already exists!\nPlease choose another name for your repository.";
		return false;
	}

	return this->IsRepoReady(errorMsg);
}

bool Repository::IsRepoReady(std::string& errorMsg)
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

