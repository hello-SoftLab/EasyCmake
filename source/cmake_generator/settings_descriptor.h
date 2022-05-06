#pragma once
#include "../global.h"
#include "pointer_holder.h"


class SettingsDescriptor {
public:
	void ShowPropertiesInTable(int numColumns) {};
	
	virtual void WriteToString(std::string& mainString) {};



};



class SettingsHolder : public PointerHolder<SettingsDescriptor> {

};