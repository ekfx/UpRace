#include "UpRace.h"

void UpRace::InitEnvironment() 
{
    SetBackgroundColor(ENGINE::WINDOW::BLACK_BACKGROUND);
    // SetVSync(false);
    /*
      Ideia atual:
        ler .ini que contenha todo caminho para textura e tenha definicoes para 
        criacao do jogo.
    */
}

void UpRace::Initialize() 
{
    GlobalShader.CreateBasicShaders("shaders/shaders.vert", "shaders/shaders.frag");
    GlobalMesh.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);

    Floor.tex.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, "../assets/floor_.png", 1, 0);
    Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, "../assets/circuit1.png", 1, 0);
    
    KartData.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, "../assets/kart1.png", 1, 0);

    BlackMap.LoadImage("../assets/black_map1.png", false);

    Kart.Start(0.65f, 0.0f);
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
    /*
      We have: a X value that is in our coordinate system,
      that runs from -1,50 to +1,50, and we need to convert 
      to the imagem system that goes from 0 to 4096 in height 
      and width (so x and y are the same).

      Following the formula: Xc = C + ((Xo - A) * (D - C))/B - A
      Being:
        A - the old minimum
        B - the old maximum 
        C - the new minimum
        D - the new maximum

      so, changing their values:
        Xc = -1.50 + ((Xo - 0) * (+1.50 (-1.50))) / 4096 - 0
      Being Xc -> X converted and Xo -> X old.

    */
    // f32 PCoorX = -1.50f + ((Kart.GetX() - 0.0f) * (1.50f - (-1.50f))) / 4096.0f - 0;
    // f32 PCoorY = -1.50f + ((Kart.GetY() - 0.0f) * (1.50f - (-1.50f))) / 4096.0f - 0;
 
    i32 PCoorX = ((Kart.GetX() + 1.00f) * 4096) / 2.0f;
    i32 PCoorY = ((Kart.GetY() + 1.00f) * 4096) / 2.0f;

    if (BlackMap.isPixelBlack(PCoorX, PCoorY)) {
      if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
              Kart.Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
          Kart.Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }

      if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
         Kart.Turn(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
          Kart.Turn(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    }

    Kart.Run(ENGINE::DIRECTION::NOTHING, Delta);
    Kart.Turn(ENGINE::DIRECTION::NOTHING, Delta);
}

void UpRace::ProcessPhysics(f32 Delta) 
{
    Kart.UpdateData(Delta);
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
    // Warning: to compare black_map to the circuit needs to zoom 2x too! Or no.
}

void UpRace::Render() 
{
    // Data
    glBindVertexArray(GlobalMesh.GetVAO());

    // Apenas tres chamadas de renderizacao
    glBindTexture(GL_TEXTURE_2D, Floor.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 0);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", Floor.model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
 
    glBindTexture(GL_TEXTURE_2D, Circuit.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 1);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", Circuit.model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    
    glBindTexture(GL_TEXTURE_2D, KartData.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 2);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(Kart.GetModel(), glm::vec3(0.005f)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

void UpRace::Release() 
{
    KartData.tex.ReleaseTexture();
    Circuit.tex.ReleaseTexture();
    Floor.tex.ReleaseTexture();
    GlobalShader.ReleaseShaders();
    GlobalMesh.ReleaseBuffers();
    BlackMap.ReleaseData();
}
