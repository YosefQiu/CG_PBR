#include "Camera.h"

Camera::Camera (glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Target = target;
    
	Forward = glm::normalize(Position - Target);
	Right = glm::cross(glm::normalize(WorldUp), Forward);
	Right = glm::normalize(Right);
	Up = glm::cross(Forward, Right);
	Up = glm::normalize(Up);


	const glm::vec3 yUnit(0, 1, 0);
	const glm::vec3 xUnit(1, 0, 0);
    const glm::vec3 zUnit(0, 0, 1);

    float pitch = glm::degrees(-glm::asin(glm::dot(Forward, yUnit)));
    glm::vec3 forward = Forward;
    forward.y = 0;
    forward = glm::normalize(forward);
    float yaw = glm::degrees(glm::acos(glm::dot(forward, xUnit)));
    if (glm::dot(forward, zUnit) > 0)
        yaw = 360 - yaw;
    Yaw = glm::radians(yaw);
    Pitch = glm::radians(pitch);
    CameraPitchYaw(Position, Pitch, Yaw, WorldUp);
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
    CameraPitchYaw(position, pitch, yaw, worldup);

}

void Camera::CameraPitchYaw(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup)
{
    Position = position;
    WorldUp = worldup;
    Pitch = pitch;
    Yaw = yaw;

    Forward.x = glm::cos(pitch) * glm::sin(yaw);
    Forward.y = glm::sin(pitch);
    Forward.z = glm::cos(pitch) * glm::cos(yaw);
    Forward = glm::normalize(Forward);

    Right = glm::cross(Forward, glm::normalize(WorldUp));
    Right = glm::normalize(Right);
    Up = glm::cross(Right, Forward);
    Up = glm::normalize(Up);
    //Forward.z = -1.0f * Forward.z;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Forward, WorldUp);
    //return LookAt();
}

glm::mat4 Camera::LookAt()
{
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rotat = glm::mat4(1.0f);
    
    trans[3][0] = -Position.x;
    trans[3][1] = -Position.y;
    trans[3][2] = -Position.z;
    rotat[0][0] = Right.x;
    rotat[1][0] = Right.y;
    rotat[2][0] = Right.z;
    rotat[0][1] = Up.x;
    rotat[1][1] = Up.y;
    rotat[2][1] = Up.z;
    rotat[0][2] = Forward.x;
    rotat[1][2] = Forward.y;
    rotat[2][2] = Forward.z;
    
    return rotat * trans;
}

void Camera::CameraUpdatePos()
{
    Position += Forward * SpeedZ * 0.1f + Right * SpeedX * 0.1f + Up * SpeedY * 0.1f;
}


void Camera::CameraUpdateVec()
{
    Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
    Forward.y = glm::sin(Pitch);
    Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
    Forward = glm::normalize(Forward);

    Right = glm::cross(Forward, glm::normalize(WorldUp));
    Right = glm::normalize(Right);
    Up = glm::cross(Right, Forward);
    Up = glm::normalize(Up);
    //Forward.z = -1.0f * Forward.z;
}

void Camera::ProcessMouseMovementPitch(float deltaX, float deltaY)
{
    deltaX *= glm::radians(0.1f);
    deltaY *= glm::radians(0.1f);

    Pitch += deltaX;
    Yaw += deltaY;
    if (Pitch > 89.0f)
        Pitch = 89.9f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
    //CameraPitchYaw(Position, Pitch, Yaw, WorldUp);
    CameraUpdateVec();
}

void Camera::ProcessMouseMovementYaw(float deltaX, float deltaY)
{
    deltaX *= glm::radians(0.1f);
    deltaY *= glm::radians(0.1f);

    Yaw += deltaX;
    //CameraPitchYaw(Position, Pitch, Yaw, WorldUp);
    CameraUpdateVec();
}