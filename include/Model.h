#pragma once
#include "ggl.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Light.h"
#include "UI.h"

class Model
{
public:
	std::vector<Mesh> meshes;
	std::vector<MyTexture> textures;
	std::string directory;
	std::vector<std::string> m_texPath;
	std::map<std::string, std::string> m_mapTexpathToTexname;
	std::map<std::string, unsigned int> m_mapTexnameToTexid;
	glm::vec3 MinVec;
	glm::vec3 MaxVec;
	glm::vec3 center;
	glm::vec3 camera_pos;
	glm::vec3 camera_target;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
	Shader* m_shader;
public:
	Model(std::string path);
	~Model() {}
public:
	void Draw(Shader* shader, bool b_mat, bool b_tex);
	void SetLight(Shader* shader, 
		LightDirection* lightD = NULL, 
		LightPoint* lightP = NULL, 
		LightSpot* lightS = NULL);
	void SetShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = NULL,
		const char* tessControlPath = NULL, const char* tessEvalPath = NULL);
	void SetShader(Shader* shader);
	void SetMVP(Shader* shader, glm::mat4 M, glm::mat4 V, glm::mat4 P);
	void SetMVP(glm::mat4 M, glm::mat4 V, glm::mat4 P);
	void SetMVP();
	void SetImguiParameter(Shader* shader, UI* myUI);
	void SetImguiParameter(UI* myUI);
	void SetTexture(std::string pathRoot);
	unsigned int GetTextureID(std::string textureType);
	void BindTexture();

	glm::vec3 GetCameraPos();
	glm::vec3 GetCameraTarget();
	glm::vec3 GetModelCenter();
	Shader* GetShader();
private:
	void LoadModel(std::string path);
	unsigned int LoadTexture(const char* path, const string& directory);
	unsigned int LoadTexture(const char* path);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void SetBoundingBox();
	void SetLightD(Shader* shader, LightDirection* lightD);
	void SetLightP(Shader* shader, LightPoint* lightP);
	void SetLightS(Shader* shader, LightSpot* lightS);
	
};
