#pragma once
#include "ggl.h"
#include "Model.h"
typedef void(*FunType)();
class Scene
{
public:
	Scene() {}
	virtual ~Scene();
public:
	void LoadScene(std::string modelName, Model* model);
	void DrawScene(std::string modelName, bool b_mat, bool b_tex);
	void Render(FunType pFunc);
	Model* GetModel(std::string modelName);
public:
	std::map<std::string, Model*> m_scene;
	
};

