#include "Program.h"

void Program::framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height) {
    glViewport(0, 0, width, height);
} 

Program::Program() 
{
}

Program::~Program() 
{
}

void Program::Loop() {
    ekClock.StartCounter();
    InitializeEssential();
    //if (StartMenu()) {
        InitEnvironment();
        Initialize();
        while (!glfwWindowShouldClose(ekWindowProvider.GetWindow())) {
            ProcessEssential();
            Processing();
            Render();
            Essential();
        }
        Release();
    //}
    std::cout << (f32)glfwGetTime() << " seconds elapsed since the program start." << std::endl;
}

void Program::Essential() {
    glfwSwapBuffers(ekWindowProvider.GetWindow());   // SwapBuffer, nunca esqueça!
    glfwPollEvents();                   // Puxar Eventos, nunca esqueça!
}

void Program::ProcessEssential() {
    glClear(GL_COLOR_BUFFER_BIT);   // tem que ser definido mesmo fora do set3d
    ekClock.ProcessDelta();
    UpdateWindowName(true, 64.0f);
    Input(ekWindowProvider.GetWindow(), ekClock.GetDeltaTime());
    ProcessPhysics(ekClock.GetDeltaTime());
}

void Program::InitializeEssential() 
{
    ekWindowProvider.RunWindow(800, 600, "Window", ENGINE::WINDOW::WINDOWED);
    ekWindowProvider.SetVSync(true);
    glfwSetFramebufferSizeCallback(ekWindowProvider.GetWindow(), framebuffer_size_callback);
    
    Mouse::StartMouse(GetWindowHandle());
    Keyboard::StartKeyboard(GetWindowHandle());
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    Set3D(false);
    SetRGBA();
    
    Mouse::EndFrame();
    Mouse::BlockMouseInScreen(GetWindowHandle(), false);
}

// bool Program::StartMenu() 
// {
// }

void Program::InitEnvironment() 
{
}

void Program::Initialize() 
{    
}

void Program::Processing() 
{
}

void Program::ProcessPhysics(f32 Delta) 
{
}

void Program::Input(GLFWwindow* window, f32 Delta) 
{    
}

void Program::Render() 
{
}

void Program::Release() 
{
}


// Configs
void Program::UpdateWindowName(bool if_run, f32 miliseconds) {
    if (if_run) {
        if (ekTimer.Update(ekClock, miliseconds)) {
            glfwSetWindowTitle(GetWindowHandle(), TimerUI::GetFPSandMS(ekClock).c_str());    
        }
    }
}

// Getters
GLFWwindow* Program::GetWindowHandle() {
    return ekWindowProvider.GetWindow();
};

f32 Program::GetWindowWidth() {
    return (f32)ekWindowProvider.GetWindowWidth();
}

f32 Program::GetWindowHeight() {
    return (f32)ekWindowProvider.GetWindowHeight();
}

f32 Program::GetWindowAspectRatio() {
    return (f32)ekWindowProvider.GetWindowAspectRatio();
}

std::string Program::GetInfoLog() {
    return InfoLog;
}

void Program::SetVSync(bool off_on) {
    ekWindowProvider.SetVSync(off_on);
}

// Setters
void Program::SetBackgroundColor(i32 program_color) {
    switch (program_color) {
        case ENGINE::WINDOW::RED_BACKGROUND:         glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break;
        case ENGINE::WINDOW::BLUE_BACKGROUND:        glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break;
        case ENGINE::WINDOW::GREEN_BACKGROUND:       glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break;
        case ENGINE::WINDOW::BLACK_BACKGROUND:       glClearColor(0.0f, 0.0f, 0.0f, 1.0f); break;
        case ENGINE::WINDOW::WHITE_BACKGROUND:       glClearColor(1.0f, 1.0f, 1.0f, 1.0f); break;
        case ENGINE::WINDOW::GRAY_BACKGROUND:        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); break;
        case ENGINE::WINDOW::GRAY_WHITE_BACKGROUND:  glClearColor(0.75f, 0.75f, 0.75f, 1.0f); break; 
        case ENGINE::WINDOW::GRAY_BLACK_BACKGROUND:  glClearColor(0.25f, 0.25f, 0.25f, 1.0f); break;
        case ENGINE::WINDOW::PURPLE_BACKGROUND:      glClearColor(1.0f, 0.0f, 1.0f, 1.0f); break;
        case ENGINE::WINDOW::YELLOW_BACKGROUND:      glClearColor(1.0f, 1.0f, 0.0f, 1.0f); break;
        default: InfoLog += "INVALID_VALUE\0\0"; break;
    }
}

void Program::Set3D(bool on) {
    if (on) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST); 
    } else if (!on) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Program::SetRGBA() {
    glEnable(GL_BLEND);     // mistura de cores
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // aceita rgba
    /*
        apenas diz que vai multiplicar a cor da imagem pelo alpha, e 
        subtrair a cor de fundo com o alpha da cor sobressalente
    */
}