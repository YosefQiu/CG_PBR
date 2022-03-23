#pragma once
#include "ggl.h"

//MVP matrix
glm::mat4 ModelMatrix = glm::mat4(1.0f);
glm::mat4 ViewMatrix = glm::mat4(1.0f);
glm::mat4 ProjMatrix = glm::mat4(1.0f);

glm::mat4 plane_M = glm::mat4(1.0f);
glm::mat4 plane_V = glm::mat4(1.0f);
glm::mat4 plane_P = glm::mat4(1.0f);


glm::mat4 lightD_view = glm::mat4(1.0f);
glm::mat4 lightD_proj = glm::mat4(1.0f);
glm::mat4 lightD_model = glm::mat4(1.0f);
glm::mat4 lightD_trans = glm::mat4(1.0f);

glm::mat4 lightP_view = glm::mat4(1.0f);
glm::mat4 lightP_proj = glm::mat4(1.0f);
glm::mat4 lightP_model = glm::mat4(1.0f);
glm::mat4 lightP_trans = glm::mat4(1.0f);

glm::mat4 lightS_view = glm::mat4(1.0f);
glm::mat4 lightS_proj = glm::mat4(1.0f);
glm::mat4 lightS_model = glm::mat4(1.0f);
glm::mat4 lightS_trans = glm::mat4(1.0f);

//Light
glm::vec3 AmbientColor = glm::vec3(0.3f, 0.3f, 0.3f);
//direction light
glm::vec3 LightDPos = glm::vec3(5.0f, 5.0f, 0.0f);
glm::vec3 LightDAngles = glm::vec3(0.0f, glm::radians(120.0f), 0.0f);
glm::vec3 LightDColor = glm::vec3(1.0f, 1.0f, 1.0f);


//point light
glm::vec3 LightPPos = glm::vec3(-4.0f, 0.0f, 3.0f);
glm::vec3 LightPAngles = glm::vec3(0.0f, 0.0, glm::radians(45.0f));
glm::vec3 LightPColor = glm::vec3(1.0f, 1.0f, 1.0f);


//spot light
glm::vec3 LightSPos = glm::vec3(10.0f, 10.0f, 5.0f);
glm::vec3 LightSAngles = glm::vec3(glm::radians(90.0f), 0.0f, 0.0f);
glm::vec3 LightSColor = glm::vec3(10.0f, 10.0f, 10.0f);


//Material
glm::vec3 mat_ambient = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 mat_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 mat_specular = glm::vec3(0.5f, 0.5f, 0.5f);
float mat_shineiness = 32.0f;

//mouse & keyboard
float lastX, lastY;
float times = 100.0f;
bool firstMouse = true;
bool b_Proj = true;
bool b_btnLeft;
bool b_btnRight;
bool b_keyctl;