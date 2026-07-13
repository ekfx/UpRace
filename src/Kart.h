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
#include "Core.h"

class Kart {
private:
    float X, Y, Z;  // Z is not functional, but I put here to stay vec3
    float Vel, VelInc;
    float VelAng, VelIncAng;
    float rpmMax, VelAngMax, rpm;
    float VelInertia, VelAngInertia;
    float Power;
    float i_x, i_y;

    glm::mat4 model;
    unsigned int TexSlot;

public:
    Kart() {
        Vel             = 0;
        rpmMax          = 8000.0f;
        VelInc          = 20.55f;
        VelInertia      = 0.75f;
        rpm             = 1600.0f;

        VelAng          = 0;
        VelAngMax       = 5.85f;   // em angulos
        VelIncAng       = 2.95f;
        VelAngInertia   = 0.75f;
        
        model           = glm::mat4(1.0f);
    };
    ~Kart() {};

    void Start(f32 x, f32 y) {
        model = glm::translate(model, glm::vec3(x, y, 0.0f));
    }

    void Run(int direction, float delta) {
        if (direction == ENGINE::DIRECTION::FORWARD) {
            if (rpm < rpmMax) rpm += 500.0f*delta;
            
            Vel += ((rpmMax-rpm)/10000.0f)*delta;
            // VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::BACKWARD) {
            if (rpm > -rpmMax) rpm -= 500.0f*delta;

            Vel -= ((rpmMax-rpm)/10000.0f)*delta;
            // VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::NOTHING) {
            rpm = rpm*VelInertia;
        }
    }

    void Turn(int direction, float delta) {
        if (Vel > 0.00005f || Vel < -0.00005f) {    
            if (direction == ENGINE::DIRECTION::FORWARD) {
                if (VelAng < VelAngMax) {
                    VelAng += VelIncAng*delta;
                    
                }
            } else if (direction == ENGINE::DIRECTION::BACKWARD) {
                if (VelAng > -VelAngMax) {
                    VelAng -= VelIncAng*delta;
                    
                }
            } else if (direction == ENGINE::DIRECTION::NOTHING) {
                VelAng = VelAng*VelAngInertia;    
            }
        } else {
            VelAng = VelAng*VelAngInertia*delta;
        }
    }

    void UpdateData(float delta) {
        // Vel = 0;
        // VelAng = 0;

        //Vel = VelInc;
        //VelAng = VelIncAng;
        
        Vel = Vel*VelInertia;

        float x_axis = Vel * tan(VelAng);
        float h = sqrt(Vel*Vel + x_axis*x_axis);

        glm::vec3 directionVector = glm::vec3(x_axis * delta, Vel * delta, 0.0f);

        X = model[3][0];
        Y = model[3][1];
        Z = model[3][2];

        ShowData();
        
        model = glm::rotate(model, VelAng, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, directionVector);
    }

    std::pair<int, int> ConvertToImageScale(float x_old, float y_old, float width_space, float height_space, float width_scale, float height_scale) {
        return {((x_old + (width_space/2) * width_scale) / width_space), 
                ((y_old + (height_space/2) * height_scale) / height_space)};
    }

    bool SetConfig(std::map<std::string, float>& config) {
      if (config.empty()) {
        return -1;
      } else {
        Vel                      = config["Vel"];
        rpmMax                   = config["rpmMax"];
        rpm                      = config["rpm"];
        VelInc                   = config["VelInc"];
        VelInertia               = config["VelInertia"];

        VelAng                   = config["VelAng"];
        VelAngMax                = config["VelAngMax"];
        VelIncAng                = config["VelIncAng"];
        VelAngInertia            = config["VelAngInertia"];

        i_x                      = config["initialX"];
        i_y                      = config["initialY"];
        model = glm::translate(model, glm::vec3(i_x, i_y, 0.0f));

        return 0;
      }
    }

    // returns the kart position to the new image position, or anything new scale
    int GetRelativeCoord(int axis, float old_max, float new_scale_size) {
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

            f32 PCoorY = -1.50f + ((Kart.GetY() - 0.0f) * (1.50f - (-1.50f))) / 4096.0f - 0;
        */

        if (axis == ENGINE::AXIS::X_AXIS) {
            return ((X + old_max) * new_scale_size) / (old_max * 2);
        } else if (axis == ENGINE::AXIS::Y_AXIS) {
            return ((Y + old_max) * new_scale_size) / (old_max * 2);
        } else {
            return 0;
        }
    }

    void Freeze() {
      Vel = 0;
    }

    void ResetPos() {
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(i_x, i_y, 0.0f));
    }

    glm::mat4 GetModel() {
        return model;
    }

    float GetX() {
        return X;
    }

    float GetY() {
        return Y;
    }

    float GetZ() {
        return Z;
    }

    float GetVel() {
      return Vel;
    }

    void ShowData() {
        std::cout << "\rX: " << X << " - Y: " << Y << " - Z: " << Z << " - Vel: " << Vel << " - VelAng: " << VelAng << " - rpm: " << rpm << " - rpmMax: " << rpmMax << std::endl;
    }
 };

#endif // KART_H
