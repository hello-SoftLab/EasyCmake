#include <iostream>
#include "global.h"
#include "window/window.h"
#include "object/tagged_object.h"


int main() {

	Window window(640,480);

	Object obj = ObjectPropertyRegister::CreateNew<Object>("HI");

	HelperFunctions::RegisterObjectMemberMetaFunction<Object>("Get My Name",&Object::GetName);

	while (window.IsOpen()) {

		window.BeginFrame();

		window.ShowGUI();


		window.EndFrame();
	}


};