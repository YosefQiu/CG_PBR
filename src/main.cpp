#include "ggl.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"
#include "Config.h"


#define SCR_WIDTH		1280
#define SCR_HEIGHT		720
const char* SCR_WND_NAME = "Project";

unique_ptr<Window>	wnd		 = NULL;
unique_ptr<Camera>	myCamera = NULL;
unique_ptr<UI>		myUI	 = NULL;
Model*	myTeapot			 = NULL;
Shader* teapotshader		 = NULL;
LightDirection* myLightD = new LightDirection(LightDPos, LightDAngles, LightDColor);
#pragma region interactiv


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
#pragma region cameraview
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		myCamera->SpeedZ = 1.0f;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		myCamera->SpeedZ = -1.0f;
	else
		myCamera->SpeedZ = 0.0f;
	myCamera->CameraUpdatePos();
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		myCamera->SpeedX = -1.0f;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		myCamera->SpeedX = 1.0f;
	else
		myCamera->SpeedX = 0.0f;
	myCamera->CameraUpdatePos();
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		myCamera->SpeedY = 1.0f;
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		myCamera->SpeedY = -1.0f;
	else
		myCamera->SpeedY = 0.0f;
	myCamera->CameraUpdatePos();
#pragma endregion cameraview


}
#pragma endregion interactiv


glm::vec3 pos[]
{
	glm::vec3(-15.0f, 25.0f, -10.0f),
	glm::vec3(15.0f, 25.0f, -10.0f),
	glm::vec3(-15.0f, -10.0f, -10.0f),
	glm::vec3(15.0f, -10.0f, -10.0f),
	glm::vec3(-25.0f, 7.5f, -10.0f),
	glm::vec3(25.0f, 7.5f, -10.0f),
};

int main(int argc, char* argv[])
{

    // Create and Init the Window
	wnd.reset(new Window(SCR_WIDTH, SCR_HEIGHT, SCR_WND_NAME));
	if (!wnd->Init())
	{
		wnd->Close();
		return -1;
	}
    // Set interaction
#pragma region interaction
	glfwSetFramebufferSizeCallback(wnd->getWnd(), framebuffer_size_callback);
	glfwSetCursorPosCallback(wnd->getWnd(), mouse_callback);
    glfwSetMouseButtonCallback(wnd->getWnd(), mouse_button_callback);
	glfwSetKeyCallback(wnd->getWnd(), key_callback);
#pragma endregion interaction

#pragma region loadUI
	//UI
	myUI.reset(new UI(wnd->getWnd()));
	myUI->ImguiInit();
#pragma endregion loadUI

#pragma region loadmodel
	string obj_filename = "../res/model/teapot/teapot.obj";
	myTeapot = new Model(obj_filename);
#pragma endregion loadmodel

#pragma region shader
	teapotshader = new Shader("../res/shader/teapot_shadow_mapping_vert.glsl", 
		"../res/shader/teapot_shadow_mapping_frag.glsl");
	
#pragma endregion shader
	
    // Create Camera
	myCamera.reset(new Camera(glm::vec3(0.0f, 0.0f, 50.0f), glm::radians(10.0f), glm::radians(180.0f) , glm::vec3(0.0f, 1.0f, 0.0f)));
	
    // Calc the MVP matrix
#pragma region modelmatrix
	ViewMatrix = myCamera->GetViewMatrix();
	ModelMatrix = glm::mat4(1.0f);
	
	ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	
#pragma endregion modelmatrix

	cout << "[4] Start Redering..." << endl;
	while (!wnd->IsClose())
	{
		myUI->ImguiCreateWindow();
		//rendering
		wnd->Update(0.0f, 0.34f, 0.57f);

////--------------------------------------------------------------------
#pragma region renderscene
		for (int i = 0; i < 6; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pos[i]);
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			teapotshader->Use();
			ViewMatrix = myCamera->GetViewMatrix();
			teapotshader->SetMat4("M", model);
			teapotshader->SetMat4("V", ViewMatrix);
			teapotshader->SetMat4("P", ProjMatrix);
			myTeapot->SetLight(teapotshader, myLightD, NULL, NULL);
			myTeapot->Draw(teapotshader, true, true);
		}
		ViewMatrix = myCamera->GetViewMatrix();
		glm::mat4 rota = glm::mat4(1.0f);
		rota = glm::rotate(rota, glm::radians((float)glfwGetTime() * 3), glm::vec3(0.0f, 1.0f, 0.0f));
		teapotshader->SetMat4("M", rota);
		teapotshader->SetMat4("V", ViewMatrix);
		teapotshader->SetMat4("P", ProjMatrix);
		myTeapot->SetLight(teapotshader, myLightD, NULL, NULL);
		myTeapot->Draw(teapotshader, true, true);

#pragma endregion renderscene
		
	
//--------------------------------------------------------------------
		myUI->ImguiRender();

		wnd->SwapBuffer();
		wnd->PollEvents();
	}

	wnd->Close();
	return 0;
}




