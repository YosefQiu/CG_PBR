#pragma once
#include "ggl.h"

class Camera
{
public:
    Camera (glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
    Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
    virtual ~Camera () {};
public:
    glm::mat4 GetViewMatrix();
    glm::mat4 LookAt();
    void CameraUpdatePos();
    void CameraUpdateVec();
    void CameraPitchYaw(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
    void ProcessMouseMovementPitch(float deltaX, float deltaY);
    void ProcessMouseMovementYaw(float deltaX, float deltaY);
public:
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Forward;
    glm::vec3 Right;
    glm::vec3 Up;
    glm::vec3 WorldUp;
    float Pitch;
    float Yaw;
	float SpeedX = 0.0f;
	float SpeedY = 0.0f;
	float SpeedZ = 0.0f;
    float Zoom = 45.0f;
};
