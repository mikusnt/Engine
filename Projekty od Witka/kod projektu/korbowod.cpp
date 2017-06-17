#include "korbowod.h"
#include "korbowod_data.h"

namespace std {

    Korbowod korbowod;

void Korbowod::DrawSolid() {
    DrawSolid(vec3(0.8f, 0.8f, 0.8f));
}
void Korbowod::DrawSolid(vec3 color) {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów
    glEnableClientState(GL_NORMAL_ARRAY);

    //vec3 color = vec3(0.8f, 0.8f, 0.8f);
    float korbowodColors[korbowodVertices * 3];
    for (int i = 0; i < korbowodVertices; i++) {
        korbowodColors[i*3] = color.x;
        korbowodColors[i*3 + 1] = color.y;
        korbowodColors[i*3 + 2] = color.z;
    }
    glVertexPointer(3,GL_FLOAT,0,korbowodPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glColorPointer(3,GL_FLOAT,0,korbowodColors); //Ustaw tablicê myCubeColors jako tablicê kolorów
    glNormalPointer( GL_FLOAT, 0, korbowodNormals);

    glDrawArrays(GL_TRIANGLES,0,korbowodVertices); //Rysuj model

    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
