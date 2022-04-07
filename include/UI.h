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
		ap = 1;
		sen = 2;
		shutterspeed = 3;
	};
	virtual ~UI() {};
public:
	void ImguiInit();
	void ImguiCreateWindow();
	void ImguiRender();
private:
	void ImguiMenuBar();
	void ImguiOpenDlg();
	void ImguiMaterialDlg();
	void ImguiLightDlg();
	void ImguiCameraDlg();
	void ImguiDisneyDlg();
	void ImguiStatsDlg();
public:
	
	bool b_showWindow;
	GLFWwindow* pWind;
	string m_filePath;
public:
	bool b_showAppControls = true;
	bool b_showAppMaterial = true;
	bool b_showAppStats = true;
	bool b_showAppLight = true;
	bool b_showAppCamera = true;
	bool b_showOpenDiag = false;
	bool b_showAppDisney = false;
	bool b_dirFlag = false;
	bool b_pointFlag = true;
	bool b_spotFlag = false;
	bool b_areaFlag = true;
	bool b_Wireframe = false;
	bool b_lam = false;
	bool rotationEnabled = false;
	bool wireframeEnabled = false;
	bool lightEnabled = false;
	float displacementAmount = 0.05f;
	MaterialMapPreview::Type hoveredPreviewItem = MaterialMapPreview::NONE;
	glm::vec3 m_dlclr;
	glm::vec3 m_plclr;
	glm::vec3 m_slclr;
	glm::vec3 m_alclr;
	glm::vec3 m_camPos;
	glm::vec3 m_camTarget;
	glm::vec3 m_camWorldup;
	ImVec4 m_dirLColor;
	ImVec4 m_pointLColor;
	ImVec4 m_spotLColor;
	ImVec4 m_areaColr;
	float m_radius;
	float m_fact;
	float m_rectWidth_Height[2];
	int ap;
    int shutterspeed;
    int sen;
    float m_apterture = 1.4;
    float m_shutterspeed = 1.0f / 30.0f;
    float m_sensitivity = 25600.0f;
    float m_innerLevel = 2.0f;
    float m_outerLevel = 1.0f;
};