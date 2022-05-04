#pragma once

#include "../global.h"
#include <iostream>

// dummy class to make the functions available to the register;

class ObjectBase {

protected:
	virtual void Init() {};
	virtual void Destroy() {};

	/**
	 * Serializer function.
	 * Called before Destroy().
	 * *WARNING* -> node is already inside of component
	 */
	virtual YAML::Node Serialize() { return {}; };

	/**
	 * Deserializer function.
	 * Called after Init()
	 * *WARNING* -> node is already inside of component
	 */
	virtual bool Deserialize(YAML::Node& node) { return true; };



private:

	friend class ObjectPropertyRegister;


};