#ifndef PARTICLES_H_INCLUDED
#define PARTICLES_H_INCLUDED

#include "opengl.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include "constants.h"

using namespace glm;

namespace std {
    class Particles {
    private:
        vec3 position;
        float speed;
        float directionSpeed;
        float finalMin_Y;
        float radius;
        Direction moveDirection;
       // Direction fromZeroDirection;
        unsigned int maxParticlesCount;
        float percentY;
        float oldPercentY;
        float *particlesTable;

        bool isInCircle(vec3 center, vec3 pos, float radius);
        bool isInside(vec3 point_pos, float max_y, float min_y);
        float rand1();

        vec3 newParticle(float max_y, float min_y);



    public:
        void newMoveDraw() { newParticles(); moveParticles(); drawSolid(); }
        void drawSolid();
         void newParticles();
        void reloadTable();
        void moveParticles();
        void setPosition(vec3 _position) { position = _position; } //reload
        void setMinY(float min_y) { finalMin_Y = min_y; }
        void setSpeed(float _speed) { speed = _speed; }
        void setDirectionSpeed(float _speed) { directionSpeed = _speed; }
        void setRadius(float _radius); // reload
        void setMoveDirection(Direction direction) { moveDirection = direction; }
        //void setFromZeroDirection(Direction _fromZeroDirection) { fromZeroDirection = _fromze; }
        void setParticlesCount(unsigned int _particlesCount) { maxParticlesCount = _particlesCount;} // reload
        void setPercentY(float _percentY); // reload

        vec3 getPosition() { return position; }
        float getRadius() { return radius; }
        float getMin_y() { return finalMin_Y; }
        Direction getMoveDirection() { return moveDirection; }
        unsigned int getMaxParticlesCount() { return maxParticlesCount; }
        float getPercenyY() { return percentY; }

        Particles(float _radius = 0.5, float _speed = 0.01f, float _directionSpeed = 0.05f, unsigned int _maxParticlesCount = 1000,
                  vec3 _position = vec3(0.0f, 1.0f, 0.0f),
            float _min_y = 0, Direction _moveDirection = dir_none, float _percentY = 0.0f);
        Particles(Particles &other);
        ~Particles();
    };
}

#endif // PARTICLES_H_INCLUDED
