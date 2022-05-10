#pragma once
#include "../global.h"
#include "pointer_holder.h"



class Repository {
public:
	Repository();

	virtual void SetupPopupWidgets();
	bool CheckRepoValidity(std::string& errorMsg);

	virtual size_t GetNumberOf(std::string type);


	void OpenPopup();
	void ClosePopup();

	bool IsPopupOpen();


	

	std::string GetPopupID();

	std::string GetHash();

	std::string GetAlias();

protected:
	virtual bool IsRepoReady(std::string& errorMsg);

	std::string m_Alias = "";
private:
	bool m_ShouldOpenPopup = false;
	
	friend class CMakeGenerator;
};

class RepositoryHandle : public PointerHolder<Repository> {

};