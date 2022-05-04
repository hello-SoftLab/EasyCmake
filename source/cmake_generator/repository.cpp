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


std::string Repository::GetAlias()
{
	return m_Alias;
}

RepositoryHandle::operator bool() const
{
	return m_Pointer.operator bool();
}

Repository* RepositoryHandle::Get()
{
	return m_Pointer.get();
}

std::string RepositoryHandle::GetType()
{
	return m_CurrentType;
}
