#include "repository.h"
#include "repository.h"
#include "repository.h"
#include "repository.h"
#include "repository.h"


Repository::Repository()
{
}



void Repository::SetupPopupWidgets()
{
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

RepositoryHandle::operator bool() const
{
	return m_Pointer.operator bool();
}

void RepositoryHandle::ClearCurrentType()
{
	m_Pointer.reset();
}

Repository* RepositoryHandle::Get()
{
	return m_Pointer.get();
}

std::string RepositoryHandle::GetType()
{
	return m_CurrentType;
}
