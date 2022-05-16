#include "window.h"
#include "../cmake_generator/cmake_generator.h"
#include "../cmake_generator/window_manager.h"
#include "../cmake_generator/cmake_serializer.h"

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


    glfwSetWindowCloseCallback(this->GetContextPointer(), [](GLFWwindow* ptr) {
            Window::GetCurrentWindow().m_ClosingEvent.EmitEvent();
        
        });


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

            if (ImGui::MenuItem("Reset Current")) {
                CMakeGenerator::ClearCurrentSettings();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Save As")) {
                static std::string saveNameForSerialization = "";
                static bool isSaveNameForOverwriting = false;
                saveNameForSerialization = "";
                CustomPopupProperties prop;
                prop.initialSize = ImVec2(300,200);
                prop.title = "Please Choose a Save Name";
                prop.widgetFunc = [&]() {
                    
                    ImGui::Text("Current Configurations");
                    ImGui::BeginChild("ChildForLoadingConfigOverwriting",ImVec2(ImGui::GetWindowSize().x - ImGui::GetCursorPosX(),70),true,ImGuiWindowFlags_AlwaysVerticalScrollbar);

                    if (ImGui::BeginTable("TableForConfigLoadingOverwriting",1)) {

                        bool foundAnyEqual = false;
                        std::string deletionConfig = "";
                        for (auto& config : CMakeSerializer::GetSavedConfigs()) {
                            ImGui::TableNextColumn();

                            const bool selected = saveNameForSerialization == config.first.as < std::string>();

                            if (ImGui::Selectable(config.first.as<std::string>().c_str(), selected)) {
                                saveNameForSerialization = config.first.as<std::string>();
                                
                            }

                            if (ImGui::IsItemHovered()) {
                                std::string toolTip = CMakeSerializer::GetSaveStringRepresentation(config.first.as<std::string>());

                                ImGui::SetTooltip(toolTip.c_str());
                            }
                            if (ImGui::BeginPopupContextItem()) {
                                if (ImGui::MenuItem("Delete")) {
                                    deletionConfig = config.first.as<std::string>();
                                }
                                
                                ImGui::EndPopup();
                            }


                            if (selected) {
                                isSaveNameForOverwriting = true;
                                foundAnyEqual = true;
                            }

                        }

                        if (deletionConfig != "") {
                            CMakeSerializer::RemoveSave(deletionConfig);
                        }

                        if (!foundAnyEqual) {
                            isSaveNameForOverwriting = false;
                        }

                        ImGui::EndTable();
                    }

                    

                    ImGui::EndChild();

                    if (ImGui::BeginTable(("SavingConfigTable"),2)) {


                        ImGui::TableNextColumn();

                        ImGui::Text("Name");

                        ImGui::TableNextColumn();

                        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                        ImGui::InputText("##NameOfConfig",&saveNameForSerialization);
                       

                        ImGui::EndTable();
                    }

                    std::string buttonTitle = isSaveNameForOverwriting ? "Overwrite" : "Ok";

                    ImGui::SetCursorPos(ImVec2(2, ImGui::GetWindowSize().y - (ImGui::CalcTextSize("A").y * 2 - 2)));

                    if (ImGui::Button(buttonTitle.c_str())) {
                        if (isSaveNameForOverwriting) {
                            CMakeSerializer::RemoveSave(saveNameForSerialization);
                        }
                        CMakeSerializer::SerializeToSave(saveNameForSerialization);
                        CMakeGenerator::CloseCustomPopup();
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Cancel")) {
                        CMakeGenerator::CloseCustomPopup();
                    }
                    
                };
                CMakeGenerator::ShowCustomPopup(prop);
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Load")) {
                if (CMakeSerializer::GetSavedConfigs().size() > 0) {
                    static std::string chosenSaveForDeserialization = "";
                    chosenSaveForDeserialization = "";
                    CustomPopupProperties prop;
                    prop.title = "Saved Configurations";
                    prop.initialSize = ImVec2(300,300);
                    prop.widgetFunc = [&]() {

                        ImGui::PushStyleColor(ImGuiCol_Header,Color(50,50,50).AsImVec4());
                        ImGui::Selectable("Please choose a save",true,ImGuiSelectableFlags_SpanAvailWidth | ImGuiSelectableFlags_Disabled);
                        ImGui::PopStyleColor();
                        
                        if (ImGui::BeginTable(("TableFor" + HelperFunctions::GenerateStringHash(&chosenSaveForDeserialization)).c_str(),2,ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchSame)) {
                            int index = 0;
                            ImGui::TableSetupColumn("first",0,10);
                            ImGui::TableSetupColumn("second", 0, 0.2);
                            
                            std::string configToRemove = "";
                            for (auto& config : CMakeSerializer::GetSavedConfigs()) {
                                ImGui::TableNextColumn();
                                const bool selected = chosenSaveForDeserialization == config.first.as<std::string>();
                                if (ImGui::Selectable(config.first.as<std::string>().c_str(), selected)) {
                                    chosenSaveForDeserialization = config.first.as<std::string>();
                                }
                                if (ImGui::IsItemHovered()) {
                                    std::string toolTip = CMakeSerializer::GetSaveStringRepresentation(config.first.as<std::string>());

                                    ImGui::SetTooltip(toolTip.c_str());
                                }

                                ImGui::TableNextColumn();

                                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                                if (ImGui::Selectable(("X##" + std::to_string(std::hash<int>()(index))).c_str())) {
                                    configToRemove = config.first.as<std::string>();
                                }
                               

                                index++;
                            }
                            if (configToRemove != "") {
                                CMakeSerializer::RemoveSave(configToRemove);
                            }

                            ImGui::EndTable();
                        }

                        if (chosenSaveForDeserialization != "") {
                            

                            ImGui::SetCursorPos(ImVec2(ImGui::CalcTextSize("A").x, ImGui::GetWindowSize().y - (ImGui::CalcTextSize("A").y * 2 + 1)));
                            if (ImGui::Button("Load")) {
                                CMakeSerializer::DeserializeSavedConfig(chosenSaveForDeserialization);
                                CMakeGenerator::CloseCustomPopup();
                            }
                        }



                    };
                    CMakeGenerator::ShowCustomPopup(prop);
                }
            }
            ImGui::EndMenu();
        }



        ImGui::EndMainMenuBar();
    }


    WindowManager::HandleWindowModes();

    CMakeGenerator::ShowMainWindow();


    ImGui::End();


}

GLFWwindow* Window::GetContextPointer()
{
    return m_ContextPointer;
}

Window& Window::GetCurrentWindow()
{
    return *m_CurrentWindow;
}

FunctionSink<void()> Window::OnClosing()
{
    return FunctionSink<void()>(m_ClosingEvent);
}
