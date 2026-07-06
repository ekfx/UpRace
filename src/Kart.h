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
#include "Core.h"

class KartS {
private:
    float X, Y, Z;  // Z is not functional, but I put here to stay vec3
    float Vel, VelAng, VelInc, VelIncAng;
    float VelMax, VelAngMax;
    float VelInertia, VelAngInertia;
    float Power;

    glm::mat4 model;
    unsigned int TexSlot;

public:
    KartS() {
        Vel             = 0;
        VelMax          = 0.025f;
        VelInc          = 0.15f;
        VelInertia      = 0.99f;

        VelAng          = 0;
        VelAngMax       = 5.85f;   // em angulos
        VelIncAng       = 1.95f;
        VelAngInertia   = 0.75f;
        
        model           = glm::mat4(1.0f);
    };
    ~KartS() {};

    void Run(int direction, float delta) {
        if (direction == ENGINE::DIRECTION::FORWARD) {
            if (Vel < VelMax) Vel += VelInc*delta;
            VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::BACKWARD) {
            if (Vel > -VelMax) Vel -= VelInc*delta;
            VelInc -= Vel*delta;

        } else if (direction == ENGINE::DIRECTION::NOTHING) {
            Vel = Vel*VelInertia;
            if (VelInc < 0.15f) {
                VelInc = VelInc*1.005f;
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
            VelAng = VelAng*VelAngInertia;
        }
    }

    void UpdateData(float delta) {
        // Vel = 0;
        // VelAng = 0;

        //Vel = VelInc;
        //VelAng = VelIncAng;

        float x_axis = Vel * tan(VelAng);
        float h = sqrt(Vel*Vel + x_axis*x_axis);

        glm::vec3 directionVector = glm::vec3(x_axis, Vel, 0.0f);

        X = model[3][0];
        Y = model[3][1];
        Z = model[3][2];

        ShowData();
        
        model = glm::rotate(model, VelAng, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, directionVector);
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
        std::cout << "X: " << X << " - Y: " << Y << " - Z: " << Z << " - Vel: " << Vel << " - VelAng: " << VelAng << std::endl;
    }
 };

#endif // KART_H