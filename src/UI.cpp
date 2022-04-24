#include "UI.h"
#include "Camera.h"
void UI::ImguiInit()
{
	// create and bind ImGui's CONTEXT
	const char* glsl_version = "#version 150";/*{{{*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// set imgui style
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(pWind, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void UI::ImguiCreateWindow()
{
	// create imgui window
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImguiMenuBar();
	ImguiOpenDlg();
	ImguiControlDlg();
	ImguiMaterialDlg();
	ImguiLightDlg();
	ImguiCameraDlg();
	ImguiDisneyDlg();
	ImguiStatsDlg();

	
}

void UI::ImguiRender()
{
	//Render imgui
	ImGui::Render();/*{{{*/
	int display_w, display_h;
	glfwMakeContextCurrent(pWind);
	glfwGetFramebufferSize(pWind, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());/*}}}*/
}

void UI::ImguiUpdateCamera(Camera* cam)
{
	m_camPos = cam->Position;
	m_camTarget = cam->Target;
	m_camWorldup = cam->WorldUp;
	m_camPitch = cam->Pitch;
	m_camYaw = cam->Yaw;
}

void UI::ImguiUpdateTexture(std::map<std::string, unsigned int> mapTexnameToTexid)
{
	m_mapTexnameToTexid = mapTexnameToTexid;
}

void UI::ImguiMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu")) 
		{
			ImGui::MenuItem("Controls", NULL, &b_showAppControls);
			ImGui::MenuItem("Material", NULL, &b_showAppMaterial);
			ImGui::MenuItem("Light", NULL, &b_showAppLight);
			ImGui::MenuItem("Camera", NULL, &b_showAppCamera);
			ImGui::MenuItem("Stats", NULL, &b_showAppStats);
			ImGui::MenuItem("Disney", NULL, &b_showAppDisney);
			
			if (ImGui::MenuItem("Quit", "Alt+F4")) {
				glfwSetWindowShouldClose(pWind, true);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open", NULL, &b_showOpenDiag);
			ImGui::MenuItem("Save");
			if (b_showOpenDiag)
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", nullptr, ".");
				
			}
			

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("Version");

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void UI::ImguiOpenDlg()
{
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			m_filePath = filePathName;
			string::size_type pos = 0;
			string sub_str = "\\";
			while ((pos = m_filePath.find(sub_str)) != string::npos)
			{
				m_filePath.replace(pos, sub_str.length(), "/");
			}
			b_fileChange = true;
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void UI::ImguiControlDlg()
{
	// Control window
	if (b_showAppControls)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 220), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(950, 20), ImGuiCond_FirstUseEver);
		ImGui::Begin("Control", &b_showAppControls, ImGuiWindowFlags_HorizontalScrollbar);
		static int style_idx = 0;
		const char* label = "Window Style";
		if (ImGui::Combo(label, &style_idx, "Dark\0Light\0Classic\0"))
		{
			switch (style_idx)
			{
			case 0: ImGui::StyleColorsDark(); break;
			case 1: ImGui::StyleColorsLight(); break;
			case 2: ImGui::StyleColorsClassic(); break;
			}
		}
		if (ImGui::RadioButton("Wire frame mode", b_wireframeEnabled))
		{
			b_wireframeEnabled = !b_wireframeEnabled;
			if (b_wireframeEnabled)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::SliderFloat("InnerLevel", &m_innerLevel, 1.0, 64.0);
		ImGui::SliderFloat("OuterLevel", &m_outerLevel, 1.0, 64.0);
		ImGui::SliderFloat("Height factor", &m_heightFactor, 0.0, 2.0);

		ImGui::End();
	}
}

