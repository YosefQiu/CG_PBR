#include "Scene.h"

Scene::~Scene()
{
	if (!m_scene.empty())
	{
		m_scene.clear();
	}
}

void Scene::LoadScene(std::string modelName, Model* model)
{
	m_scene.insert(std::pair<std::string, Model*>(modelName, model));
}

void Scene::DrawScene(std::string modelName,bool b_mat, bool b_tex)
{
	Model* tmpModel = NULL;
	auto iter = m_scene.find(modelName);
	if (iter != m_scene.end())
	{
		tmpModel = iter->second;
	}
	else
		return;
	tmpModel->Draw(tmpModel->m_shader, b_mat, b_tex);
}

void Scene::Render(FunType pFunc)
{
	pFunc();
}

Model* Scene::GetModel(std::string modelName)
{
	auto iter = m_scene.find(modelName);
	if (iter != m_scene.end())
	{
		return iter->second;
	}
	else
		return NULL;
}
