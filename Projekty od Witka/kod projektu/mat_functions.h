#ifndef MAT_FUNCTIONS_H_INCLUDED
#define MAT_FUNCTIONS_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include "constants.h"

using namespace glm;

namespace std {

    class EngineParams {
        public:
            static const float r;
            static const float length;
            static const float alfa;
            static const float valveAngle;
            static const float sinCut;

    };

    mat4 matRotateXYZ(float angle_x, float angle_y, float angle_z);
    mat4 mPiston(float pos_x, float pos_z, float max_y, float angle);
    mat4 mCrank(float pos_x, float pos_z, float max_y, float engine_angle);
    mat4 mValve(float pos_x, float pos_z, float min_y, float delta_y, float engine_angle, ValveDirection direction);
    mat4 mValveParticle(vec3 pos, ValveDirection direction);
    mat4 mPistonParticle(vec3 pos);
    Direction ValveParticleDir(float engine_angle, ValveDirection direction);
    float ValveParticlePercent(float engine_angle, float minPercent, ValveDirection direction);
    float PistonParticlePercent(float engine_angle, float minPercent);
    vec3 PistonParticleColor(float engine_angle);
    //float normalizeAngle(float angle, float max_angle);

}

#endif // MAT_FUNCTIONS_H_INCLUDED
