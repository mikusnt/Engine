#ifndef PARTICLES_CPP_INCLUDED
#define PARTICLES_CPP_INCLUDED

#include "particles.h"

namespace std {


void Particles::drawSolid() {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,particlesTable);
    glDrawArrays(GL_POINTS, 0, oldPercentY * maxParticlesCount);
    glDisableClientState(GL_VERTEX_ARRAY);
}
bool Particles::isInCircle(vec3 center, vec3 pos, float radius) {
    return sqrt(pow(center.x - pos.x, 2) + pow(center.z - pos.z, 2)) < radius;
}

bool Particles::isInside(vec3 point_pos, float max_y, float min_y) {
    //float actual_min_y = (position.y - (position.y - min_y) * percentY);
    return ( isInCircle(position, point_pos, radius) && (point_pos.y <= max_y) && (point_pos.y >= min_y));
}

float Particles::rand1() {
    return rand() / (float)RAND_MAX;
}

void Particles::reloadTable() {
    delete [] particlesTable;
    particlesTable = new float[maxParticlesCount * 3];
    oldPercentY = 0;
    newParticles();
}

vec3 Particles::newParticle(float max_y, float min_y) {
    vec3 temp;
    do{
        temp.x = rand1() * radius * 2;
        temp.x -= radius;
        if (min_y == max_y) temp.y = max_y;
        else {
            if (min_y > max_y) {
                int t = min_y;
                min_y = max_y;
                max_y = t;
            }
            temp.y = max_y - (rand1() * (max_y - min_y));
        }
        temp.z = rand1() * radius * 2;
        temp.z -= radius;
    } while (!isInside(temp, max_y, min_y));
    return temp;
}
void Particles::newParticles() {
    for(int i = maxParticlesCount * oldPercentY; i < maxParticlesCount * percentY; i++) {
        //cout << "b";
        vec3 temp = newParticle(position.y - (position.y - finalMin_Y) * oldPercentY, position.y - (position.y - finalMin_Y) * percentY);
        particlesTable[i*3] = temp.x;
        particlesTable[(i*3) + 1] = temp.y;
        particlesTable[(i*3) + 2] = temp.z;

    }
    oldPercentY = percentY;

   // cout << "a";
    //cout << maxParticlesCount << endl;
    //cout << oldPercentY << endl;
}

void Particles::moveParticles() {
    for(int i = 0; i < maxParticlesCount * oldPercentY; i++) {
        vec3 temp;
        float after1, after2, after3;
        int j = 0;
        float actualMinY = position.y - ((position.y - finalMin_Y) * oldPercentY);
        do {
            after1 = rand1() * speed * 2;
            after1 -= speed;
            switch(moveDirection) {
                case dir_none: {
                    after2 = rand1() * speed * 2;
                    after2 -= speed;
                } break;
                case dir_up: {
                    after2 = rand1() * directionSpeed;
                } break;
                case dir_down: {
                    after2 = -rand1() * directionSpeed;
                } break;
            }

            after3 = rand1() * speed * 2;
            after3 -= speed;
            j++;

            temp = vec3(particlesTable[i*3] + after1, particlesTable[(i*3)+1] + after2, particlesTable[(i*3)+2] + after3);
        } while (!isInside(temp, position.y, position.y - ((position.y - finalMin_Y) * oldPercentY)) && (j < 2));

        switch(moveDirection) {
            case dir_none: {
                if (j >= 2) temp = newParticle(position.y, actualMinY);
            } break;
            case dir_up: {
                if (j >= 2) temp = newParticle(actualMinY, actualMinY);
            } break;
            case dir_down: {
                if (j >= 2) {
                        temp = newParticle(position.y, position.y);
                }
            } break;
        }
        if (j >= 2) {
            particlesTable[i*3] = temp.x;
            particlesTable[(i*3)+1] = temp.y;
            particlesTable[(i*3)+2] = temp.z;
        } else {
            particlesTable[i*3] += after1;
            particlesTable[(i*3)+1] += after2;
            particlesTable[(i*3)+2] += after3;
        }
    }
}

void Particles::setRadius(float _radius) {
    if (_radius <= 0) radius = 1;
        else radius = _radius;
    }
void Particles::setPercentY(float _percentY) {
    //if ((percentY != oldPercentY) || (percentY == 0)) {
        if (_percentY > 1) percentY = 1;
            else if (_percentY < 0) percentY = 0;
                else percentY = _percentY;
    //}
   // cout << percentY << endl;
}

Particles::Particles(float _radius, float _speed, float _directionSpeed, unsigned int _maxParticlesCount, vec3 _position,
            float _min_y, Direction _moveDirection, float _percentY) : speed(_speed), directionSpeed(_directionSpeed),
            maxParticlesCount(_maxParticlesCount), position(_position), finalMin_Y(_min_y), moveDirection(_moveDirection)  {
    setRadius(_radius);
    setPercentY(_percentY);
    oldPercentY = 0;
    particlesTable = new float[maxParticlesCount * 3];
    reloadTable();
}

Particles::Particles(Particles &other) {
    //fromZeroDirection = other.fromZeroDirection;
    radius = other.radius;
    speed = other.speed;
    directionSpeed = other.directionSpeed;
    maxParticlesCount = other.maxParticlesCount;
    position = other.position;
    finalMin_Y = other.finalMin_Y;
    moveDirection = other.moveDirection;
    percentY = other.percentY;
    oldPercentY = other.oldPercentY;

    particlesTable = new float[maxParticlesCount * 3];
    for (int i = 0; i < maxParticlesCount; i++) {
        particlesTable[i*3] = other.particlesTable[i*3];
        particlesTable[(i*3)+1] = other.particlesTable[(i*3)+1];
        particlesTable[(i*3)+2] = other.particlesTable[(i*3)+2];
    }
}

Particles::~Particles() {
    delete [] particlesTable;
}
}

#endif // PARTICLES_CPP_INCLUDED
