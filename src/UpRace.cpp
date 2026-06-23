#include "UpRace.h"

void UpRace::InitEnvironment() 
{
    GlobalShader.CreateBasicShaders();
    floorM.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);
    floorT.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart.png", 1, 1);
    GlobalShader.Use();
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, int>("actor", 1);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", floorMd);
}

void UpRace::Initialize() 
{
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
}

void UpRace::ProcessPhysics(f32 Delta) 
{
}

void UpRace::Processing() 
{
}

void UpRace::Render() 
{
}

void UpRace::Release() 
{
}