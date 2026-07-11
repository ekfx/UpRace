/*
    Some wishes to practice here:
    -  multithreading
    -  macros to debug
    -  anti AI
*/

#ifndef UPRACE_H
#define UPRACE_H

#include "Program.h"
#include "Kart.h"

class UpRace : public Program {
private:
    struct Entity {
        glm::mat4   model;
        Texture     tex;

        f32 x, y;
        f32 vel;
        f32 velInc, ang, velAng;

        Entity() {
            model = glm::mat4(1.0f);
        }
    };

    Texture BlackMap;
    Entity Floor;
    Entity Circuit;
    Shader GlobalShader;
    Mesh GlobalMesh;  
    // the vertices are the same, only real difference is the 
    // model matrix, there's no need to create one vao and vbo
    // for each object in the screen.

    Entity KartData;
    Kart Kart;
    IniReader EnvConfig;
    // std::ofstream AI;
    
    std::filesystem::path background;
    std::filesystem::path circuit;
    std::filesystem::path bitmask;
    std::filesystem::path skin;

    IniReader Settings;
    StrIniReader SettingsStr;

    std::vector<std::string> Environment {
      "../settings/setup1.ini",
      "../settings/setup2.ini",
      "../settings/setup3.ini",
      "../settings/setup4.ini"
    };

    std::vector<std::string> Actors = {
      "../settings/kart1.ini",
      "../settings/kart2.ini",
      "../settings/kart3.ini",
      "../settings/kart4.ini"
    };


    // Vertex
    u32 MyEBO[6] {
        0, 1, 2,
        2, 3, 0,
    };

    // Default Vertex
    Mesh::vertex MyVertex[4] {         // Padrão
        // X      Y       Z  |  R      G     B  |    U     V
        {-0.50f, -0.50f,  0.0f, 1.0f,  1.0f,  0.0f,  0.00f, 0.00f}, // bottom left
        { 0.50f, -0.50f,  0.0f, 0.0f,  0.85f, 0.0f,  1.00f, 0.00f}, // bottom right
        { 0.50f,  0.50f,  0.0f, 0.85f, 0.0f,  0.0f,  1.00f, 1.00f},  // top right
        {-0.50f,  0.50f,  0.0f, 0.0f,  0.0f,  0.85f, 0.00f, 1.00f}, // top left 
    };

public:

    void InitEnvironment();                         // Inicia o ambiente com COnfiguracoes personalziadas
    void Initialize();                              // inicializa RECURSOS
    void Input(GLFWwindow* window, f32 Delta);    // Entrada do jogador
    void ProcessPhysics(f32 Delta);               // processa fisica
    void Processing();                              // processamento
    void Render();                                  // render
    void Release();                                 // Deletes
};

#endif // UPRACE_H
