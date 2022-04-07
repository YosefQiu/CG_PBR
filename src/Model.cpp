#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Model::Model(std::string path)
{
	float floatMax = numeric_limits<float>::max();
	float floatMin = numeric_limits<float>::min();
	MinVec = glm::vec3(floatMax, floatMax, floatMax);
	MaxVec = glm::vec3(floatMin, floatMin, floatMin);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	LoadModel(path);
	SetBoundingBox();
}

void Model::Draw(Shader* shader, bool b_mat, bool b_tex)
{
	for(int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader,b_mat, b_tex);
}

void Model::SetLight(Shader* shader, LightDirection* lightD /*= NULL*/, LightPoint* lightP /*= NULL*/, LightSpot* lightS /*= NULL*/)
{
	shader->Use();
	shader->SetVec3("AmbientClr", glm::vec3(0.3f, 0.3f, 0.3f));
	shader->SetVec3("CameraPos", glm::vec3(0.0f, 0.0f, 100.0f));
	/*shader->SetVec3("LightPos", glm::vec3(MaxVec.x, MaxVec.y, MaxVec.z));
	shader->SetVec3("LightClr", glm::vec3(1.0f, 1.0f, 1.0f));*/
	if (lightD)
		shader->SetVec3("AmbientClr", 0.3f * lightD->color);
		SetLightD(shader, lightD);
	if (lightP)
		SetLightP(shader, lightP);
	if (lightS)
		SetLightS(shader, lightS);
}

glm::vec3 Model::GetCameraPos()
{
	return camera_pos;
}

glm::vec3 Model::GetCameraTarget()
{
	return camera_target;
}

glm::vec3 Model::GetModelCenter()
{
	return center;
}

void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[ASSIMP]::ERROR::Assimp ERROR..." << std::endl;
		return;
	}
	else
	{
		std::cout << "[ASSIMP]::Success load model..." << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
	
 }

unsigned int Model::LoadTexture(const char* path, const string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
		{
			format = GL_RED;
		}
		else if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "FAILED TO LOAD TEXTURE..." << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}

std::vector<MyTexture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<MyTexture> tepTextures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (int j = 0; j < textures.size(); j++)
		{
			if (std::strcmp(textures[j].path.data(), str.C_Str()) == 0)
			{
				tepTextures.push_back(textures[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			MyTexture texture;
			texture.id = LoadTexture(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			tepTextures.push_back(texture);
			textures.push_back(texture);
		}
	}
	return tepTextures;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(curMesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<MyVertex> tepVerteices;
	std::vector<MyTexture> tepTextures;
	std::vector<unsigned int> tepIndices;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		MyVertex tepVertex;

		tepVertex.Position.x = mesh->mVertices[i].x;
		tepVertex.Position.y = mesh->mVertices[i].y;
		tepVertex.Position.z = mesh->mVertices[i].z;

		tepVertex.Normal.x = mesh->mNormals[i].x;
		tepVertex.Normal.y = mesh->mNormals[i].y;
		tepVertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			tepVertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
			tepVertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
			
		}
		else
		{
			tepVertex.TexCoord = glm::vec2(0.0f, 0.0f);
		}

		tepVerteices.push_back(tepVertex);
	}

	for (int i = 0; i <mesh->mNumFaces; i++)
	{
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			tepIndices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<MyTexture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	tepTextures.insert(tepTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<MyTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	tepTextures.insert(tepTextures.end(), specularMaps.begin(), specularMaps.end());
	
	MyMaterial mat;
	aiColor3D color;
	//read mtl
	material->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat.Ka = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	mat.Kd = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_COLOR_SPECULAR, color);
	mat.Ks = glm::vec3(color.r, color.g, color.b);
	material->Get(AI_MATKEY_SHININESS, mat.Ns);


	return Mesh(tepVerteices, tepIndices, tepTextures, mat);
}

void Model::SetBoundingBox()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].vertices.size(); j++)
		{
			if (meshes[i].vertices[j].Position.x < MinVec.x)
				MinVec.x = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.y < MinVec.y)
				MinVec.y = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.z < MinVec.z)
				MinVec.z = meshes[i].vertices[j].Position.z;

			if (meshes[i].vertices[j].Position.x > MaxVec.x)
				MaxVec.x = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.y > MaxVec.y)
				MaxVec.y = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.z > MaxVec.z)
				MaxVec.z = meshes[i].vertices[j].Position.z;
		}
	}
	
	center = glm::vec3((MinVec + MaxVec) / 2.0f);
	float dist = (MaxVec - MinVec).z * 2;
	camera_pos = glm::vec3(0, 0, dist);
	camera_target = glm::vec3(center - camera_pos);
}

void Model::SetLightD(Shader* shader, LightDirection* lightD)
{
	shader->SetVec3("lightD.color", lightD->color);
	shader->SetVec3("lightD.dirToLight", lightD->direction);
}

void Model::SetLightP(Shader* shader, LightPoint* lightP)
{
	shader->SetVec3("lightP.color", lightP->color);
	shader->SetVec3("lightP.pos", lightP->position);
	shader->SetFloat("lightP.constant", lightP->constant);
	shader->SetFloat("lightP.linear", lightP->linear);
	shader->SetFloat("lightP.quadratic", lightP->quadratic);
}

void Model::SetLightS(Shader* shader, LightSpot* lightS)
{
	shader->SetVec3("lightS.color", lightS->color);
	shader->SetVec3("lightS.pos", lightS->position);
	shader->SetVec3("lightS.dirToLight", lightS->direction);
	shader->SetFloat("lightS.constant", lightS->constant);
	shader->SetFloat("lightS.linear", lightS->linear);
	shader->SetFloat("lightS.quadratic", lightS->quadratic);
	shader->SetFloat("lightS.cosInnerPhy", lightS->cosInnerPhy);
	shader->SetFloat("lightS.cosOutterPhy", lightS->cosOutterPhy);
}
