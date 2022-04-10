#pragma once
#include "ggl.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"

namespace MaterialMapPreview {
	enum Type { ALBEDO, NORMAL, METALLIC, ROUGHNESS, AO, DISPLACEMENT, NONE };
}
enum MaterialType {PICTURE, CODE, PARAMETER};
class Camera;
class UI
{
public:
	UI(GLFWwindow* pWind)
	{
		
		this->pWind = pWind;
		hoveredPreviewItem = MaterialMapPreview::NONE;
		m_dirLColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		m_pointLColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		m_spotLColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		m_areaColr = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
		m_radius = 0.2355;
		m_rectWidth_Height[0] = 0.5;
		m_rectWidth_Height[1] = 0.44;
	};
	virtual ~UI() {};
public:
	void ImguiInit();
	void ImguiCreateWindow();
	void ImguiRender();
	void ImguiUpdateCamera(Camera* cam);
	void ImguiUpdateTexture(std::map<std::string, unsigned int> mapTexnameToTexid);
private:
	void ImguiMenuBar();
	void ImguiOpenDlg();
	void ImguiControlDlg();
	void ImguiMaterialDlg();
	void ImguiLightDlg();
	void ImguiCameraDlg();
	void ImguiDisneyDlg();
	void ImguiStatsDlg();
public:
	
	bool b_showWindow;
	GLFWwindow* pWind;
	
public:
	bool b_showAppControls = true;
	bool b_showAppMaterial = true;
	bool b_showAppStats = true;
	bool b_showAppLight = false;
	bool b_showAppCamera = true;
	bool b_showOpenDiag = true;
	bool b_showAppDisney = true;

	bool b_dirFlag = false;
	bool b_pointFlag = true;
	bool b_spotFlag = false;
	bool b_areaFlag = true;
	
	bool b_lam = false;
	bool rotationEnabled = false;
	
	bool lightEnabled = false;
	float displacementAmount = 0.05f;
	MaterialMapPreview::Type hoveredPreviewItem = MaterialMapPreview::NONE;

	glm::vec3 m_dlclr;
	glm::vec3 m_plclr;
	glm::vec3 m_slclr;
	glm::vec3 m_alclr;
//** ------------
//** camera UI parameter
//** ------------
	std::string m_filePath;
	bool b_fileChange = false;
	bool b_fileFirst = true;


//** ------------
//** camera UI parameter
//** ------------
	glm::vec3 m_camPos;
	glm::vec3 m_camTarget;
	glm::vec3 m_camWorldup;
	float m_camYaw = 0.0f;
	float m_camPitch = 0.0f;
	float m_apterture = 1.4;
	float m_shutterspeed = 1.0f / 30.0f;
	float m_sensitivity = 25600.0f;
	int m_camApterture = 1;
	int m_camShutterspeed = 2;
	int m_camSensitivity = 3;

//** ------------
//** material UI parameter
//** ------------
	std::map<std::string, unsigned int> m_mapTexnameToTexid;
	float m_matAlbedo[3] = {0.0f, 0.0f, 0.0f};
	float m_matSheenColor[3] = { 0.0f, 0.0f, 0.0f };
	float m_matMetallic = 0.5f;
	float m_matRoughness = 0.5f;
	float m_matAo = 0.5f;
	MaterialType m_matType = PICTURE;

//** ------------
//** light UI parameter
//** ------------
	ImVec4 m_dirLColor;
	ImVec4 m_pointLColor;
	ImVec4 m_spotLColor;
	ImVec4 m_areaColr;
	float m_radius;
	float m_rectWidth_Height[2];
	
//** ------------
//** model UI parameter
//** ------------
	bool b_wireframeEnabled = false;
	float m_heightFactor = 1.0f;
    float m_innerLevel = 2.0f;
    float m_outerLevel = 1.0f;

//** ------------
//** Disney Principled BRDF parameter
//** ------------
	glm::vec3 m_disneyBaseColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float m_disneySubsurface = 0.0f;
	float m_disneyMetallic = 0.0f;
	float m_disneySpecular = 0.0f;
	float m_disneySpecularTint = 0.0f;
	float m_disneyRoughness = 0.0f;
	float m_disneyAnisotropic = 0.0f;
	float m_disneySheen = 0.0f;
	float m_disneySheenTint = 0.0f;
	float m_disneyClearcoat = 0.0f;
	float m_disneyClearcoatGloss = 0.0f;
};