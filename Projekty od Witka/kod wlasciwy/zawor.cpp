#include "zawor.h"
#include "zawor_data.h"

namespace std {

    Zawor zawor;

void Zawor::DrawSolid(GLuint *tex, int number) {

    glBindTexture(GL_TEXTURE_2D,tex[number]);
    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,zaworPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glNormalPointer( GL_FLOAT, 0, zaworNormals);
    glTexCoordPointer( 2, GL_FLOAT, 0, zaworTexels);

    glDrawArrays(GL_TRIANGLES,0,zaworVertices); //Rysuj model

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
