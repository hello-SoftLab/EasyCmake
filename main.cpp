#include <iostream>
#include "global.h"
#include "window/window.h"

int main() {

	Window window(640,480);

	while (window.IsOpen()) {

		window.BeginFrame();

		window.ShowGUI();


		window.EndFrame();
	}


};