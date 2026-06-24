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
    Kart.vel = 0.0f;
    Kart.velAng = 0.0f;
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
    // Acceleration
    if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
        if (Kart.vel < 0.025f) Kart.vel += 0.015f*Delta;

    } else if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
        if (Kart.vel > -0.025f) Kart.vel -= 0.015f*Delta;
    } else {
        Kart.vel = Kart.vel*0.85f;
    }

    if ((Kart.vel > 0.0005f || Kart.vel < -0.0005f) && (Kart.velAng < 0.085f && Kart.velAng > -0.085f)) {    
        if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
            Kart.velAng += 0.95f*Delta;
        } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
            Kart.velAng -= 0.95f*Delta;
        } 
    } else {
        Kart.velAng = Kart.velAng*0.07f;
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_INSERT)) 
        Kart.model = glm::mat4(1.0f);

    if (Keyboard::isKeyPressed(GLFW_KEY_1)) {
        Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit3.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_2)) {
        Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_3)) {
        Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit2.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_4)) {
        Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_5)) {
        Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart2.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_6)) {
        Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart3.png", 1, 0);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_7)) {
        Kart.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/dot.png", 1, 0);
    }
}

void UpRace::ProcessPhysics(f32 Delta) 
{
    //std::cerr << "VelAng: " << Kart.velAng << " - Vel: " << Kart.vel << " - Ang: " << Kart.ang << " - Forward" << Kart.forward << std::endl;
    // std::cerr << Kart.model[0][0] << " - " << Kart.model[1][0] << " - " << Kart.model[2][0] << " - " << Kart.model[3][0] << "\n";
    // std::cerr << Kart.model[0][1] << " - " << Kart.model[1][1] << " - " << Kart.model[2][1] << " - " << Kart.model[3][1] << "\n";
    // std::cerr << Kart.model[0][2] << " - " << Kart.model[1][2] << " - " << Kart.model[2][2] << " - " << Kart.model[3][2] << "\n";
    // std::cerr << Kart.model[0][3] << " - " << Kart.model[1][3] << " - " << Kart.model[2][3] << " - " << Kart.model[3][3] << "\n";
    // std::cerr << "---------------------------------------------------------------------------------------------------------\n";
    // a coluna [3] carrega a coordenada do objeto no plano 

    std::cerr << Kart.model[3][0] << " - " << Kart.model[3][1] << std::endl;

    Kart.forward = 0;
    Kart.ang = 0;

    Kart.ang += Kart.velAng;
    Kart.forward += Kart.vel;

    float angulo = Kart.ang;
    float cateto_adj = Kart.forward;
    float cateto_oposto = cateto_adj * tan(angulo);
    float hipotenusa = sqrt(cateto_adj*cateto_adj + cateto_oposto*cateto_oposto);
    // hipotenusa é o vetor direção
    glm::vec3 triangulo_retangulo = glm::vec3(cateto_oposto, cateto_adj, 0.0f);
    position += triangulo_retangulo * Delta;

    Kart.x = Kart.model[3][0];
    Kart.y = Kart.model[3][1];


    Kart.model = glm::rotate(Kart.model, Kart.ang, glm::vec3(0.0f, 0.0f, 1.0f)); 
    Kart.model = glm::translate(Kart.model, triangulo_retangulo); 

}

void UpRace::Processing() 
{
    ekCamera.Start();
    ekCamera.SetProjection(ENGINE::CAMERA::ORTHOGRAPHIC, GetWindowWidth(), GetWindowHeight());
    ekCamera.Translate(0.0f, 0.0f, -1.0f);

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
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(Kart.model, glm::vec3(0.10f)));
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