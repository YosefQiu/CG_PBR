#include "Shader.h"



int Shader::GetShaderFromFile(const char* vertexPath, const char* fragmentPath, std::string* vertexCode, std::string* fragmentCode)
{
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        // read files t stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file
        vShaderFile.close();
        fShaderFile.close();
        // stream 2 string
        *vertexCode   = vShaderStream.str();
        *fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "[SHADER]ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return -1;
    }
    return 0;
}

int Shader::LinkShader(const char* vShaderCode, const char* fShaderCode)
{
    // 1. compile shader
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    
    //2. shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    //3. delete shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return 0;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. get vs and fs from file
    std::string vertexCode;
    std::string fragmentCode;
    
    if(GetShaderFromFile(vertexPath, fragmentPath, &vertexCode, &fragmentCode))
    {
        return;
    }
    std:: cout << "[SHADER] SUCCESS TO READ FILE..." << std::endl;

    //2. link shader program
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    memcpy(&(this->vShaderCode), &vShaderCode, sizeof(vShaderCode));
    memcpy(&(this->fShaderCode), &fShaderCode, sizeof(fShaderCode));


    if(LinkShader(vShaderCode, fShaderCode))
    {
        return;
    }
    std:: cout << "[SHADER] SUCCESS TO LINKING SHADER PROGRAM..." << std::endl;
}

Shader::~Shader()
{}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::ReCompileShader()
{
	if (LinkShader(vShaderCode, fShaderCode))
	{
		return;
	}
	std::cout << "[SHADER] SUCCESS TO RECOMPILE SHADER PROGRAM..." << std::endl;
}



void Shader::SetBool(const std::string &name, bool value) const
{
    glUniform1i(GetUniform(name), value);;
}

void Shader::SetInt(const std::string &name, int value) const
{
    glUniform1i(GetUniform(name), value);
}

void Shader::SetFloat(const std::string &name, float value) const
{
    glUniform1f(GetUniform(name), value);
}

void Shader::SetVec2(const std::string &name, float x, float y) const
{
    glUniform2f(GetUniform(name), x, y);
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    SetVec2(name, value.x, value.y);
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
    glUniform3f(GetUniform(name), x, y, z);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    SetVec3(name, value.x, value.y, value.z);
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
    glUniform4f(GetUniform(name), x, y, z, w);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    SetVec4(name, value.x, value.y, value.z, value.w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &value) const
{
    glUniformMatrix2fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &value) const
{
    glUniformMatrix3fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    glUniformMatrix4fv(GetUniform(name), 1, GL_FALSE, &value[0][0]);
}


void Shader::CheckCompileErrors(unsigned int shader, string type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << "[SHADER]ERROR::SHADER_COMPILATION_ERROR OF TYPE: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << "[SHADER]ERROR::PROGRAM_LINKING_ERROR OF TYPE: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
}

int Shader::GetUniform(const std::string &name) const
{
    int position = glGetUniformLocation(ID, name.c_str());
    if (position == -1)
    {
        std::cout << "[SHADER]ERROR::UNIFORM OF NAME: " << name << " SET FAILED..." << "\n -- --------------------------------------------------- --" <<std::endl;
    }
    return position;
}
