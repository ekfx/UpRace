#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include "Types.h"

class Shader {
private:
    /////////////////////////////////////////////////////////////////////////////////
    // Shaders
    std::stringstream vertexShader; // Codigo
    const char* vShaderSource;
    u32 vShaderID;

    std::stringstream fragmentShader; // Codigo
    const char* fShaderSource;
    u32 fShaderID;

    std::stringstream shaderProgram; // Codigo
    u32 ShaderProgID;

    /////////////////////////////////////////////////////////////////////////////////
    // Uniforms
    std::map<const char*, GLint> UniformArray;

    /////////////////////////////////////////////////////////////////////////////////
    // Status
    std::stringstream error;
    int success;
    char infoLog[512];
    
public:
    /////////////////////////////////////////////////////////////////////////////////
    // Constructor and destructor
    Shader();
    ~Shader();

    /////////////////////////////////////////////////////////////////////////////////
    // Functions
    void ProcessVertexShader(std::string path);
    void CompileVertexShader(const char* vertexShaderSourceCode);
    u32 GetVertexShaderID();
    std::string GetVertexShader();

    void ProcessFragmentShader(std::string path);
    void CompileFragmentShader(const char* fragmentShaderSourceCode);
    u32 GetFragmentShaderID();
    std::string GetFragmentShader();

    void AttachShaderProgram();
    void LinkShaderProgram();
    void ReleaseShaders();
    void ReleaseShaderProgram();
    void Use(i32 unbind = 1);
    u32 GetShaderProgramID();
    std::string GetShaderProgram();

    void CreateBasicShaders(std::string path_vertex = "../shaders/shaders.vert", std::string path_fragment = "../shaders/shaders.frag");

    /////////////////////////////////////////////////////////////////////////////////
    // Status
    std::string GetError();
    i32 GetSuccessStatus();
    char* GetInfoLog();

    /////////////////////////////////////////////////////////////////////////////////
    // Set Uniforms
    void SetEssentialUniforms(glm::mat4 view, glm::mat4 projection, glm::mat4 model);
    void SetFloat(const char* name_var, float value);
    void SetInt(const char* name_var, int value);
    void SetBool(const char*  name_var, bool value);
    void SetVec2Float(const char* name_var, float value_1, float value_2);
    void SetVec3Float(const char* name_var, float value_1, float value_2, float value_3);
    void SetVec4Float(const char* name_var, float value_1, float value_2, float value_3, float value_4);
    void SetVec2Int(const char* name_var, int value_1, int value_2);
    void SetVec3Int(const char* name_var, int value_1, int value_2, int value_3);
    void SetVec4Int(const char* name_var, int value_1, int value_2, int value_3, int value_4);
    void SetMat3(const char* name_var, glm::mat3 matrix_3);
    void SetMat4(const char* name_var, glm::mat4 matrix_4);

    /////////////////////////////////////////////////////////////////////////////////
    // Getters
    u32 GetUniformLocation(const char* name_uniform);
};

#endif //SHADER_H