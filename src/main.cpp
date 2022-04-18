#include "ggl.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "UI.h"
#include "Config.h"
#include "FrameBufferObject.h"
#include "Scene.h"
//#include "stb_image.h"
#include "IBL.h"


string hdr_path = "../res/pic/IBL/Walk_Of_Fame/Mans_Outside_2k.hdr";

#define SCR_WIDTH		1280
#define SCR_HEIGHT		720
const char* SCR_WND_NAME = "Project";

unique_ptr<Window>	wnd		 = NULL;
Camera*	myCamera = NULL;
unique_ptr<UI>		myUI	 = NULL;
Model*	mySphere			 = NULL;
Model*  myClothes			 = NULL;
Model*  myNoise              = NULL;
Shader* teapotshader		 = NULL;
Shader* clothesshader		 = NULL;
Shader* noiseshader          = NULL;

Shader* equirectangularToCubemapShader = NULL;
Shader* irradianceShader = NULL;
Shader* backgroundShader = NULL;
Shader* prefilterShader = NULL;
Shader* brdfShader = NULL;

IBL* myIbl = NULL;

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
	myUI->ImguiUpdateCamera(myCamera);
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		myCamera->SpeedX = -1.0f;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		myCamera->SpeedX = 1.0f;
	else
		myCamera->SpeedX = 0.0f;
	myCamera->CameraUpdatePos();
	myUI->ImguiUpdateCamera(myCamera);
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		myCamera->SpeedY = 1.0f;
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		myCamera->SpeedY = -1.0f;
	else
		myCamera->SpeedY = 0.0f;
	myCamera->CameraUpdatePos();
	myUI->ImguiUpdateCamera(myCamera);
#pragma endregion cameraview
#pragma endregion cameraview

    
}
#pragma endregion interactive


void RenderMain();



unsigned int brdfLUTTexture;
unsigned int prefilterMap;
unsigned int irradianceMap;
unsigned int envCubemap;

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
    myNoise = new Model(obj_filename); 
#pragma endregion loadmodel

	glDepthFunc(GL_LEQUAL);


#pragma region shader
	teapotshader = new Shader("../res/shader/vert.glsl", "../res/shader/frag3.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");	
	mySphere->SetShader(teapotshader);
	clothesshader = new Shader("../res/shader/vert.glsl", "../res/shader/brdf.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");
	myClothes->SetShader(clothesshader);
	noiseshader = new Shader("../res/shader/vert.glsl", "../res/shader/noise.glsl", NULL, "../res/shader/tctl.glsl", "../res/shader/teva.glsl");
    myNoise->SetShader(noiseshader);

	//IBL Shader
	equirectangularToCubemapShader = new Shader("../res/shader/equirectangular_to_cubemap_vert.glsl", "../res/shader/equirectangular_to_cubemap_frag.glsl", NULL, NULL, NULL);
	irradianceShader = new Shader("../res/shader/cubemap_vert.glsl", "../res/shader/irradiance_conv_frag.glsl", NULL, NULL, NULL);
	prefilterShader = new Shader("../res/shader/cubemap_vert.glsl", "../res/shader/prefilter_frag.glsl", NULL, NULL, NULL);
	brdfShader = new Shader("../res/shader/brdf_vert.glsl", "../res/shader/brdf_frag.glsl", NULL, NULL, NULL);
	backgroundShader = new Shader("../res/shader/skybox_vert.glsl", "../res/shader/skybox_frag.glsl", NULL, NULL, NULL);
	std::vector<Shader*> iblshader = { equirectangularToCubemapShader,
	irradianceShader, prefilterShader, brdfShader, backgroundShader };

#pragma endregion shader

#pragma region loadscene
	myScene = new Scene();
	myScene->LoadScene("Sphere", mySphere);
	myScene->LoadScene("Clothes", myClothes);
    myScene->LoadScene("Noise", myNoise);
#pragma endregion loadscene

	myIbl = new IBL(myUI->m_iblPath);
	myIbl->SetShader(iblshader);

	teapotshader->Use();
	teapotshader->SetInt("irradianceMap", 6);
	teapotshader->SetInt("prefilterMap", 7);
	teapotshader->SetInt("brdfLUT", 8);

	myIbl->GetShader("backgroundShader")->Use();
	myIbl->GetShader("backgroundShader")->SetInt("environmentMap", 0);

#pragma region loadcamera
    // Create Camera
	myCamera = new Camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	myUI->ImguiUpdateCamera(myCamera);
#pragma endregion loadcamera

	myIbl->Init();
	
#pragma region modelmatrix
	// Calc the MVP matrix
	ViewMatrix = myCamera->GetViewMatrix();
	ModelMatrix = glm::mat4(1.0f);
    ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
#pragma endregion modelmatrix

	backgroundShader->Use();
	backgroundShader->SetMat4("P", ProjMatrix);

	int srcWidth, srcHeight;
	glfwGetFramebufferSize(wnd->getWnd(), &srcWidth, &srcHeight);
	glViewport(0, 0, srcWidth, srcHeight);
	std::cout << "[4] Start Rendering..." << std::endl;
	while (!wnd->IsClose())
	{
		myUI->ImguiCreateWindow();
		//rendering
		wnd->Update(0.0f, 0.34f, 0.57f);
////--------------------------------------------------------------------
#pragma region renderscene
		
		myScene->Render(RenderMain);
		myIbl->GetShader("backgroundShader")->Use();
		myIbl->GetShader("backgroundShader")->SetMat4("V", myCamera->GetViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myIbl->m_envCubemap);
		//glBindTexture(GL_TEXTURE_2D, irradianceMap);
		myIbl->RenderCube();

		// find imgui api
		//ImGui::ShowDemoWindow();

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
	if (myUI->b_ibl)
	{
		myIbl->SetHdrFilepath(myUI->m_iblPath);
		myIbl->Init();
		myUI->b_ibl = false;
	}
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
		myScene->GetModel("Sphere")->GetShader()->Use();
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myIbl->m_irradianceMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_CUBE_MAP, myIbl->m_prefilterMap);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, myIbl->m_brdfLUTTexture);

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
    if (myUI->m_matType == CODE)
    {
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::rotate(glm::radians((float)glfwGetTime() * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        myScene->GetModel("Noise")->SetMVP(rot * ModelMatrix, ViewMatrix, ProjMatrix);
        myScene->GetModel("Noise")->GetShader()->SetVec2("u_resolution", glm::vec2(1280.0f, 720.0f));
        myScene->GetModel("Noise")->GetShader()->SetFloat("u_time", (float)glfwGetTime());
        myScene->GetModel("Noise")->SetImguiParameter(noiseshader, myUI.get());
        myScene->DrawScene("Noise", false, false);
    }
}
