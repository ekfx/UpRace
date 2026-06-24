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
    Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit.png", 1, 0);
    
    Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart.png", 1, 0);
    KartID = Objects.CreateOneObject(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, glm::vec3(5.5f), glm::vec4(1.0f));
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
    Kart.model = glm::mat4(1.0f);
    Kart.model = glm::scale(Kart.model, glm::vec3(0.025f));


    if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
        Objects.MoveObject(ENGINE::AXIS::Y_AXIS, ENGINE::DIRECTION::FORWARD, Delta, KartID);

        std::cerr << Objects.ObjectArray.at(KartID).position.y << std::endl;
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_S)) {

        Objects.MoveObject(ENGINE::AXIS::Y_AXIS, ENGINE::DIRECTION::BACKWARD, Delta, KartID);

        std::cerr << Objects.ObjectArray.at(KartID).position.y << std::endl;
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
        KartBox.ang -= 2.50f*Delta;
        
        std::cerr << "GLFW_KEY_D\n";
    } else if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
        KartBox.ang += 2.50f*Delta;

        std::cerr << "GLFW_KEY_A\n";
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_K)) {
        CircuitAng += 1*Delta;
        std::cerr << CircuitAng << std::endl;
    }
}

void UpRace::ProcessPhysics(f32 Delta) 
{
    glm::mat4 tempMat;
    tempMat = glm::mat4(1.0f);

    tempMat = glm::rotate(tempMat, KartBox.ang, glm::vec3(0.0f, 0.0f, 1.0f));
    tempMat = glm::translate(tempMat, Objects.ObjectArray.at(KartID).position);

    Kart.model = tempMat;
    
    Kart.model = glm::translate(Kart.model, Objects.ObjectArray.at(KartID).position);
    //tempMat = glm::rotate(tempMat, CircuitAng, glm::vec3(0.0f, 0.0f, 1.0f));

}

void UpRace::Processing() 
{
    ekCamera.Start();
    ekCamera.SetProjection(ENGINE::CAMERA::ORTHOGRAPHIC, GetWindowWidth(), GetWindowHeight());
    ekCamera.Translate(0.0f, 0.0f, -1.0f);
    //ekCamera.Zoom(0.05f);
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
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", Kart.model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void UpRace::Release() 
{
    Floor.mesh.ReleaseBuffers();
    Floor.tex.ReleaseTexture();
    GlobalShader.ReleaseShaders();
}