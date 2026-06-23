#include "UpRace.h"

void UpRace::InitEnvironment() 
{
    SetBackgroundColor(ENGINE::WINDOW::BLUE_BACKGROUND);
}

void UpRace::Initialize() 
{
    GlobalShader.CreateBasicShaders();
    floorM.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);
    floorT.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart.png", 1, 1);
    GlobalShader.Use();
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, int>("actor", 1);
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
}

void UpRace::ProcessPhysics(f32 Delta) 
{
}

void UpRace::Processing() 
{
    ekCamera.Start();
    ekCamera.SetProjection(ENGINE::CAMERA::ORTHOGRAPHIC, GetWindowWidth(), GetWindowHeight());
    ekCamera.Translate(0.0f, 0.0f, -1.0f);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("view", ekCamera.GetCameraMat4());
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("projection", ekCamera.GetProjectionMat4());

    floorMd = glm::mat4(1.0f);
}

void UpRace::Render() 
{
    // target é o tipo da textura
    glBindTexture(GL_TEXTURE_2D, floorT.GetTextureID());
    glBindVertexArray(floorM.GetVAO());
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", floorMd);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void UpRace::Release() 
{
    floorM.ReleaseBuffers();
    floorT.ReleaseTexture();
    GlobalShader.ReleaseShaders();
}