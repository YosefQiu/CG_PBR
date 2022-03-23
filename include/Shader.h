#pragma once
#include "ggl.h"
#include "Light.h"

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
public:
    unsigned int ID;
public:
    void Use();  
    void ReCompileShader();
    //Set Uniform
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetVec4(const std::string &name, const glm::vec4 &value) const;
    void SetVec4(const std::string &name, float x, float y, float z, float w) const;
    void SetMat2(const std::string &name, const glm::mat2 &value) const;
    void SetMat3(const std::string &name, const glm::mat3 &value) const;
    void SetMat4(const std::string &name, const glm::mat4 &value) const;
private:
    int GetShaderFromFile(const char* vertexPath, const char* fragmentPath, std::string* vertexCode, std::string* fragmentCode);   
    int LinkShader(const char* vShaderCode, const char* fShaderCode);   
    int GetUniform(const string& name)const;
    void CheckCompileErrors(unsigned int shader, std::string type);  
private:
    const char* vShaderCode;
    const char* fShaderCode;
};



