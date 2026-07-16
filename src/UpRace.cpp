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

    
    std::cout << err(Settings.Read(std::filesystem::path("../settings/settings.ini").make_preferred())) << std::endl;
    
    std::cout << err(Settings.Read(Environment.at(Settings.GetValueN("env_num").value()).make_preferred())) << std::endl;
    std::cout << err(Settings.GetValueN("spaceMaxH"), spaceMaxH) << std::endl;
    std::cout << err(Settings.GetValueN("spaceMaxH"), spaceMaxW) << std::endl;
    std::cout << err(Settings.GetValueC("background"), background) << std::endl;
    std::cout << err(Settings.GetValueC("circuit"), circuit) << std::endl;
    std::cout << err(Settings.GetValueC("bitmask"), bitmask) << std::endl;

    // Warning: I know this is wrong. the reason of I still use .value() having
    // the possible chance of dont have the value is because I implemented it
    // later of the start of this project, in the next, I'll use more err().
    Kart0.SetInitialPos(Settings.GetValueN("initialX").value(), Settings.GetValueN("initialY").value());
    Kart1.SetInitialPos(Settings.GetValueN("initialX").value(), Settings.GetValueN("initialY").value());
    
    // Actors -> needs to clean before read other, else it will get incorrect data
    std::cout << err(Settings.Read(Actors.at(0).make_preferred())) << std::endl;
    Kart0.SetConfig(Settings.GetMapN(), std::filesystem::path(Settings.GetValueC("Skin").value()).make_preferred().string());
    std::cout << Kart0.GetSkin() << std::endl;
    Settings.Clean();
    
    std::cout << err(Settings.Read(Actors.at(1).make_preferred())) << std::endl;
    Kart1.SetConfig(Settings.GetMapN(), std::filesystem::path(Settings.GetValueC("Skin").value()).make_preferred().string());
    std::cout << Kart1.GetSkin() << std::endl;
    Settings.Clean();
    
}

void UpRace::Initialize() 
{
    GlobalShader.CreateBasicShaders("../shaders/shaders.vert", "../shaders/shaders.frag");
    GlobalMesh.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);

    Floor.tex.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, background.string().c_str(), 1, 0);
    Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, circuit.string().c_str(), 1, 0);
    
    KartData0.tex.CreateBasicTexture(GL_TEXTURE2, GL_TEXTURE_2D, GL_LINEAR, Kart0.GetSkin().c_str(), 1, 0);
    KartData1.tex.CreateBasicTexture(GL_TEXTURE3, GL_TEXTURE_2D, GL_LINEAR, Kart1.GetSkin().c_str(), 1, 0);

    BlackMap.LoadImage(bitmask.string().c_str(), false);

    ekSound.Init();
    
    ekHUD.Start(GetWindowHandle());
    ekHUD.SetCustomRenderFunction([&](){
        ImGui::Begin("UpRace", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Circuit ---------------");
        ImGui::Text("Laps: %d (Don't work yet)", 1);
        ImGui::Text("");
        ImGui::Text("Kart 1 ---------------");
        ImGui::Text("%.1f KM/H", Kart0.GetVel()*165);
        ImGui::Text("%d RPM", (i32)(Kart0.GetRPM()*12));
        ImGui::Text("%d Kg", (i32)(Kart0.GetWeight()));
        ImGui::Text("");
        ImGui::Text("Kart 2 ---------------");
        ImGui::Text("%.1f KM/H", Kart1.GetVel()*165);
        ImGui::Text("%d RPM", (i32)(Kart1.GetRPM()*12));
        ImGui::Text("%d Kg", (i32)(Kart1.GetWeight()));
        ImGui::End();
    });
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
    /*
      Proximas ideias: posso pegar um bitmask e criar zonas 
      de velocidade para quando carro passar e for cor amarela por exemplo
      ele ganhe velocidade.
    */

    if (Keyboard::isKeyPressed(GLFW_KEY_F)) {
      std::cout << err(Settings.Read(Actors.at(0).make_preferred())) << std::endl;
      Kart0.SetConfig(Settings.GetMapN(), Settings.GetValueC("Skin").value());
      std::cout << Kart0.GetSkin() << std::endl;
      Settings.Clean();
      
      std::cout << err(Settings.Read(Actors.at(1).make_preferred())) << std::endl;
      Kart1.SetConfig(Settings.GetMapN(), Settings.GetValueC("Skin").value());
      std::cout << Kart1.GetSkin() << std::endl;
      Settings.Clean();
    }

    if (BlackMap.isPixelBlack(Kart0.GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              Kart0.GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
              Kart0.Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
          Kart0.Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
      Kart0.Freeze();
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
       Kart0.Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
        Kart0.Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_SPACE)) {
      Kart0.ResetPos();
    }
   
    Kart0.Run(ENGINE::DIRECTION::NOTHING, Delta);
    Kart0.Turn(ENGINE::DIRECTION::NOTHING, Delta);

    /////////////////////////////////////////////////////////////

    
    if (BlackMap.isPixelBlack(Kart1.GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              Kart1.GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_UP)) {
              Kart1.Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_DOWN)) {
          Kart1.Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
      Kart1.Freeze();
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_LEFT)) {
       Kart1.Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_RIGHT)) {
        Kart1.Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_END)) {
      Kart1.ResetPos();
    }
    
    Kart1.Run(ENGINE::DIRECTION::NOTHING, Delta);
    Kart1.Turn(ENGINE::DIRECTION::NOTHING, Delta);

    // Kart.ShowData();

    
}

void UpRace::ProcessPhysics(f32 Delta) 
{
    Kart0.UpdateData(Delta);
    Kart1.UpdateData(Delta);

    // for (i32 i = 0; i < (i32)(Kart0.GetVel()*10); i++) {
    //   ekSound._PlaySound(std::filesystem::path("../assets/sound/engine.mp3").string().c_str());
    // };
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
    
    glBindTexture(GL_TEXTURE_2D, KartData0.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 2);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(Kart0.GetModel(), glm::vec3(0.05f)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

  ////////////////////////////////////////////

    glBindTexture(GL_TEXTURE_2D, KartData1.tex.GetTextureID());
    GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 3);
    GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(Kart1.GetModel(), glm::vec3(0.05f)));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    ekHUD.Run(GetWindowWidth(), GetWindowHeight(), 0);
}

void UpRace::Release() 
{
    KartData0.tex.ReleaseTexture();
    KartData1.tex.ReleaseTexture();
    Circuit.tex.ReleaseTexture();
    Floor.tex.ReleaseTexture();
    GlobalShader.ReleaseShaders();
    GlobalMesh.ReleaseBuffers();
    BlackMap.ReleaseData();
    ekSound.Release();
    ekHUD.Release();
}
