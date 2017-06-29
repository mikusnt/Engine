#include "wal_korbowy.h"
#include "wal_korbowy_data.h"



namespace std {

    Wal_Korbowy wal_korbowy;

void Wal_Korbowy::DrawSolid(GLuint *tex, int number) {

    glBindTexture(GL_TEXTURE_2D,tex[number]);
    glEnableClientState(GL_VERTEX_ARRAY); //Podczas rysowania u¿ywaj tablicy wierzcho³ków
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3,GL_FLOAT,0,wal_korbowyPositions); //Ustaw tablicê myCubeVertices jako tablicê wierzcho³ków
    glNormalPointer( GL_FLOAT, 0, wal_korbowyNormals);
    glTexCoordPointer( 2, GL_FLOAT, 0, wal_korbowyTexels);

    glDrawArrays(GL_TRIANGLES,0,wal_korbowyVertices); //Rysuj model

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
}
