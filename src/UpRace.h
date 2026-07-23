/*
    Some wishes to practice here:
    -  multithreading (dont have reasons to use, discard)
    -  macros to debug
    -  anti AI
*/

#ifndef UPRACE_H
#define UPRACE_H

constexpr float LAP_INCREASE_DELAY = 2900.0f;

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

    std::filesystem::path background;
    std::filesystem::path circuit;
    std::filesystem::path bitmask;
    
    f32 spaceMaxW;  // relative size of the space 
    f32 spaceMaxH;  // to the circuit (1.0f)
    
    Configurator Settings;
    i32          env_num;
    i32          ply_num;
    Texture      BlackMap;
    Entity       Floor;
    Entity       Circuit;
    Shader       GlobalShader;
    Mesh         GlobalMesh;  
    // the vertices are the same, only real difference is the 
    // model matrix, there's no need to create one vao and vbo
    // for each object in the screen.

    f32 actorSize;
    bool delayLap;
    i32 lap;
    i32 indexWinner;
    f32 range_checkpoint;
    glm::vec2 checkpoint;
    std::vector<Entity> KartData;
    std::vector<Kart> KartActor;

    Entity KartData0;
    Kart Kart0;
    Entity KartData1;
    Kart Kart1;
    

    std::vector<std::filesystem::path> Environment {
      "../settings/setup0.ini",
      "../settings/setup1.ini",
      "../settings/setup2.ini",
      "../settings/setup3.ini",
      "../settings/setup4.ini",
      "../settings/setup5.ini"
    };

    std::vector<std::filesystem::path> Actors = {
      "../settings/kart0.ini",
      "../settings/kart1.ini",
      "../settings/kart2.ini",
      "../settings/kart3.ini"
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

    bool StartMenu();
    void InitEnvironment();                         // Inicia o ambiente com COnfiguracoes personalziadas
    void Initialize();                              // inicializa RECURSOS
    void Input(GLFWwindow* window, f32 Delta);    // Entrada do jogador
    void ProcessPhysics(f32 Delta);               // processa fisica
    void Processing();                              // processamento
    void Render();                                  // render
    void Release();                                 // Deletes
};

#endif // UPRACE_H
