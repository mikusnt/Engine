#include "kula_zaplonu.h"
#include "kula_zaplonu_data.h"

namespace std {

    Kula_Zaplonu kula_zaplonu;

void Kula_Zaplonu::DrawSolid(GLuint *tex, int number) {
glBindTexture(GL_TEXTURE_2D,tex[number]);
    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,kula_zaplonuPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glNormalPointer( GL_FLOAT, 0, kula_zaplonuNormals);
    glTexCoordPointer( 2, GL_FLOAT, 0, kula_zaplonuTexels);

    glDrawArrays(GL_TRIANGLES,0,kula_zaplonuVertices); //Rysuj model

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
