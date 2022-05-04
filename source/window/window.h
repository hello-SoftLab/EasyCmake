#pragma once
#include "global.h"


class Window {
public:
    Window(int width,int height);

    bool IsOpen();
    bool EndFrame();
    void ShowGUI();
    void BeginFrame();
    


    static Window& GetCurrentWindow();

private:
    static inline Window* m_CurrentWindow = nullptr;

    GLFWwindow* m_ContextPointer = nullptr;

};