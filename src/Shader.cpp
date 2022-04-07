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

int Shader::GetShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessControlPath, const char* tessEvalPath, std::string* vertexCode, std::string* fragmentCode, std::string* geometryCode, std::string* tessctrlCode, std::string* tessevalCode)
{
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    std::ifstream tCShaderFile;
    std::ifstream tEShaderFile;


	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (geometryPath != NULL)
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (tessControlPath != NULL)
        tCShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    if (tessEvalPath != NULL)
        tEShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
        if(geometryPath != NULL)
            gShaderFile.open(geometryPath);
        if(tessControlPath != NULL)
            tCShaderFile.open(tessControlPath);
        if(tessEvalPath != NULL)
            tEShaderFile.open(tessEvalPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;
        std::stringstream gShaderStream;
        std::stringstream tCShaderStream;
        std::stringstream tEShaderStream;

		// read files t stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
        if(geometryPath != NULL)
            gShaderStream << gShaderFile.rdbuf();
        if (tessControlPath != NULL)
            tCShaderStream << tCShaderFile.rdbuf();
        if (tessEvalPath != NULL)
            tEShaderStream << tEShaderFile.rdbuf();

		// close file
		vShaderFile.close();
		fShaderFile.close();
        if(geometryPath != NULL)
            gShaderFile.close();
        if (tessControlPath != NULL)
            tCShaderFile.close();
        if(tessEvalPath != NULL)
            tEShaderFile.close();

		// stream 2 string
		*vertexCode = vShaderStream.str();
		*fragmentCode = fShaderStream.str();
        if(geometryPath != NULL)
            *geometryCode = gShaderStream.str();
        if(tessControlPath != NULL)
            *tessctrlCode = tCShaderStream.str();
        if(tessEvalPath != NULL)
            *tessevalCode = tEShaderStream.str();

	}
	catch (std::ifstream::failure e)
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

int Shader::LinkShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode, const char* tCShaderCode, const char* tEShaderCode)
{
	// 1. compile shader
    unsigned int vertex, fragment, geometry, tessControl, tessEval;
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
    // geometry shader
    if (gShaderCode != NULL)
    {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
    }
    // tessControl shader
    if (tCShaderCode != NULL)
    {
		tessControl = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tessControl, 1, &tCShaderCode, NULL);
		glCompileShader(tessControl);
		CheckCompileErrors(tessControl, "TESS_CONTROL");
    }
    //tessEval shader
    if (tEShaderCode != NULL)
    {
		tessEval = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tessEval, 1, &tEShaderCode, NULL);
		glCompileShader(tessEval);
		CheckCompileErrors(tessEval, "TESS_EVALUATION");
    }
	
	//2. shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
    if(gShaderCode != NULL)
        glAttachShader(ID, geometry);
    if(tCShaderCode != NULL)
        glAttachShader(ID, tessControl);
    if(tEShaderCode != NULL)
        glAttachShader(ID, tessEval);

	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");

	//3. delete shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
    if (gShaderCode != NULL)
        glDeleteShader(geometry);
    if (tCShaderCode != NULL)
        glDeleteShader(tessControl);
    if (tEShaderCode != NULL)
        glDeleteShader(tessEval);
	return 0;
}

//Shader::Shader(const char* vertexPath, const char* fragmentPath)
//{
//    // 1. get vs and fs from file
//    std::string vertexCode;
//    std::string fragmentCode;
//    
//    if(GetShaderFromFile(vertexPath, fragmentPath, &vertexCode, &fragmentCode))
//    {
//        return;
//    }
//    std:: cout << "[SHADER] SUCCESS TO READ FILE..." << std::endl;
//
//    //2. link shader program
//    const char* vShaderCode = vertexCode.c_str();
//    const char* fShaderCode = fragmentCode.c_str();
//
//    memcpy(&(this->vShaderCode), &vShaderCode, sizeof(vShaderCode));
//    memcpy(&(this->fShaderCode), &fShaderCode, sizeof(fShaderCode));
//
//
//    if(LinkShader(vShaderCode, fShaderCode))
//    {
//        return;
//    }
//    std:: cout << "[SHADER] SUCCESS TO LINKING SHADER PROGRAM..." << std::endl;
//}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath /*= NULL*/, const char* tessControlPath /*= NULL*/, const char* tessEvalPath /*= NULL*/)
{
	// 1. get vs and fs from file
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
    std::string tessctrlCode;
    std::string tessevalCode;

	if (GetShaderFromFile(vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath,
        &vertexCode, &fragmentCode, &geometryCode, &tessctrlCode, &tessevalCode))
	{
		return;
	}
	std::cout << "[SHADER] SUCCESS TO READ FILE..." << std::endl;

	//2. link shader program
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
    const char* tmp_gShaderCode;
    const char* tmp_tCShaderCode;
    const char* tmp_tEShaderCode;
    if (geometryPath != NULL)
        tmp_gShaderCode = geometryCode.c_str();  
    if (tessControlPath != NULL)
        tmp_tCShaderCode = tessctrlCode.c_str();
    if(tessEvalPath != NULL)
        tmp_tEShaderCode = tessevalCode.c_str();

	memcpy(&(this->vShaderCode), &vShaderCode, sizeof(vShaderCode));
	memcpy(&(this->fShaderCode), &fShaderCode, sizeof(fShaderCode));
    if (geometryPath != NULL)
        memcpy(&(this->gShaderCode), &tmp_gShaderCode, sizeof(tmp_gShaderCode));
    else
        this->gShaderCode = NULL;
    if (tessControlPath != NULL)
        memcpy(&(this->tCShaderCode), &tmp_tCShaderCode, sizeof(tmp_tCShaderCode));
    else
        tCShaderCode = NULL;
    if (tessEvalPath != NULL)
        memcpy(&(this->tEShaderCode), &tmp_tEShaderCode, sizeof(tmp_tEShaderCode));
    else
        tEShaderCode = NULL;


	if (LinkShader(vShaderCode, fShaderCode, gShaderCode, tCShaderCode, tEShaderCode))
		return;
	std::cout << "[SHADER] SUCCESS TO LINKING SHADER PROGRAM..." << std::endl;
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
