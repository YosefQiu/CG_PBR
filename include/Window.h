#pragma once
#include "ggl.h"

struct GLFWDelete
{
    void operator()(GLFWwindow* pWind)
    {
        cout << "Destory GLFW Window Context" << endl;
        glfwDestroyWindow(pWind);
    }
};



class Window
{
public:
    Window (int width, int height, const char* name);
    virtual ~Window ();
public:
    unique_ptr<GLFWwindow, GLFWDelete> p_Wnd;
    const char* SCREEN_NAME;
    int SCREEN_WIGHT;
    int SCREEN_HEIGHT;
public:
    
    GLFWwindow* getWnd() const;
    bool Init();
    bool IsClose() const;
    void PollEvents() const;
    void SwapBuffer() const;
    void Close() const;
    void Update(float r, float g, float b, float a = 1.0f);
private:
    bool InitGLFW();
    bool InitGLContext();
    bool InitGLAD();
};

