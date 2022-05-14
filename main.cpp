#include <iostream>
#include "global.h"
#include "window/window.h"
#include "object/tagged_object.h"
#include "cmake_generator/cmake_serializer.h"

int main() {

	Window window(1280,720);

	CMakeSerializer::LoadConfigsFromFile("save_files.yaml");
	window.OnClosing().Connect([](){
		//DEBUG_LOG("Saving...");
		CMakeSerializer::SaveCurrentConfigsToFile("save_files.yaml");
	});
	

	while (window.IsOpen()) {

		window.BeginFrame();

		window.ShowGUI();


		window.EndFrame();
	}
	


};