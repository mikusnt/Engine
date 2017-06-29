#include "tlok.h"
#include "tlok_data.h"

namespace std {

    Tlok tlok;

void Tlok::DrawSolid(GLuint *tex, int number) {

    glBindTexture(GL_TEXTURE_2D,tex[number]);
    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,tlokPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glNormalPointer( GL_FLOAT, 0, tlokNormals);
    glTexCoordPointer( 2, GL_FLOAT, 0, tlokTexels);

    glDrawArrays(GL_TRIANGLES,0,tlokVertices); //Rysuj model

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
