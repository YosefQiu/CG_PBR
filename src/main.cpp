#include "ggl.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"
#include "Config.h"
#include "FrameBufferObject.h"
#include "Scene.h"

#define SCR_WIDTH		1280
#define SCR_HEIGHT		720
const char* SCR_WND_NAME = "Project";

unique_ptr<Window>	wnd		 = NULL;
unique_ptr<Camera>	myCamera = NULL;
unique_ptr<UI>		myUI	 = NULL;
Model*	mySphere			 = NULL;
Model*  myClothes			 = NULL;
Shader* teapotshader		 = NULL;
Shader* clothesshader		 = NULL;
Scene*	myScene				 = NULL;

#pragma region interactive


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
#pragma region cameraview
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		myCamera->SpeedZ = 1.0f;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		myCamera->SpeedZ = -1.0f;
	else
		myCamera->SpeedZ = 0.0f;
	myCamera->CameraUpdatePos();
	myUI->ImguiUpdateCamera(myCamera.get());
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		myCamera->SpeedX = -1.0f;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		myCamera->SpeedX = 1.0f;
	else
		myCamera->SpeedX = 0.0f;
	myCamera->CameraUpdatePos();
	myUI->ImguiUpdateCamera(myCamera.get());
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		myCamera->SpeedY = 1.0f;
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		myCamera->SpeedY = -1.0f;
	else
		myCamera->SpeedY = 0.0f;
	myCamera->CameraUpdatePos();
	myUI->ImguiUpdateCamera(myCamera.get());
#pragma endregion cameraview
#pragma endregion cameraview

    
}
#pragma endregion interactive

void RenderMain();

glm::vec3 lightPosition[] =
{
	glm::vec3(0.0f, 0.0f, 5.0f),
};
glm::vec3 lightColor[] =
{
	glm::vec3(150.0f, 150.0f, 150.0f),
};

int main(int argc, char* argv[])
{
#pragma region createwind
    // Create and Init the Window
	wnd.reset(new Window(SCR_WIDTH, SCR_HEIGHT, SCR_WND_NAME));
	if (!wnd->Init())
	{
		wnd->Close();
		return -1;
	}
#pragma endregion createwind

#pragma region interaction
	// Set interaction
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
	string obj_filename = "../res/model/sphere.obj";
	mySphere = new Model(obj_filename);
	myClothes = new Model(obj_filename);
#pragma endregion loadmodel

#pragma region shader
	teapotshader = new Shader("../res/shader/vert.glsl", "../res/shader/frag.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");	
	mySphere->SetShader(teapotshader);
	clothesshader = new Shader("../res/shader/vert.glsl", "../res/shader/brdf.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");
	myClothes->SetShader(clothesshader);
#pragma endregion shader

#pragma region loadscene
	myScene = new Scene();
	myScene->LoadScene("Sphere", mySphere);
	myScene->LoadScene("Clothes", myClothes);
#pragma endregion loadscene

#pragma region loadcamera
    // Create Camera
	myCamera.reset(new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::radians(5.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	myUI->ImguiUpdateCamera(myCamera.get());
#pragma endregion loadcamera
	
#pragma region modelmatrix
	// Calc the MVP matrix
	ViewMatrix = myCamera->GetViewMatrix();
	ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
#pragma endregion modelmatrix

	std::cout << "[4] Start Rendering..." << std::endl;
	while (!wnd->IsClose())
	{
		myUI->ImguiCreateWindow();
		//rendering
		wnd->Update(0.0f, 0.34f, 0.57f);
////--------------------------------------------------------------------
#pragma region renderscene
		
		myScene->Render(RenderMain);

#pragma endregion renderscene
		
//--------------------------------------------------------------------
		myUI->ImguiRender();

		wnd->SwapBuffer();
		wnd->PollEvents();
	}

	wnd->Close();
	return 0;
}



void RenderMain()
{
	if (myUI->m_matType == PICTURE)
	{
		if (myUI->b_fileChange)
		{
			//Texture
			std::cout << "[IMAGE]::Changing the Texture map [" << myUI->m_filePath << "] " << std::endl;
			myScene->GetModel("Sphere")->SetTexture(myUI->m_filePath + "/");
			myUI->ImguiUpdateTexture(myScene->GetModel("Sphere")->m_mapTexnameToTexid);
			myUI->b_fileChange = false;
		}
		else if (myUI->b_fileFirst)
		{
			myScene->GetModel("Sphere")->SetTexture("../res/pic/Glass_Vintage_001_SD/");
			myUI->ImguiUpdateTexture(myScene->GetModel("Sphere")->m_mapTexnameToTexid);
			myUI->b_fileFirst = false;
		}

		glm::mat4 rot = glm::mat4(1.0f);
		rot = glm::rotate(glm::radians((float)glfwGetTime() * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myScene->GetModel("Sphere")->SetMVP(rot * ModelMatrix, ViewMatrix, ProjMatrix);
		myScene->GetModel("Sphere")->GetShader()->SetVec3("cameraPosition", myCamera->Position);
		myScene->GetModel("Sphere")->SetImguiParameter(teapotshader, myUI.get());
		myScene->GetModel("Sphere")->BindTexture();

		for (int i = 0; i < sizeof(lightPosition) / sizeof(lightPosition[0]); i++)
		{
			glm::vec3 newPos = lightPosition[i] + glm::vec3(sin(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			newPos = lightPosition[i];
			myScene->GetModel("Sphere")->GetShader()->SetVec3("lightPosition[" + std::to_string(i) + "]", newPos);
			myScene->GetModel("Sphere")->GetShader()->SetVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);
			myScene->DrawScene("Sphere", false, false);
		}
	}
	if (myUI->m_matType == PARAMETER)
	{
		glm::mat4 rot = glm::mat4(1.0f);
		rot = glm::rotate(glm::radians((float)glfwGetTime() * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myScene->GetModel("Clothes")->SetMVP(rot * ModelMatrix, ViewMatrix, ProjMatrix);
		myScene->GetModel("Clothes")->GetShader()->SetVec3("cameraPosition", myCamera->Position);
		myScene->GetModel("Clothes")->SetImguiParameter(clothesshader, myUI.get());

		for (int i = 0; i < sizeof(lightPosition) / sizeof(lightPosition[0]); i++)
		{
			glm::vec3 newPos = lightPosition[i] + glm::vec3(sin(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			newPos = lightPosition[i];
			myScene->GetModel("Clothes")->GetShader()->SetVec3("lightPosition[" + std::to_string(i) + "]", newPos);
			myScene->GetModel("Clothes")->GetShader()->SetVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);
			myScene->DrawScene("Clothes", false, false);
		}
	}
}
