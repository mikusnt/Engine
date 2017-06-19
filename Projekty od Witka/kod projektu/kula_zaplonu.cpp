#include "kula_zaplonu.h"
#include "kula_zaplonu_data.h"

namespace std {

    Kula_Zaplonu kula_zaplonu;

    void Kula_Zaplonu::DrawSolid() {
    DrawSolid(vec3(0.8f, 0.8f, 0.8f));
}
void Kula_Zaplonu::DrawSolid(vec3 color) {
     glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_COLOR_ARRAY); //Podczas rysowania u¿ywaj tablicy kolorów
    glEnableClientState(GL_NORMAL_ARRAY);

    //vec3 color = vec3(0.8f, 0.8f, 0.8f);
    float kula_zaplonuColors[kula_zaplonuVertices * 3];
    for (int i = 0; i < kula_zaplonuVertices; i++) {
        kula_zaplonuColors[i*3] = color.x;
        kula_zaplonuColors[i*3 + 1] = color.y;
        kula_zaplonuColors[i*3 + 2] = color.z;
    }
    glVertexPointer(3,GL_FLOAT,0,kula_zaplonuPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glColorPointer(3,GL_FLOAT,0,kula_zaplonuColors); //Ustaw tablicê myCubeColors jako tablicê kolorów
    glNormalPointer( GL_FLOAT, 0, kula_zaplonuNormals);

    glDrawArrays(GL_TRIANGLES,0,kula_zaplonuVertices); //Rysuj model

    //Posprz¹taj po sobie
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
