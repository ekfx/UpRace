#include "UpRace.h"

// bool StartMenu() {
// 	return true;	// true means to start
// }

void UpRace::InitEnvironment() 
{
    SetBackgroundColor(ENGINE::WINDOW::BLACK_BACKGROUND);
    SetVSync(false);	// issue: kart delta
    /*
      Ideia atual:
        ler .ini que contenha todo caminho para textura e tenha definicoes para 
        criacao do jogo.
    */

    
    std::cout << err(Settings.Read(std::filesystem::path("../settings/settings.ini").make_preferred())) << std::endl;
    
    std::cout << err(Settings.Read(Environment.at(Settings.GetValueN("env_num").value()).make_preferred())) << std::endl;
	std::cout << err(Settings.GetValueN("actorSize"), actorSize) << std::endl;
	std::cout << err(Settings.GetValueN("range_checkpoint"), range_checkpoint) << std::endl;
    std::cout << err(Settings.GetValueN("spaceMaxH"), spaceMaxH) << std::endl;
    std::cout << err(Settings.GetValueN("spaceMaxH"), spaceMaxW) << std::endl;
    std::cout << err(Settings.GetValueC("background"), background) << std::endl;
    std::cout << err(Settings.GetValueC("circuit"), circuit) << std::endl;
    std::cout << err(Settings.GetValueC("bitmask"), bitmask) << std::endl;
	float initial_x;
	float initial_y;
	std::cout << err(Settings.GetValueN("initialX"), initial_x) << std::endl;
	std::cout << err(Settings.GetValueN("initialY"), initial_y) << std::endl;
	
	float final_x;
	float final_y;
	std::cout << err(Settings.GetValueN("final_x"), final_x) << std::endl;
	std::cout << err(Settings.GetValueN("final_y"), final_y) << std::endl;
	checkpoint = glm::vec2(final_x, final_y);
	
	lap = Settings.GetValueN("laps").value();
	
	
    for (size_t i = 0; i < Actors.size(); i++) {
		KartActor.push_back({});
		KartData.push_back({});
    }
	
	// Warning: I know this is wrong. the reason of I still use .value() having
	// the possible chance of dont have the value is because I implemented it
	// later of the start of this project, in the next, I'll use more err().
	size_t i = 0;
	for (auto& member : KartActor) {
		// Actors -> needs to clean before read other, else it will get incorrect data
		std::cout << err(Settings.Read(Actors.at(i).make_preferred())) << std::endl;

		member.SetInitialPos(initial_x, initial_y);
		std::cout << Settings.GetValueN("initialX").value() << " - " << Settings.GetValueN("initialY").value() << std::endl;
		member.SetConfig(Settings.GetMapN(), std::filesystem::path(Settings.GetValueC("Skin").value()).make_preferred().string());
		Settings.Clean();

		std::cout << member.GetRPM() << std::endl;
		std::cout << member.GetSkin() << std::endl;
		
		i++;
	}
    
}

