#pragma once
#include "../global.h"
#include <variant>

template<typename Derived>
class PointerHolder {
public:
	operator bool() const {

		if (std::holds_alternative<std::monostate>(m_Pointer)) {
			return false;
		}
		if (IsHolding()) {
			return std::get<std::shared_ptr<Derived>>(m_Pointer).operator bool();
		}

		if (IsObserver()) {
			return std::get<std::weak_ptr<Derived>>(m_Pointer).lock().operator bool();
		}

		return false;
		
	};

	template<typename T>
	requires std::is_base_of<Derived, T>::value
	T& HoldType() {
		auto deleter = [](Derived* ptr) {
			delete ((T*)ptr);
		};
		m_CurrentType = HelperFunctions::GetClassName<T>();
		m_Pointer = std::shared_ptr<Derived>(new T(), deleter);
		return *((T*)std::get< std::shared_ptr<Derived>>(m_Pointer).get());
	}


	void WatchPointer(const PointerHolder<Derived>& pointer) {
		if (!std::holds_alternative<std::shared_ptr<Derived>>(pointer.m_Pointer)) {
			DEBUG_LOG("Trying to watch a pointer that is not valid!");
			return;
		}
		m_CurrentType = pointer.m_CurrentType;
		m_Pointer = std::weak_ptr<Derived>(std::get<std::shared_ptr<Derived>>(pointer.m_Pointer));
	};

	void ClearCurrentType() {
		m_Pointer = std::monostate();
	}

	template<typename T>
	bool IsHoldingType() const {
		return m_CurrentType == HelperFunctions::GetClassName<T>();
	}

	template<typename T>
	T* GetAs() const {
		if (std::holds_alternative< std::shared_ptr<Derived>>(m_Pointer)) {
			return (T*)std::get<std::shared_ptr<Derived>>(m_Pointer).get();
		}
		if (std::holds_alternative<std::weak_ptr<Derived>>(m_Pointer)) {
			return (T*)std::get<std::weak_ptr<Derived>>(m_Pointer).lock().get();
		}

		return nullptr;
	}

	

	Derived* Get() const {
		Derived* ptr = nullptr;

		if (IsHolding()) {
			ptr = (Derived*)std::get<std::shared_ptr<Derived>>(m_Pointer).get();
		}
		if (IsObserver()) {
			ptr = (Derived*)std::get<std::weak_ptr<Derived>>(m_Pointer).lock().get();
		}

		return ptr;
	}

	bool operator==(const PointerHolder<Derived>& other) const{
		if (!other || !this) {
			return false;
		};

		return (this->m_CurrentType == other.m_CurrentType) && (this->Get() == other.Get());
	};

	std::string GetType() {
		return m_CurrentType;
	}

	bool IsHolding() const {
		return std::holds_alternative<std::shared_ptr<Derived>>(m_Pointer);
	}

	bool IsObserver() const{
		return std::holds_alternative<std::weak_ptr<Derived>>(m_Pointer);
	}

private:
	std::string m_CurrentType = "";
	std::variant<std::monostate, std::shared_ptr<Derived>, std::weak_ptr<Derived>> m_Pointer;

};


