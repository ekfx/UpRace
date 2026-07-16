#include "Kart.h"

Kart::Kart() {
    Vel             = 0;
    RPMMax          = 8000.0f;
    RPMInc          = 500.55f;
    VelInertia      = 0.75f;
    RPM             = 1600.0f;

    VelAng          = 0;
    VelAngMax       = 5.85f;   // em angulos
    VelIncAng       = 2.95f;
    VelAngInertia   = 0.75f;
    
    model           = glm::mat4(1.0f);
};

Kart::~Kart() 
{
};

//////////////////////////////////////////////////////////////////////////////////////
// Logic

void Kart::Run(int direction, float delta) {
    if (direction == ENGINE::DIRECTION::FORWARD) {
        if (RPM < RPMMax) RPM += RPMInc*delta;
        
        Vel += ((RPMMax-RPM)/10000.0f)*delta;
        // VelInc -= Vel*delta;

    } else if (direction == ENGINE::DIRECTION::BACKWARD) {
        if (RPM < +RPMMax) RPM += RPMInc*delta;

        Vel -= ((RPMMax-RPM)/10000.0f)*delta;
        // VelInc -= Vel*delta;

    } else if (direction == ENGINE::DIRECTION::NOTHING) {
        RPM = RPM*VelInertia;
    }
}

void Kart::Turn(int direction, float delta) {
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

void Kart::UpdateData(float delta) {
    // Vel = 0;
    // VelAng = 0;

    //Vel = VelInc;
    //VelAng = VelIncAng;
    
    Vel = Vel*VelInertia;

    // We have the Y (Vel) and the angle, so we need the X to create
    // the direction vector multiplying the Y with the tangent of the angle.
    float x_axis = Vel * tan(VelAng);

    // putting it into the vector
    glm::vec3 directionVector = glm::vec3(x_axis * delta, Vel * delta, 0.0f);

    // extracting the coordinates
    X = model[3][0];
    Y = model[3][1];
    Z = model[3][2];
    
    // and finally applying the transformation
    model = glm::rotate(model, VelAng, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, directionVector);
}

//////////////////////////////////////////////////////////////////////////////////////
// Utilitary

std::pair<int, int> Kart::ConvertToImageScale(float x_old, float y_old, float width_space, float height_space, float width_scale, float height_scale) {
    return {((x_old + (width_space/2) * width_scale) / width_space), 
            ((y_old + (height_space/2) * height_scale) / height_space)};
}

void Kart::Freeze() {
    Vel = 0;
    RPM = 0;
}   

void Kart::ResetPos() {
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(i_x, i_y, 0.0f));
}

//////////////////////////////////////////////////////////////////////////////////////
// Setters

void Kart::SetInitialPos(f32 _x, f32 _y) {
    i_x = _x;
    i_y = _y;
}

bool Kart::SetSkin(const std::string& _skin) {
    if (_skin.empty()) {
        return -1;
    } else {
        Skin = _skin;
        return 0;
    }
}

bool Kart::SetConfig(const std::map<std::string, float>& config, const std::string& _skin) {
    if (config.empty()) {
        return -1;
    } else {
        SetSkin(_skin);
        Vel                      = config.at(std::string("Vel"             ));
        RPMMax                   = config.at(std::string("RPMMax"          ));
        RPM                      = config.at(std::string("RPM"             ));
        RPMInc                   = config.at(std::string("RPMInc"          ));

        // you can use this to set the weight:
        // 0.99379 * 10000 = 9937.9
        // 10000 - 9937.9 = 62.1
        // 62.1 * 10 = 621
        Weight                   = config.at(std::string("Weight"          ));
        VelInertia = (10000 - (Weight / 10)) / 10000;

        VelAng                   = config.at(std::string("VelAng"          ));
        VelAngMax                = config.at(std::string("VelAngMax"       ));
        VelIncAng                = config.at(std::string("VelIncAng"       ));
        VelAngInertia            = config.at(std::string("VelAngInertia"   ));
        model = glm::translate(model, glm::vec3(i_x, i_y, 0.0f));

        return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////////////
// Getters

// returns the kart position to the new image position, or anything new scale
int Kart::GetRelativeCoord(int axis, float old_max, float new_scale_size) {
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

glm::mat4       Kart::GetModel()   { return model; }
std::string&    Kart::GetSkin()    { return Skin; }
float           Kart::GetX()       { return X; }
float           Kart::GetY()       { return Y; }
float           Kart::GetZ()       { return Z; }
float           Kart::GetVel()     { return Vel; }
float           Kart::GetRPM()     { return RPM; }
float           Kart::GetWeight()  { return Weight; }