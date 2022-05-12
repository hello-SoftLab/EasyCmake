#include "subdirectory_settings.h"
#include "cmake_generator.h"

void SubdirectorySettings::ShowWidgets(std::vector<SubdirectorySettings>& vec)
{
	CMakeGeneratorChangeableWidgetSetup setup(vec);

	setup.mainTextStringFunc = [](SubdirectorySettings& settings) {return &settings.subdir; };


	CMakeGenerator::SetupChangeableWidgets(setup);

}
