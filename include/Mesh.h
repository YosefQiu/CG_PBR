#pragma once
#include "ggl.h"
#include "Shader.h"
struct MyVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct MyTexture
{
	unsigned int id;
	std::string type;
	std::string path;
};

struct MyMaterial 
{
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float     Ns;
};

class Mesh
{
public:
	std::vector<MyVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MyTexture> textures;
	MyMaterial mats;
public:
	Mesh(std::vector<MyVertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<MyTexture> textures);
	Mesh(std::vector<MyVertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<MyTexture> textures,
		MyMaterial mat);
	~Mesh() {}
public:
	void Draw(Shader* shader, bool b_mat, bool b_tex);
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
private:
	void SetupMesh();
};

