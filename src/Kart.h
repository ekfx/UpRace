#ifndef KART_H
#define KART_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/common.hpp>
#include <utility>
#include <map>
#include <string>
#include <iostream>
#include "Core.h"
#include "Types.h"

class Kart {
private:
    float X, Y, Z;  // Z is not functional, but I put here to stay vec3

    // Angle Velocity
    float VelAng, VelIncAng;
    float VelAngMax;
    float VelAngInertia;
    
    // Velocity:
    float Vel, VelInertia;
    
    // Attributes
    float RPMInc, RPMMax, RPM;
    float Weight;

    float i_x, i_y;

    glm::mat4 model;
    std::string  Skin;

public:
    int lap;

    Kart();
    ~Kart();

    // Logic
    void Run(int direction, float delta);
    void Turn(int direction, float delta);

    void UpdateData(float delta);
    bool CheckAABB(std::pair<float, float> x_interval, std::pair<float, float> y_interval);
    std::pair<int, int> ConvertToImageScale(float x_old, float y_old, float width_space, float height_space, float width_scale, float height_scale);

    // Utilitary
    int GetRelativeCoord(int axis, float old_max, float new_scale_size);
    void Freeze();
    void ResetPos();
    
    // Setters
    bool SetConfig(const std::map<std::string, float>& config, const std::string& _skin);
    bool SetSkin(const std::string& _skin);
    void SetInitialPos(f32 _x, f32 _y);
    
    // Getters
    glm::mat4 GetModel();
    std::string& GetSkin();
    float GetX();
    float GetY();
    float GetZ();
    float GetVel();
    float GetRPM();
    float GetWeight();
 };

#endif // KART_H
