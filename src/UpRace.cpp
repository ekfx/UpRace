#include "UpRace.h"

bool UpRace::StartMenu() {
    // Menu Data
    HUD menu;
    int ShouldStart = -1;
    i32 env_buf = -1;
    
    // Style
    std::string topName = "UpRace Main Menu";

    menu.Start(GetWindowHandle());
	
    menu.SetCustomStyleFunction([&](){
		ImGuiStyle& Style = ImGui::GetStyle();
        float aspect_ratio 	= GetWindowHeight() / GetWindowWidth();
        float WindowSize = 500.0f;
        float Width = (GetWindowWidth() / 2) - (WindowSize / 2);
        float Height = (GetWindowHeight() / 2) - (WindowSize / 2);

		ImGui::SetNextWindowPos(ImVec2(Width, Height));
		ImGui::SetNextWindowSize(ImVec2(WindowSize, WindowSize));
		Style.FontScaleMain = 2.05f;
	});

    menu.SetCustomRenderFunction([&](){
        ImGui::Begin("noname", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
            // is basically the same thing that I used to centralize the window: ((MajorWndSize - MinorWndSize) / 2)
            ImGui::SetCursorPosX(((ImGui::GetWindowSize().x) - (ImGui::CalcTextSize(topName.c_str()).x)) / 2);
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), topName.c_str());
            ImGui::Text("");

            ////////////////////////////////////////////////////////////////
            // Map

            std::string slt = "Setup: number from 0 to ";
            slt += std::to_string(Environment.size() - 1);
            
            ImGui::SetCursorPosX(((ImGui::GetWindowSize().x) - (ImGui::CalcTextSize(slt.c_str()).x)) / 2);
            ImGui::Text(slt.c_str());
            
            ImGui::InputInt("##label", &env_buf, 0, 0);

            ////////////////////////////////////////////////////////////////
            // Input system
            
            ImGui::SetCursorPosX(((ImGui::GetWindowSize().x) - (ImGui::CalcTextSize("Start").x)) / 2);
            if (ImGui::Button("Start")) {
                if (env_buf >= 0 && env_buf < Environment.size()) {
                    env_num = env_buf;
                    ShouldStart = 1;       
                } else {
                    ImGui::SetCursorPosX(((ImGui::GetWindowSize().x) - (ImGui::CalcTextSize("Select a number.").x)) / 2);
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Select a number.");    
                }
            } 
            
            ImGui::Text("");
            ImGui::SetCursorPosX(((ImGui::GetWindowSize().x) - (ImGui::CalcTextSize("Quit").x)) / 2);
            if (ImGui::Button("Quit")) {
                ShouldStart = 0;
            } 
        ImGui::End();
    });

    while (1) {
        menu.Run(GetWindowWidth(), GetWindowHeight(), 0);
        
        glfwSwapBuffers(GetWindowHandle());
        glfwPollEvents(); 

        if (glfwWindowShouldClose(GetWindowHandle())) {
            ShouldStart = 0;
            break;
        }

        if (ShouldStart != -1) {
            break;
        }
    }

    menu.Release();

 	return ShouldStart;	// true means to start
}

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
    
    std::cout << err(Settings.Read(Environment.at(env_num).make_preferred())) << std::endl;
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
        ImGui::Text("Time: %dm %ds", (i32)glfwGetTime()/60, ((i32)glfwGetTime())%60);
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
	int i = 1;
	for (auto& member : KartActor) {
		member.UpdateData(Delta);
        // issue: couldnt create lap system successfully
		if (ekTimer.UpdateTick(ekClock, LAP_INCREASE_DELAY)) {
            delayLap = true;
        }

        if (delayLap && member.CheckAABB({checkpoint.x - range_checkpoint, checkpoint.x + range_checkpoint},
                                         {checkpoint.y - range_checkpoint, checkpoint.y + range_checkpoint})) {
            member.lap++;
            delayLap = false;
        }

		if (member.lap == lap) {
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
