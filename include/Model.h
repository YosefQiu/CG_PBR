#pragma once
#include "ggl.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Light.h"

class Model
{
public:
	std::vector<Mesh> meshes;
	std::vector<MyTexture> textures;
	std::string directory;
	glm::vec3 MinVec;
	glm::vec3 MaxVec;
	glm::vec3 center;
	glm::vec3 camera_pos;
	glm::vec3 camera_target;
public:
	Model(std::string path);
	~Model() {}
public:
	void Draw(Shader* shader, bool b_mat, bool b_tex);
	void SetLight(Shader* shader, 
		LightDirection* lightD = NULL, 
		LightPoint* lightP = NULL, 
		LightSpot* lightS = NULL);
	glm::vec3 GetCameraPos();
	glm::vec3 GetCameraTarget();
	glm::vec3 GetModelCenter();
private:
	void LoadModel(std::string path);
	unsigned int LoadTexture(const char* path, const string& directory);
	std::vector<MyTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void SetBoundingBox();
	void SetLightD(Shader* shader, LightDirection* lightD);
	void SetLightP(Shader* shader, LightPoint* lightP);
	void SetLightS(Shader* shader, LightSpot* lightS);
	void SetTexture(Shader* shader);
};

