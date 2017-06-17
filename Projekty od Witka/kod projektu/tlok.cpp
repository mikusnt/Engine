#include "tlok.h"
#include "tlok_data.h"

namespace std {

    Tlok tlok;

void Tlok::DrawSolid() {
    DrawSolid(vec3(0.8f, 0.8f, 0.8f));
}
void Tlok::DrawSolid(vec3 color) {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów
    glEnableClientState(GL_NORMAL_ARRAY);

    //vec3 color = vec3(0.8f, 0.8f, 0.8f);
    float tlokColors[tlokVertices * 3];
    for (int i = 0; i < tlokVertices; i++) {
        tlokColors[i*3] = color.x;
        tlokColors[i*3 + 1] = color.y;
        tlokColors[i*3 + 2] = color.z;
    }
    glVertexPointer(3,GL_FLOAT,0,tlokPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glColorPointer(3,GL_FLOAT,0,tlokColors); //Ustaw tablicê myCubeColors jako tablicê kolorów
    glNormalPointer( GL_FLOAT, 0, tlokNormals);

    glDrawArrays(GL_TRIANGLES,0,tlokVertices); //Rysuj model

    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