void UI::ImguiMaterialDlg()
{
	// Material window
	if (b_showAppMaterial) 
	{
		ImGui::SetNextWindowSize(ImVec2(670, 230), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 28), ImGuiCond_FirstUseEver);
		ImGui::Begin("Material", &b_showAppMaterial, ImGuiWindowFlags_HorizontalScrollbar);
		if (ImGui::TreeNode("Picture..."))
		{
			if (ImGui::Button("Change to current material type"))
			{
				m_matType = PICTURE;
			}
			ImGui::BeginGroup();
			ImGui::Text("    Albedo");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["albedo"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::ALBEDO;
			}

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text("    Normal");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["normal"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::NORMAL;
			}

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text("   Metallic");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["metallic"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::METALLIC;
			}

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text("   Roughness");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["roughness"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped)/* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::ROUGHNESS;
			}

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text("      AO");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["ao"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::AO;
			}

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::Text(" Displacement");
			ImGui::Image((ImTextureID)m_mapTexnameToTexid["height"], ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem |
				ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
			{
				ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				r.Expand(3.5f);
				ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
				hoveredPreviewItem = MaterialMapPreview::DISPLACEMENT;
			}

			ImGui::EndGroup();

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("Code..."))
		{
			if (ImGui::Button("Change to current material type"))
			{
				m_matType = CODE;
			}
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("Parameter..."))
		{
			if (ImGui::Button("Change to current material type"))
			{
				m_matType = PARAMETER;
			}
			ImGui::SliderFloat3("ALBEDO", m_matAlbedo, 0.0, 1.0);
			ImGui::SliderFloat3("SHEENCOLOR", m_matSheenColor, 0.0, 1.0);
			ImGui::SliderFloat("METALLIC", &m_matMetallic, 0.0, 1.0);
			ImGui::SliderFloat("ROUGHNESS", &m_matRoughness, 0.0, 1.0);
			ImGui::SliderFloat("AO", &m_matAo, 0.0, 1.0);
			

			ImGui::TreePop();
			ImGui::Separator();
		}
		ImGui::End();
	}

}

void UI::ImguiLightDlg()
{
	if (b_showAppLight)
	{
		ImGui::SetNextWindowSize(ImVec2(345, 365), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 275), ImGuiCond_FirstUseEver);
        
        
		
		ImGui::Begin("Light", &b_showAppLight, ImGuiWindowFlags_None);
		if (ImGui::TreeNode("Area Light..."))
		{
			ImGui::SetNextItemWidth(120);
			if (ImGui::RadioButton("Diffuse", b_lam))
			{
				b_lam = !b_lam;
			}
			ImGui::Checkbox("AreaLight", &b_areaFlag); 
			static bool ref_color = false;
			static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
			if(ImGui::ColorPicker4("MyColor##4", (float*)&m_areaColr, 
				ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar, 
				ref_color ? &ref_color_v.x : NULL))
				m_alclr = glm::vec3(m_areaColr.x, m_areaColr.y, m_areaColr.z);
			ImGui::SliderFloat("Circle Radius", &m_radius, 0.0, 1.0);
			ImGui::SliderFloat2("Width & Height", m_rectWidth_Height, 0.0, 1.0);
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("Theory Light..."))
		{
			if (ImGui::ColorEdit3("Direction Light Color", (float*)&m_dirLColor))
				m_dlclr = glm::vec3(m_dirLColor.x, m_dirLColor.y, m_dirLColor.z);
			if (ImGui::ColorEdit3("Point Light Color", (float*)&m_pointLColor))
				m_plclr = glm::vec3(m_pointLColor.x, m_pointLColor.y, m_pointLColor.z);
			if (ImGui::ColorEdit3("Spot Light Color", (float*)&m_spotLColor))
				m_slclr = glm::vec3(m_spotLColor.x, m_spotLColor.y, m_spotLColor.z);
			ImGui::SetNextItemWidth(120);
			ImGui::Checkbox("DirectionLight", &b_dirFlag); ImGui::SameLine();
			ImGui::Checkbox("PointLight", &b_pointFlag); ImGui::SameLine();
			ImGui::Checkbox("SpotLight", &b_spotFlag);
			
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("Environment Light..."))
		{
			if (ImGui::TreeNode("Environment map..."))
			{
				

				ImGui::TreePop();
				ImGui::Separator();
			}
			if (ImGui::TreeNode("cube map..."))
			{


				ImGui::TreePop();
				ImGui::Separator();
			}
			if (ImGui::TreeNode("IBL(Image base light)..."))
			{
				static int style_idx = 0;
				const char* label = "IBL";
				if (ImGui::Combo(label, &style_idx, "None\0Walk Of Fame\0PaperMill\0Arches\0NewportLoft\0"))
				{
					switch (style_idx)
					{
						case 0: b_ibl = false; break;
						case 1: 
						{
							b_ibl = true;
							b_iblFirst = true;
							m_iblPath = "../res/pic/IBL/Walk_Of_Fame/Mans_Outside_2k.hdr";
						}
						break;
						case 2: 
						{
							b_iblFirst = true;
							b_ibl = true;
							m_iblPath = "../res/pic/IBL/PaperMill_Ruins_A/PaperMill_A_3k.hdr";
						}
						break;
						case 3:
						{
							b_iblFirst = true;
							b_ibl = true;
							m_iblPath = "../res/pic/IBL/Arches_E_PineTree/Arches_E_PineTree_3k.hdr";
						}
						break;
						case 4:
						{
							b_iblFirst = true;
							b_ibl = true;
							m_iblPath = "../res/pic/IBL/Newport_Loft/Newport_Loft_Ref.hdr";
						}
						break;
					}
				}

				ImGui::TreePop();
				ImGui::Separator();
			}

			ImGui::TreePop();
			ImGui::Separator();
		}

		ImGui::End();
	}
}

void UI::ImguiCameraDlg()
{
	if (b_showAppCamera)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 220), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(950, 495), ImGuiCond_FirstUseEver);
		ImGui::Begin("Camera", &b_showAppCamera, ImGuiWindowFlags_None);
		ImGui::SetNextItemWidth(120);
		ImGui::Text("Camera Position\n [%f \t %f \t %f]", m_camPos.x, m_camPos.y, m_camPos.z);
		ImGui::Text("Camera Yaw [%f]\nCamera Pitch[%f]\n", m_camYaw, m_camPitch);
		ImGui::Text("Camera WorldUp\n [%f \t %f \t %f]", m_camWorldup.x, m_camWorldup.y, m_camWorldup.z);
		ImGui::SliderInt("Aperture", &m_camApterture, 1, 8);
		switch (m_camApterture)
		{
		case 1:
			m_apterture = 1.4f; break;
		case 2:
			m_apterture = 2.0f; break;
		case 3:
			m_apterture = 2.8f; break;
		case 4:
			m_apterture = 4.0f; break;
		case 5:
			m_apterture = 5.6f; break;
		case 6:
			m_apterture = 8.0f; break;
		case 7:
			m_apterture = 11.0f; break;
		case 8:
			m_apterture = 16.0f; break;
		default:
			m_apterture = 1.4f; break;
		}
		ImGui::SliderInt("Shutter Speed", &m_camShutterspeed, 1, 3);
		switch (m_camShutterspeed)
		{
		case 1:
			m_shutterspeed = 1 / 30.0f; break;
		case 2:
			m_shutterspeed = 1 / 60.0f; break;
		case 3:
			m_shutterspeed = 1 / 200.0f; break;
		default:
			m_shutterspeed = 1 / 30.0f; break;
		}
		ImGui::SliderInt("Sensitivity", &m_camSensitivity, 1, 4);
		switch (m_camSensitivity)
		{
		case 1:
			m_sensitivity = 800.0f; break;
		case 2:
			m_sensitivity = 6400.0f; break;
		case 3:
			m_sensitivity = 12800.0f; break;
		case 4:
			m_sensitivity = 25600.0f; break;
		default:
			m_sensitivity = 25600.0f; break;
		}
		ImGui::End();
	}
	
}

void UI::ImguiDisneyDlg()
{
	if (b_showAppDisney)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 245), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(950, 245), ImGuiCond_FirstUseEver);
		ImGui::Begin("Disney", &b_showAppDisney, ImGuiWindowFlags_None);
		ImGui::SetNextItemWidth(120);
		if (ImGui::Button("Change to current material type"))
		{
			m_matType = DISNEY;
		}
		ImGui::SliderFloat3("base color", m_disneyBaseColor, 0.0, 1.0);
		
		ImGui::SliderFloat("Subsurface", &m_disneySubsurface, 0.0, 1.0);
		ImGui::SliderFloat("Metallic", &m_disneyMetallic, 0.0, 1.0);
		ImGui::SliderFloat("Specular", &m_disneySpecular, 0.0, 1.0);
		ImGui::SliderFloat("SpecularTint", &m_disneySpecularTint, 0.0, 1.0);
		ImGui::SliderFloat("Roughness", &m_disneyRoughness, 0.0, 1.0);
		ImGui::SliderFloat("Anisotropic", &m_disneyAnisotropic, 0.0, 1.0);
		ImGui::SliderFloat("Sheen", &m_disneySheen, 0.0, 1.0);
		ImGui::SliderFloat("SheenTint", &m_disneySheenTint, 0.0, 1.0);
		ImGui::SliderFloat("Clearcoat", &m_disneyClearcoat, 0.0, 1.0);
		ImGui::SliderFloat("ClearcoatGloss", &m_disneyClearcoatGloss, 0.0, 1.0);

		ImGui::End();
	}
}

void UI::ImguiStatsDlg()
{
	if (b_showAppStats) 
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos = ImVec2(3.0f, io.DisplaySize.y - 3.0f);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        
		if (ImGui::Begin("Example: Simple overlay", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs))
		{
			ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

			if (ImGui::IsMousePosValid())
            {
                m_statsMousePosX = io.MousePos.x;
                m_statsMousePosY = io.MousePos.y;
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			}
			else {
				ImGui::Text("Mouse Position: <invalid>");
			}
		}
		ImGui::End();
	}
}

