#pragma once
#include "global.h"


class Window {
public:
    Window(int width,int height);

    bool IsOpen();
    bool EndFrame();
    void ShowGUI();
    void BeginFrame();
  
    
    

    GLFWwindow* GetContextPointer();
    static Window& GetCurrentWindow();
    FunctionSink<void()> OnClosing();

private:
    static inline Window* m_CurrentWindow = nullptr;

    EventLauncher<void()> m_ClosingEvent;
    GLFWwindow* m_ContextPointer = nullptr;

};