#include <iostream>
#include "global.h"
#include "window/window.h"
#include "object/tagged_object.h"


int main() {

	Window window(1280,720);

	
	

	while (window.IsOpen()) {

		window.BeginFrame();

		window.ShowGUI();


		window.EndFrame();
	}


};