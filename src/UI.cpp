#include "UI.h"
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
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
				
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
		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void UI::ImguiMaterialDlg()
{
	// Material window
	if (b_showAppMaterial) 
	{
		ImGui::SetNextWindowSize(ImVec2(670, 158), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 28), ImGuiCond_FirstUseEver);
		ImGui::Begin("Material", &b_showAppMaterial, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::BeginGroup();
		ImGui::Text("    Albedo");
		ImGui::Image((ImTextureID)1, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | 
			ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */ )
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
		ImGui::Image((ImTextureID)2, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
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
		ImGui::Image((ImTextureID)3, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

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
		ImGui::Image((ImTextureID)4, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

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
		ImGui::Image((ImTextureID)5, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

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
		ImGui::Image((ImTextureID)6, ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | 
			ImGuiHoveredFlags_AllowWhenOverlapped) /* && dropTarget.AcceptFormat() */)
		{
			ImRect r(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
			r.Expand(3.5f);
			ImGui::GetWindowDrawList()->AddRect(r.Min, r.Max, IM_COL32(255, 255, 0, 255), 0.0f, ~0, 2.0f);
			hoveredPreviewItem = MaterialMapPreview::DISPLACEMENT;
		}

		ImGui::EndGroup();

		ImGui::End();
	}

}

void UI::ImguiLightDlg()
{
	if (b_showAppLight)
	{
		ImGui::SetNextWindowSize(ImVec2(670, 158), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 188), ImGuiCond_FirstUseEver);
        if (ImGui::Button("Wireframe mode"))
        {
            b_Wireframe = !b_Wireframe;
            if(b_Wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if (ImGui::RadioButton("Diffuse", b_lam))
        {
            b_lam = !b_lam;
        }
        ImGui::SliderFloat("InnerLevel", &m_innerLevel, 1.0, 64.0);
        ImGui::SliderFloat("OuterLevel", &m_outerLevel, 1.0, 64.0);
        ImGui::SliderFloat("Height fact", &m_fact, 1.0, 2.0);
        ImGui::SliderInt("Aperture", &ap, 1, 8);
        switch(ap)
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
        ImGui::SliderInt("Shutter Speed", &shutterspeed, 1, 3);
        switch(shutterspeed)
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
        ImGui::SliderInt("Sensitivity", &sen, 1, 4);
        switch(sen)
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
		ImGui::Begin("Light", &b_showAppLight, ImGuiWindowFlags_None);
		if (ImGui::TreeNode("Area Light..."))
		{
			ImGui::SetNextItemWidth(120);
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
		ImGui::SetNextWindowSize(ImVec2(670, 158), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 215), ImGuiCond_FirstUseEver);
		ImGui::Begin("Camera", &b_showAppCamera, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::SetNextItemWidth(120);
		ImGui::Text("Camera Position\n [%f \t %f \t %f]", m_camPos.x, m_camPos.y, m_camPos.z);
		ImGui::Text("Camera Target\n [%f \t %f \t %f]", m_camTarget.x, m_camTarget.y, m_camTarget.z);
		ImGui::Text("Camera WorldUp\n [%f \t %f \t %f]", m_camWorldup.x, m_camWorldup.y, m_camWorldup.z);

		ImGui::End();
	}
	
}

void UI::ImguiDisneyDlg()
{
	if (b_showAppDisney)
	{
		ImGui::SetNextWindowSize(ImVec2(670, 158), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(1100, 515), ImGuiCond_FirstUseEver);
		ImGui::Begin("Disney", &b_showAppDisney, ImGuiWindowFlags_None);
		ImGui::SetNextItemWidth(120);

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

			if (ImGui::IsMousePosValid()) {
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			}
			else {
				ImGui::Text("Mouse Position: <invalid>");
			}
		}
		ImGui::End();
	}
}

