#include "window_manager.h"
#include "../window/window.h"

void WindowManager::HandleWindowModes()
{
	ImGuiID id = ImGui::GetID("MainWindowGroup");

	ImGui::DockSpace(id);

	if (m_ShouldSetup) {
		switch (m_CurrentType) {
		case WindowType::Simple:
			SetupSimpleWindow();
			break;
		case WindowType::Advanced:
			SetupAdvancedWindow();
			break;
		}
		m_ShouldSetup = false;
	}
}

void WindowManager::SetWindowMode(WindowType type)
{
	if (type != m_CurrentType) {
		switch (type) {
		case WindowType::Simple:
			glfwSetWindowSize(Window::GetCurrentWindow().GetContextPointer(),640,480);
			break;
		case WindowType::Advanced:
			glfwSetWindowSize(Window::GetCurrentWindow().GetContextPointer(),1280,720);
			break;

		}
	}
	m_CurrentType = type;
	m_ShouldSetup = true;
}

void WindowManager::SetupSimpleWindow()
{
	ImGuiID id = ImGui::GetID("MainWindowGroup");

	//ImGui::DockSpace(id);

	ImGui::DockBuilderRemoveNode(id);
	ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::DockBuilderSetNodeSize(id, ImGui::GetMainViewport()->WorkSize);
	ImGui::DockBuilderSetNodePos(id, ImGui::GetMainViewport()->WorkPos);

	ImGuiID firstWindow = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 1.0f, nullptr, &id);

	ImGui::DockBuilderDockWindow("CmakeWindow", firstWindow);

	ImGui::DockBuilderFinish(id);
	
}

void WindowManager::SetupAdvancedWindow()
{
	ImGuiID id = ImGui::GetID("MainWindowGroup");

	//ImGui::DockSpace(id);

	ImGui::DockBuilderRemoveNode(id);
	ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_PassthruCentralNode );

	ImGui::DockBuilderSetNodeSize(id, ImGui::GetMainViewport()->WorkSize);
	ImGui::DockBuilderSetNodePos(id, ImGui::GetMainViewport()->WorkPos);

	ImGuiID firstWindow = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 1.0f, nullptr, &id);

	ImGui::DockBuilderDockWindow("CmakeWindow", firstWindow);

	ImGui::DockBuilderFinish(id);
}
