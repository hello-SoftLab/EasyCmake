#include "window.h"
#include "../cmake_generator/cmake_generator.h"


Window::Window(int width, int height) {
    m_CurrentWindow = this;
    if(glfwInit() != GLFW_TRUE){
        DEBUG_LOG("GLEW NOT INITIALIZED!");
        return;
    }

    this->m_ContextPointer = glfwCreateWindow(width,height,"EasyCmake",NULL,NULL);

    glfwMakeContextCurrent(m_ContextPointer);

    if(glewInit() != GLEW_OK){
        DEBUG_ERROR("Glew wasn't initiated!");
        return;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_ContextPointer, true);
    ImGui_ImplOpenGL3_Init("#version 430 core");


    


}


bool Window::IsOpen() {
    return !glfwWindowShouldClose(this->m_ContextPointer);
}


void Window::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Color(50, 50, 50).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ChildBg, Color(40, 40, 40).AsImVec4());
}
bool Window::EndFrame()
{
    ImGui::PopStyleColor(2);
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(m_ContextPointer);
    glfwPollEvents();

    
    return true;
}



void Window::ShowGUI()
{
    static bool firstLoop = true;
    

    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("BackWindow", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking);
    ImGui::PopStyleVar(2);


    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            if (ImGui::MenuItem("Save Configuration As")) {

            }

            if (ImGui::MenuItem("Load Configuration")) {

            }
            ImGui::EndMenu();
        }



        ImGui::EndMainMenuBar();
    }


    
    ImGuiID id = ImGui::GetID("MainWindowGroup");




    ImGui::DockSpace(id);

    if (firstLoop) {


        ImGui::DockBuilderRemoveNode(id);
        ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::DockBuilderSetNodeSize(id, ImGui::GetMainViewport()->WorkSize);
        ImGui::DockBuilderSetNodePos(id, ImGui::GetMainViewport()->WorkPos);

        ImGuiID firstWindow = ImGui::DockBuilderSplitNode(id,ImGuiDir_Left,1.0f,nullptr,&id);

        ImGui::DockBuilderDockWindow("CmakeWindow",firstWindow);

        ImGui::DockBuilderFinish(id);
        firstLoop = false;


    }

    CMakeGenerator::ShowMainWindow();


    ImGui::End();


}

Window& Window::GetCurrentWindow()
{
    return *m_CurrentWindow;
}
