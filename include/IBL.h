#pragma once
#include "ggl.h"
#include "Shader.h"
class IBL
{
public:
	IBL(std::string hdrFilePath);
public:
	void Init();
	void SetHdrFilepath(std::string hdrFilePath);
	void SetShader(std::vector<Shader*> shader);
	void SetViewAndProj();
	void SetEnvCubeMap();
	void SetHDRToCubeMap();
	void SetIrradianceMap();
	void SetPrefilterMap();
	void SetBrdfLUTMap();

	void RenderCube();
	void RenderQuad();
	Shader* GetShader(std::string shaderName);
	unsigned int GetTextureMap(std::string textureName);
public:
	std::map<std::string, Shader*> m_mapNameToShader;

	std::string m_hdrFilePath;

	glm::mat4 m_captureProjection;
	std::vector<glm::mat4> m_captureViews;

	unsigned int m_captureFBO;
	unsigned int m_captureRBO;

	unsigned int m_brdfLUTTexture;
	unsigned int m_prefilterMap;
	unsigned int m_irradianceMap;
	unsigned int m_envCubemap;
	unsigned int m_hdrTexture;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
private:
	unsigned int LoadHDREnvironment(std::string hdrFilePaht);
};

