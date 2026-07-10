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
#include "Core.h"

class Kart {
private:
    float X, Y, Z;  // Z is not functional, but I put here to stay vec3
    float Vel, VelAng, VelInc, VelIncAng;
    float VelMax, VelAngMax;
    float VelInertia, VelAngInertia;
    float Power;

    glm::mat4 model;
    unsigned int TexSlot;

public:
    Kart() {
        Vel             = 0;
        VelMax          = 100.5f;
        VelInc          = 20.55f;
        VelInertia      = 0.75f;

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
            if (Vel < VelMax) Vel += VelInc*delta;
            // VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::BACKWARD) {
            if (Vel > -VelMax) Vel -= VelInc*delta;
            // VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::NOTHING) {
            Vel = Vel*VelInertia;
            if (VelInc < 0.15f) {
                VelInc = VelInc*1.5f;
            }
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

        float x_axis = Vel * tan(VelAng);
        float h = sqrt(Vel*Vel + x_axis*x_axis);

        glm::vec3 directionVector = glm::vec3(x_axis * delta, Vel * delta, 0.0f);

        X = model[3][0];
        Y = model[3][1];
        Z = model[3][2];

        // ShowData();
        
        model = glm::rotate(model, VelAng, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, directionVector);
    }

    std::pair<int, int> ConvertToImageScale(float x_old, float y_old, float width_space, float height_space, float width_scale, float height_scale) {
        return {((x_old + (width_space/2) * width_scale) / width_space), 
                ((y_old + (height_space/2) * height_scale) / height_space)};
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

    void ShowData() {
        std::cout << "\rX: " << X << " - Y: " << Y << " - Z: " << Z << " - Vel: " << Vel << " - VelAng: " << VelAng << std::endl;
    }
 };

#endif // KART_H