void UpRace::Initialize() 
{
    GlobalShader.CreateBasicShaders("../shaders/shaders.vert", "../shaders/shaders.frag");
    GlobalMesh.CreateMesh(MyVertex, sizeof(MyVertex), MyEBO, sizeof(MyEBO), 1, 1, 0, 1);

    Floor.tex.CreateBasicTexture(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR, background.string().c_str(), 1, 0);
    Circuit.tex.CreateBasicTexture(GL_TEXTURE1, GL_TEXTURE_2D, GL_LINEAR, circuit.string().c_str(), 1, 0);
	
	size_t i = 0;
	for (auto& member : KartData) {
		member.tex.CreateBasicTexture(GL_TEXTURE2 + i, GL_TEXTURE_2D, GL_LINEAR, std::filesystem::path(KartActor.at(i).GetSkin()).make_preferred().string().c_str(), 1, 0);	
		i++;
	}


    BlackMap.LoadImage(bitmask.string().c_str(), false);

    ekSound.Init();
    
    ekHUD.Start(GetWindowHandle());
	/*ekHUD.SetCustomStyleFunction([&](){
		ImGuiStyle& Style = ImGui::GetStyle();
		float aspect_ratio 	= GetWindowHeight() / GetWindowWidth();
		float pos_height 	= 20.0f;
		float pos_width 	= 20.0f;
		float height 		= 15.0f;
		float width 		= 15.0f;

		ImGui::SetNextWindowPos(ImVec2((GetWindowHeight()/(aspect_ratio)*pos_width), (GetWindowWidth()/(aspect_ratio)*pos_height)));
		ImGui::SetNextWindowSize(ImVec2((GetWindowWidth()/(aspect_ratio*width)), (GetWindowHeight()/(aspect_ratio*height))));
		Style.FontScaleMain = 1.25f;
	});*/

    ekHUD.SetCustomRenderFunction([&](){
        ImGui::Begin("UpRace", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
        ImGui::Text("Circuit ---------------");
        ImGui::Text("Laps: %d", lap);
		ImGui::Text("Winner: %d", indexWinner);
        ImGui::Text("");
		size_t j = 1;
        for (auto& member : KartActor) {
			ImGui::Text("Kart %d ---------------", (i32)j);
			for (size_t k = 0; k < member.GetVel()*15; k++) {
				ImGui::TextColored(ImVec4(member.GetVel()*10, 1.0f-(member.GetVel()*k), 0.5f-member.GetVel(), 1.0f), "#");
				ImGui::SameLine();
			}
			ImGui::Text("");
			ImGui::Text("%.1f KM/H", member.GetVel()*165);
			// ImGui::Text("%d RPM", (i32)(member.GetRPM()*12)); // velocity working but rpm no? what
			ImGui::Text("%d Kg", (i32)(member.GetWeight()));
			ImGui::Text("Lap: %d", member.lap);
			ImGui::Text("");

			j++;
		}
        ImGui::End();
    });
}

void UpRace::Input(GLFWwindow* window, f32 Delta) 
{
	size_t i = 0;
    /*
      Proximas ideias: posso pegar um bitmask e criar zonas 
      de velocidade para quando carro passar e for cor amarela por exemplo
      ele ganhe velocidade.
    */

	if (Keyboard::isKeyPressed(GLFW_KEY_ENTER)) {
		std::cout << "#####################\n";
		std::cout << "x =" + std::to_string(KartActor.at(0).GetX()) + "\n";
		std::cout << "y =" + std::to_string(KartActor.at(0).GetY()) + "\n";
	}

    if (BlackMap.isPixelBlack(KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_W)) {
            KartActor.at(i).Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_S)) {
    		KartActor.at(i).Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
		KartActor.at(i).Freeze();	
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_A)) {
        KartActor.at(i).Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_D)) {
        KartActor.at(i).Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_SPACE)) {
    	KartActor.at(i).ResetPos();
    }
   
    KartActor.at(i).Run(ENGINE::DIRECTION::NOTHING, Delta);
    KartActor.at(i).Turn(ENGINE::DIRECTION::NOTHING, Delta);

    /////////////////////////////////////////////////////////////
	i++;
    
    if (BlackMap.isPixelBlack(KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_UP)) {
              KartActor.at(i).Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_DOWN)) {
          KartActor.at(i).Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
      KartActor.at(i).Freeze();
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_LEFT)) {
       KartActor.at(i).Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_RIGHT)) {
        KartActor.at(i).Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_END)) {
      KartActor.at(i).ResetPos();
    }
    
    KartActor.at(i).Run(ENGINE::DIRECTION::NOTHING, Delta);
    KartActor.at(i).Turn(ENGINE::DIRECTION::NOTHING, Delta);

    /////////////////////////////////////////////////////////////
	i++;
    
    if (BlackMap.isPixelBlack(KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_KP_8)) {
              KartActor.at(i).Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_KP_5)) {
          KartActor.at(i).Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
      KartActor.at(i).Freeze();
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_KP_4)) {
       KartActor.at(i).Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_KP_6)) {
        KartActor.at(i).Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_KP_0)) {
      KartActor.at(i).ResetPos();
    }
    
    KartActor.at(i).Run(ENGINE::DIRECTION::NOTHING, Delta);
    KartActor.at(i).Turn(ENGINE::DIRECTION::NOTHING, Delta);

	/////////////////////////////////////////////////////////////
	i++;
    
    if (BlackMap.isPixelBlack(KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::X_AXIS, spaceMaxW, BlackMap.GetWidth()), 
                              KartActor.at(i).GetRelativeCoord(ENGINE::AXIS::Y_AXIS, spaceMaxH, BlackMap.GetHeight()))) {
      if (Keyboard::isKeyPressed(GLFW_KEY_I)) {
              KartActor.at(i).Run(ENGINE::DIRECTION::FORWARD, Delta);
      } else if (Keyboard::isKeyPressed(GLFW_KEY_K)) {
          KartActor.at(i).Run(ENGINE::DIRECTION::BACKWARD, Delta);
      }
    } else {
      KartActor.at(i).Freeze();
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_J)) {
       KartActor.at(i).Turn(ENGINE::DIRECTION::FORWARD, Delta);
    } else if (Keyboard::isKeyPressed(GLFW_KEY_L)) {
        KartActor.at(i).Turn(ENGINE::DIRECTION::BACKWARD, Delta);
    }

    if (Keyboard::isKeyPressed(GLFW_KEY_0)) {
      KartActor.at(i).ResetPos();
    }
    
    KartActor.at(i).Run(ENGINE::DIRECTION::NOTHING, Delta);
    KartActor.at(i).Turn(ENGINE::DIRECTION::NOTHING, Delta);

    
}

void UpRace::ProcessPhysics(f32 Delta) 
{
	int i = 0;
	for (auto& member : KartActor) {
		member.UpdateData(Delta);
		if (ekTimer.UpdateTick(ekClock, 5000)) {
			if (member.CheckAABB({checkpoint.x - range_checkpoint, checkpoint.x + range_checkpoint},
								 {checkpoint.y - range_checkpoint, checkpoint.y + range_checkpoint})) {
				member.lap++;
			}
		}

		if (KartActor.at(i).lap == lap) {
			indexWinner = i;
		}
		
		i++;
	}


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

	size_t i = 0;
	for (auto& member : KartData) {
		glBindTexture(GL_TEXTURE_2D, member.tex.GetTextureID());
		GlobalShader.SetUniform<ENGINE::SHADER::SIMPLE_VALUE, i32>("TexSlot", 2 + i);
		GlobalShader.SetUniform<ENGINE::SHADER::MATRIX_4, glm::mat4>("model", glm::scale(KartActor.at(i).GetModel(), glm::vec3(actorSize)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		i++;
	}

    ekHUD.Run(GetWindowWidth(), GetWindowHeight(), 0);
}

void UpRace::Release() 
{
    for (auto& member : KartData) {
		member.tex.ReleaseTexture();
	}
    Circuit.tex.ReleaseTexture();
    Floor.tex.ReleaseTexture();
    GlobalShader.ReleaseShaders();
    GlobalMesh.ReleaseBuffers();
    BlackMap.ReleaseData();
    ekSound.Release();
    ekHUD.Release();
}
