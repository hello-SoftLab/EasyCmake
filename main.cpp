#include <iostream>
#include "global.h"
#include "window/window.h"
#include "object/tagged_object.h"
#include "cmake_generator/cmake_serializer.h"

int main() {

	Window window(1280,720);


	CMakeSerializer::Init();
	
	

	while (window.IsOpen()) {

		window.BeginFrame();

		window.ShowGUI();


		window.EndFrame();
	}
	


};