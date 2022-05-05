#pragma once
#include "../global.h"

template<typename Derived>
class PointerHolder {
public:
	operator bool() const {
		return m_Pointer.operator bool();
	};

	template<typename T>
	requires std::is_base_of<Derived, T>::value
	void HoldType() {
		auto deleter = [](Derived* ptr) {
			delete ((T*)ptr);
		};
		m_CurrentType = HelperFunctions::GetClassName<T>();
		m_Pointer = std::shared_ptr<Derived>(new T(), deleter);
	}

	void ClearCurrentType() {
		m_Pointer.reset();
	}

	template<typename T>
	T* GetAs() {
		return (T*)m_Pointer.get();
	}

	Derived* Get() {
		return m_Pointer.get();
	}

	std::string GetType() {
		return m_CurrentType;
	}
private:
	std::string m_CurrentType = "";
	std::shared_ptr<Derived> m_Pointer;

};

