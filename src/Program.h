#ifndef PROGRAM_H
#define PROGRAM_H

#include <GLAD/glad.h>   
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <print>

#include "Shader.h"
#include "Timer.h"
#include "Clock.h"
#include "TimerUI.h"
#include "OpenGL.h"
#include "Types.h"
#include "Mesh.h"
#include "Textures.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "Object.h"
#include "Core.h"
#include "Sound.h"
#include "HUD.h"
#include "ExtConfig.h"

class Program {
private:
    // Attributes
    std::string InfoLog;
    f32         width        = GetWindowWidth();
    f32         height       = GetWindowHeight();

    // Objects
    OpenGL      ekWindowProvider;

public:
    Camera      ekCamera;
    Clock       ekClock;
    Timer       ekTimer;
    SoundCore   ekSound;
    HUD         ekHUD;

    Program();
    ~Program();

    void Loop();
    void InitializeEssential();
    void Essential();
    void ProcessEssential();
    virtual void InitEnvironment();                         // Inicia o ambiente com COnfiguracoes personalziadas
    virtual void Initialize();                              // inicializa RECURSOS
    virtual void Input(GLFWwindow* window, f32 Delta);    // Entrada do jogador
    virtual void ProcessPhysics(f32 Delta);               // processa fisica
    virtual void Processing();                              // processamento
    virtual void Render();                                  // render
    virtual void Release();                                 // Deletes

    
    // Configs
    void UpdateWindowName(bool if_run, f32 miliseconds);
    
    // Callbacks
    static void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height);
    
    // Getters
    GLFWwindow* GetWindowHandle();    
    f32 GetWindowHeight();
    f32 GetWindowWidth();
    f32 GetWindowAspectRatio();
    std::string GetInfoLog();
    
    // Setters
    void SetBackgroundColor(i32 program_color);
    void Set3D(bool on);            // desativado por padrão
    void SetRGBA();
    void SetVSync(bool off_on);     // ativado por padrão
};

#endif //PROGRAM_H