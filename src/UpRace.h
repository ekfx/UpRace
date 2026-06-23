/*
    Some wishes to practice here:
    -  multithreading
    -  macros to debug
    -  anti AI
*/

#ifndef UPRACE_H
#define UPRACE_H

#include "Program.h"

class UpRace : public Program {
private:
    Shader GlobalShader;

    Mesh floorM;
    Texture floorT;
    glm::mat4 floorMd;

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