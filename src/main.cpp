#include "ggl.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"
#include "Config.h"
#include "FrameBufferObject.h"
#include "stb_image.h"

#define SCR_WIDTH		1280
#define SCR_HEIGHT		720
const char* SCR_WND_NAME = "Project";

unique_ptr<Window>	wnd		 = NULL;
unique_ptr<Camera>	myCamera = NULL;
unique_ptr<UI>		myUI	 = NULL;
Model*	mySphere			 = NULL;
Shader* teapotshader		 = NULL;
//LightDirection* myLightD = new LightDirection(LightDPos, LightDAngles, LightDColor);
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
#pragma endregion cameraview

    
}
#pragma endregion interactive

unsigned int LoadTex(const char* path)
{
	unsigned int TextureID;

	glGenTextures(1, &TextureID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout << "[IMAGE]::Success to load image [" << path << "] " << std::endl;
	}
	else
		std::cout << "ERROR:Failed to load texture..." << std::endl;
	stbi_image_free(data);
	return TextureID;
}


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
	string obj_filename = "../res/model/sphere.obj";
	mySphere = new Model(obj_filename);
#pragma endregion loadmodel

#pragma region shader
	//teapotshader = new Shader("../res/shader/vert.glsl", "../res/shader/frag.glsl");
#pragma endregion shader
	teapotshader = new Shader("../res/shader/vert.glsl", "../res/shader/frag.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");
#pragma endregion shader
	
    // Create Camera
	myCamera.reset(new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::radians(5.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	//Texture
	unsigned int albedo = LoadTex("../res/pic/Metal_Plate_047_SD/albedo.jpg");
	unsigned int normal = LoadTex("../res/pic/Metal_Plate_047_SD/normal.jpg");
	unsigned int metallic = LoadTex("../res/pic/Metal_Plate_047_SD/metallic.jpg");
	unsigned int roughness = LoadTex("../res/pic/Metal_Plate_047_SD/roughness.jpg");
	unsigned int ao = LoadTex("../res/pic/Metal_Plate_047_SD/ao.jpg");
	unsigned int height = LoadTex("../res/pic/Metal_Plate_047_SD/height.png");


    // Calc the MVP matrix
#pragma region modelmatrix
	ViewMatrix = myCamera->GetViewMatrix();
	ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	
	glm::vec3 lightPosition[] =
	{
		glm::vec3(0.0f, 0.0f, 5.0f),
	};
	glm::vec3 lightColor[] =
	{
		glm::vec3(150.0f, 150.0f, 150.0f),
	};

#pragma endregion modelmatrix

	std::cout << "[4] Start Rendering..." << std::endl;
	while (!wnd->IsClose())
	{
		myUI->ImguiCreateWindow();
		//rendering
		wnd->Update(0.0f, 0.34f, 0.57f);

////--------------------------------------------------------------------
#pragma region renderscene
		
		glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(glm::radians((float)glfwGetTime() * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		teapotshader->Use();
		teapotshader->SetMat4("M", rot * ModelMatrix);
		teapotshader->SetMat4("V", ViewMatrix);
		teapotshader->SetMat4("P", ProjMatrix);
		teapotshader->SetVec3("cameraPosition", myCamera->Position);
        teapotshader->SetFloat("fact", myUI->m_fact);
        teapotshader->SetBool("b_lam", myUI->b_lam);
        teapotshader->SetFloat("aperture", myUI->m_apterture);
        teapotshader->SetFloat("shutterSpeed", myUI->m_shutterspeed);
        teapotshader->SetFloat("sensitivity", myUI->m_sensitivity);
		teapotshader->SetFloat("innerLevel", myUI->m_innerLevel);
		teapotshader->SetFloat("outerLevel", myUI->m_outerLevel);
    
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, height);

		for (int i = 0; i < sizeof(lightPosition) / sizeof(lightPosition[0]); i++)
		{
			glm::vec3 newPos = lightPosition[i] + glm::vec3(sin(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			newPos = lightPosition[i]; 
			teapotshader->SetVec3("lightPosition[" + std::to_string(i) + "]", newPos);
			teapotshader->SetVec3("lightColor[" + std::to_string(i) + "]", lightColor[i]);

			mySphere->Draw(teapotshader, false, false);
		}

		
#pragma endregion renderscene
		
	
//--------------------------------------------------------------------
		myUI->ImguiRender();

		wnd->SwapBuffer();
		wnd->PollEvents();
	}

	wnd->Close();
	return 0;
}




