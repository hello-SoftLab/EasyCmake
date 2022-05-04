#pragma once
#include "../global.h"




class Repository {
public:
	Repository();

	virtual void SetupPopupWidgets();
;

	std::string GetAlias();

protected:
	std::string m_Alias = "";

	
	friend class CMakeGenerator;
};

class RepositoryHandle {
public:
	operator bool() const;

	template<typename T>
	requires std::is_base_of<Repository,T>::value
	void HoldType() {
		auto deleter = [](Repository* ptr) {
			delete ((T*)ptr);
		};
		m_CurrentType = HelperFunctions::GetClassName<T>();
		m_Pointer = std::shared_ptr<Repository>(new T(),deleter);
	}

	template<typename T>
	T* GetAs() {
		return (T*)m_Pointer.get();
	}

	Repository* Get();

	std::string GetType();


private:
	std::string m_CurrentType = "";
	std::shared_ptr<Repository> m_Pointer;


};