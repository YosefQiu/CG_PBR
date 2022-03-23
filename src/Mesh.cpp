#include "Mesh.h"

Mesh::Mesh(std::vector<MyVertex> vertices, std::vector<unsigned int> indices, std::vector<MyTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	SetupMesh();
}

Mesh::Mesh(std::vector<MyVertex> vertices, std::vector<unsigned int> indices, std::vector<MyTexture> textures, MyMaterial mat)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->mats = mat;
	SetupMesh();
}

void Mesh::Draw(Shader* shader, bool b_mat, bool b_tex)
{
	if (b_mat)
	{
		shader->SetVec3("mat.Ka", mats.Ka);
		shader->SetVec3("mat.Kd", mats.Kd);
		shader->SetVec3("mat.Ks", mats.Ks);
		shader->SetFloat("mat.Ns", mats.Ns);
	}
	
	if (b_tex)
	{
		for (int i = 0; i < textures.size(); i++)
		{
			if (textures[i].type == "texture_diffuse")
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				shader->SetInt("tex.diffuse", 0);
			}
			if (textures[i].type == "texture_specular")
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				shader->SetInt("tex.specualr", 1);
			}
		}
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
	
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(0 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), (void*)(6 * sizeof(GL_FLOAT)));

	glBindVertexArray(0);
}
