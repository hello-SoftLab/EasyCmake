#pragma once
#include "../global.h"


enum class WindowType {
	Simple = 0,
	Advanced = 1
};


class WindowManager {
public:
	static void HandleWindowModes();
	static void SetWindowMode(WindowType type);

private:
	static inline WindowType m_CurrentType = WindowType::Simple;
	static inline bool m_ShouldSetup = true;

	static void SetupSimpleWindow();
	static void SetupAdvancedWindow();





};