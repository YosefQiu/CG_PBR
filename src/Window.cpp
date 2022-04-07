#include "Window.h"

Window::Window (int width, int height, const char* name)
{
    SCREEN_WIGHT = width;
    SCREEN_HEIGHT = height;
    SCREEN_NAME = name;
}

Window::~Window ()
{
    glfwTerminate();
}

bool Window::InitGLFW()
{
    if(!glfwInit())
    {
        cout << "[ERROR] Failed to Init GLFW" << endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);
    
    cout << "[1] Init GLFW...." << endl;
    return true;
}

bool Window::InitGLContext()
{
    p_Wnd.reset(glfwCreateWindow(SCREEN_WIGHT, SCREEN_HEIGHT, SCREEN_NAME, NULL, NULL));
    if(p_Wnd == NULL)
    {
        cout << "[ERROR:] Failed to create OpenGL Context" << endl;
        return false;
    }
    else
    {
        glfwMakeContextCurrent(getWnd());
        cout << "[2] Init OpenGL Context..." << endl;
        return true;
    }
}

bool Window::InitGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "[ERROR:]Failed to initialize GLAD" << endl;
        return false;
    }
    else
    {
        cout << "[3] Init GLAD..." << endl;
        return true;
    }
}

bool Window::Init()
{
    if(!InitGLFW())
    {
        return false;
    }
    if(!InitGLContext())
    {
        return false;
    }
    if(!InitGLAD())
    {
        return false;
    }
    glViewport(0, 0, SCREEN_WIGHT, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    return true;
}

GLFWwindow* Window::getWnd() const
{
    return p_Wnd.get();
}

bool Window::IsClose() const
{
    return glfwWindowShouldClose(getWnd());
}

void Window::PollEvents() const
{
    glfwPollEvents();
}

void Window::SwapBuffer() const
{
    glfwSwapBuffers(getWnd());
}

void Window::Close() const
{
    glfwTerminate();
}

void Window::Update(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
