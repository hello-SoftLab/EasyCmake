#pragma once
#include "../global.h"
#include "pointer_holder.h"


class Library {



private:
	std::string m_Path = "";
	std::string m_DebugPostfix = "";

};


class LibraryHandle : public PointerHolder<Library> {

};

