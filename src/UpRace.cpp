#include "UpRace.h"

void UpRace::InitEnvironment() 
{
    SetBackgroundColor(ENGINE::WINDOW::BLUE_BACKGROUND);
}

void UpRace::Initialize() 
{
    GlobalShader.CreateBasicShaders();
    GlobalMesh.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);

    Floor.tex.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, "../assets/floor_.png", 1, 0);
    Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit2.png", 1, 0);
    
    Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart.png", 1, 0);
    KartID = Objects.CreateOneObject(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(5.5f), glm::vec4(1.0f));
    Kart.vel = 0.0f;
    Kart.velAng = 0.0f;
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{

    if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
        PrettyKart.Run(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
        PrettyKart.Run(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
        PrettyKart.Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
        PrettyKart.Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    PrettyKart.Run(ENGINE::DIRECTION::NOTHING, Delta);
    PrettyKart.Turn(ENGINE::DIRECTION::NOTHING, Delta);
}

void UpRace::ProcessPhysics(f32 Delta) 
{
    PrettyKart.UpdateData(Delta);
}

void UpRace::Processing() 
{
    ekCamera.Start();
    ekCamera.SetProjection(ENGINE::CAMERA::ORTHOGRAPHIC, GetWindowWidth(), GetWindowHeight());
    ekCamera.Translate(0.0f, 0.0f, -1.0f);
    //ekCamera.Zoom(0.25f);

    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("view", ekCamera.GetCameraMat4());
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("projection", ekCamera.GetProjectionMat4());
    
    Floor.model = glm::mat4(1.0f);
    Floor.model = glm::scale(Floor.model, glm::vec3(4.0f));
    
    Circuit.model = glm::mat4(1.0f);
    Circuit.model = glm::scale(Circuit.model, glm::vec3(2.0f));
}

void UpRace::Render() 
{
    // Data
    glBindVertexArray(GlobalMesh.GetVAO());

    glBindTexture(GL_TEXTURE_2D, Floor.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 0);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", Floor.model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
 
    glBindTexture(GL_TEXTURE_2D, Circuit.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 1);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", Circuit.model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    
    glBindTexture(GL_TEXTURE_2D, Kart.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 2);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(PrettyKart.GetModel(), glm::vec3(0.10f)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void UpRace::Release() 
{
    Kart.tex.ReleaseTexture();
    Circuit.tex.ReleaseTexture();
    Floor.tex.ReleaseTexture();
    GlobalShader.ReleaseShaders();
    GlobalMesh.ReleaseBuffers();
}