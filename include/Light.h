#pragma once
#include "ggl.h"

class Light
{
public:
	Light();
	~Light();
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 angles;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f); 
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class LightDirection : public Light
{
public:
	LightDirection(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f));
	LightDirection(int n, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~LightDirection();
public:

public:
	void UpdateDirection();
};

class LightPoint : public Light
{
public:
	LightPoint(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f));
	LightPoint(int n, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~LightPoint();
public:
	float constant;
	float linear;
	float quadratic;
};

class LightSpot : public Light
{
public:
	LightSpot(glm::vec3 _position, glm::vec3 _angles, glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f));
	LightSpot(int n, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	~LightSpot();
public:
	float constant;
	float linear;
	float quadratic;
	float cosInnerPhy = 0.9f;
	float cosOutterPhy = 0.85f;
public:
	void UpdateDirection();
};
