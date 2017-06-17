#include "zawor.h"
#include "zawor_data.h"

namespace std {

    Zawor zawor;

void Zawor::DrawSolid() {
    DrawSolid(vec3(0.8f, 0.8f, 0.8f));
}

void Zawor::DrawSolid(vec3 color) {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów
    glEnableClientState(GL_NORMAL_ARRAY);

    //vec3 color = vec3(0.8f, 0.8f, 0.8f);
    float zaworColors[zaworVertices * 3];
    for (int i = 0; i < zaworVertices; i++) {
        zaworColors[i*3] = color.x;
        zaworColors[i*3 + 1] = color.y;
        zaworColors[i*3 + 2] = color.z;
    }
    glVertexPointer(3,GL_FLOAT,0,zaworPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glColorPointer(3,GL_FLOAT,0,zaworColors); //Ustaw tablicê myCubeColors jako tablicê kolorów
    glNormalPointer( GL_FLOAT, 0, zaworNormals);

    glDrawArrays(GL_TRIANGLES,0,zaworVertices); //Rysuj model

    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
