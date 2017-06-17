/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include "opengl.h"
#include <stdlib.h>
#include <stdio.h>
#include <lodepng.h>
#include <iostream>
#include "constants.h"
#include "allmodels.h"
#include "mat_functions.h"

using namespace glm;
using namespace std;


float aspect=1.0f; //Aktualny stosunek szerokości do wysokości okna
float delta_engine_speed_x;

float engine_speed_x=-PI/2; //Szybkość kątowa obrotu obiektu w radianach na sekundę wokół osi x
float camera_y=0;
float camera_x=0;
float delta_pos_y=0;
float delta_pos_x=0;
float delta_dimension = 0;

// najwyzsze polozenie
float max_piston_y = 3.62;
float max_crank_y = 3.4;


// polozenie x tlokow
float piston1_x = 3.4;
float piston2_x = 1.12;
float piston3_x = -1.12;
float piston4_x = -3.35;

//polozenie z zaworow
float min_valve_y = max_piston_y + 0.7;
float valve_z_in = -0.5;
float valve_z_out = -valve_z_in;
float valve_delta_y = 0.1f;

// polozenie casteczek zaworow
float valveParticle_y_in = min_valve_y - 0.2;
float valveParticle_y_out = min_valve_y + 0.7f;
float valveParticle_z_in = valve_z_in+0.1f;
float valveParticle_z_out = -valveParticle_z_in +0.5;
float valveParticle_delta_y = 0.25f;

float minValveParticlePercent = 0.75f;

// polozenie czasteczek tlokow
float max_pistonParticle_y = max_piston_y - 1.5f;

// obiekty czasteczek
Particles *ParticleValveIn1, *ParticleValveIn2, *ParticleValveIn3, *ParticleValveIn4;
Particles *ParticleValveOut1, *ParticleValveOut2, *ParticleValveOut3, *ParticleValveOut4;

Particles *ParticlePiston1, *ParticlePiston2, *ParticlePiston3, *ParticlePiston4;

// kolory
vec3 crackshaft_color = vec3(0.8f, 0.8f, 0.8f);
vec3 crank_color = vec3(0.8f, 0.8f, 0.8f);
vec3 piston_color = vec3(0.8f, 0.8f, 0.8f);
vec3 valve_color = vec3(0.2f, 0.2f, 0.2f);

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obługi zmiany rozmiaru bufora ramki
void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    aspect=(float)width/(float)height; //Stosunek szerokości do wysokości okna
}

//Procedura obsługi klawiatury
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_LEFT) camera_y=PI/2;
        if (key == GLFW_KEY_RIGHT) camera_y=-PI/2;
        //if (key == GLFW_KEY_A) delta_pos_x=10;
        //if (key == GLFW_KEY_D) delta_pos_x=-10;
       // if (key == GLFW_KEY_W) delta_pos_y=10;
        //if (key == GLFW_KEY_S) delta_pos_y=-10;
        if (key == GLFW_KEY_R) delta_dimension=-10;
        if (key == GLFW_KEY_F) delta_dimension=10;
        if (key == GLFW_KEY_UP) camera_x = -PI/2;
        if (key == GLFW_KEY_DOWN) camera_x = PI/2;

        if (key == GLFW_KEY_T) delta_engine_speed_x=PI;
        if (key == GLFW_KEY_G) delta_engine_speed_x=-PI;

    }

    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) camera_y=0;
        if (key == GLFW_KEY_RIGHT) camera_y=0;
        if (key == GLFW_KEY_A) delta_pos_x=0;
        if (key == GLFW_KEY_D) delta_pos_x=0;
        if (key == GLFW_KEY_W) delta_pos_y=0;
        if (key == GLFW_KEY_S) delta_pos_y=0;
        if (key == GLFW_KEY_R) delta_dimension=0;
        if (key == GLFW_KEY_F) delta_dimension=0;
        if (key == GLFW_KEY_UP) camera_x = 0;
        if (key == GLFW_KEY_DOWN) camera_x = 0;
        if (key == GLFW_KEY_T) delta_engine_speed_x=0;
        if (key == GLFW_KEY_G) delta_engine_speed_x=0;
    }
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
    glfwSetFramebufferSizeCallback(window, windowResize); //Zarejestruj procedurę obsługi zmiany rozdzielczości bufora ramki
    glfwSetKeyCallback(window, key_callback); //Zarejestruj procedurę obsługi klawiatury

	glClearColor(0,0,0,1); //Ustaw kolor czyszczenia ekranu
    //glShadeModel(GL_SMOOTH);


	glEnable(GL_LIGHT0); //Włącz zerowe źródło światła
    glEnable(GL_COLOR);
	glEnable(GL_DEPTH_TEST); //Włącz używanie budora głębokości
	glEnable(GL_NORMALIZE);
	ParticleValveIn1 = new Particles(0.2, 0.01, 0.05, 400);
	ParticleValveIn2 = new Particles(*ParticleValveIn1);
	ParticleValveIn3 = new Particles(*ParticleValveIn1);
	ParticleValveIn4 = new Particles(*ParticleValveIn1);
	ParticleValveOut1 = new Particles(*ParticleValveIn1);
	ParticleValveOut2 = new Particles(*ParticleValveIn1);
	ParticleValveOut3 = new Particles(*ParticleValveIn1);
	ParticleValveOut4 = new Particles(*ParticleValveIn1);

	ParticlePiston1 = new Particles(1.0, 0.01, 0.05, 2000, vec3(0, 2.2, 0));
	ParticlePiston2 = new Particles(*ParticlePiston1);
	ParticlePiston3 = new Particles(*ParticlePiston1);
	ParticlePiston4 = new Particles(*ParticlePiston1);
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_engine,float angle_x, float angle_y, float pos_x, float pos_y, float dimension) {
	//************Tutaj umieszczaj kod rysujący obraz******************l

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //Wyczyść bufor kolorów (czyli przygotuj "płótno" do rysowania)
    float ambient[] = {0.19f, 0.19f, 0.19f, 1.0f};
    	float specular[] = {0.5f, 0.5f, 0.5f, 1.0f};
	float diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular );
    //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, table );
    glMaterialf( GL_FRONT, GL_SHININESS, 0.6f * 128 );

    //***Przygotowanie do rysowania****
    mat4 P=perspective(50.0f*PI/180.0f,aspect,1.0f,50.0f); //Wylicz macierz rzutowania P
    vec4 camera_pos = vec4(pos_x, pos_y, -dimension, 0);

    camera_pos = camera_pos * matRotateXYZ(angle_x, angle_y, 0);
    mat4 V=lookAt( //Wylicz macierz widoku
                  vec3(camera_pos.x, camera_pos.y, camera_pos.z),
                  vec3(pos_x,pos_y, 0.0),
                  vec3(0.0f,1.0f,0.0f));
    glMatrixMode(GL_PROJECTION); //Włącz tryb modyfikacji macierzy rzutowania
    glLoadMatrixf(value_ptr(P)); //Załaduj macierz rzutowania
    glMatrixMode(GL_MODELVIEW);  //Włącz tryb modyfikacji macierzy model-widok

    //Rysowanie obiektow
    // tryb czasteczek
    glEnable(GL_COLOR_MATERIAL); //Włącz śledzenie kolorów przez materiał
    glDisable(GL_LIGHTING); //Włącz tryb cieniowania
    glColor3d(0.0f, 1.0f, 1.0f);

    // czasteczka zawor WE 1
    mat4 M=mValveParticle(vec3(piston1_x, valveParticle_y_in, valveParticle_z_in), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveIn1->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveIn1->setMoveDirection(ValveParticleDir(angle_engine, valve_in));
    ParticleValveIn1->setPercentY(ValveParticlePercent(angle_engine, minValveParticlePercent, valve_in));
    ParticleValveIn1->newMoveDraw();

    // czasteczka zawor WE 2
    M=mValveParticle(vec3(piston2_x, valveParticle_y_in, valveParticle_z_in), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveIn2->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveIn2->setMoveDirection(ValveParticleDir(angle_engine - PI, valve_in));
    ParticleValveIn2->setPercentY(ValveParticlePercent(angle_engine - PI, minValveParticlePercent, valve_in));
    ParticleValveIn2->newMoveDraw();

    // czasteczka zawor WE 3
    M=mValveParticle(vec3(piston3_x, valveParticle_y_in, valveParticle_z_in), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveIn3->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveIn3->setMoveDirection(ValveParticleDir(angle_engine + PI, valve_in));
    ParticleValveIn3->setPercentY(ValveParticlePercent(angle_engine + PI, minValveParticlePercent, valve_in));
    ParticleValveIn3->newMoveDraw();

    // czasteczka zawor WE 4
    M=mValveParticle(vec3(piston4_x, valveParticle_y_in, valveParticle_z_in), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveIn4->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveIn4->setMoveDirection(ValveParticleDir(angle_engine + (2*PI), valve_in));
    ParticleValveIn4->setPercentY(ValveParticlePercent(angle_engine + (2*PI), minValveParticlePercent, valve_in));
    ParticleValveIn4->newMoveDraw();

    // czasteczka zawor WY 1
    vec3 color = PistonParticleColor(-angle_engine + (2*PI) );
    glColor3d(color.x, color.y, color.z);

    M=mValveParticle(vec3(piston1_x, valveParticle_y_out, valveParticle_z_out), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveOut1->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveOut1->setMoveDirection(ValveParticleDir(angle_engine, valve_out));
    ParticleValveOut1->setPercentY(ValveParticlePercent(angle_engine, 0, valve_out));
    ParticleValveOut1->newMoveDraw();

    // czasteczka zawor WY 2
    color = PistonParticleColor(-angle_engine - PI );
    glColor3d(color.x, color.y, color.z);

    M=mValveParticle(vec3(piston2_x, valveParticle_y_out, valveParticle_z_out), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveOut2->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveOut2->setMoveDirection(ValveParticleDir(angle_engine - (PI), valve_out));
    ParticleValveOut2->setPercentY(ValveParticlePercent(angle_engine - (PI), 0, valve_out));
    ParticleValveOut2->newMoveDraw();

    // czasteczka zawor WY 3
    color = PistonParticleColor(-angle_engine + (PI) );
    glColor3d(color.x, color.y, color.z);

    M=mValveParticle(vec3(piston3_x, valveParticle_y_out, valveParticle_z_out), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveOut3->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveOut3->setMoveDirection(ValveParticleDir(angle_engine + (PI), valve_out));
    ParticleValveOut3->setPercentY(ValveParticlePercent(angle_engine + (PI), 0, valve_out));
    ParticleValveOut3->newMoveDraw();

    // czasteczka zawor WY 4
    color = PistonParticleColor(-angle_engine );
    glColor3d(color.x, color.y, color.z);

    M=mValveParticle(vec3(piston4_x, valveParticle_y_out, valveParticle_z_out), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    ParticleValveOut4->setDirectionSpeed(abs(engine_speed_x) / 40);
    ParticleValveOut4->setMoveDirection(ValveParticleDir(angle_engine + (2*PI), valve_out));
    ParticleValveOut4->setPercentY(ValveParticlePercent(angle_engine + (2*PI), 0, valve_out));
    ParticleValveOut4->newMoveDraw();

    // czasteczka tlok 1
    color = PistonParticleColor(-angle_engine + (2*PI) );
    glColor3d(color.x, color.y, color.z);

    M=mPistonParticle(vec3(piston1_x, max_pistonParticle_y , 0));
    glLoadMatrixf(value_ptr(V*M));
    ParticlePiston1->setPercentY(PistonParticlePercent(angle_engine, 0.1f));
    ParticlePiston1->newMoveDraw();

    // czasteczka tlok 2
    color = PistonParticleColor(-angle_engine - PI );
    glColor3d(color.x, color.y, color.z);

    M=mPistonParticle(vec3(piston2_x, max_pistonParticle_y , 0));
    glLoadMatrixf(value_ptr(V*M));
    ParticlePiston2->setPercentY(PistonParticlePercent(angle_engine + PI, 0.1f));
    ParticlePiston2->newMoveDraw();

    // czasteczka tlok 3
    color = PistonParticleColor(-angle_engine + (PI) );
    glColor3d(color.x, color.y, color.z);

    M=mPistonParticle(vec3(piston3_x, max_pistonParticle_y , 0));
    glLoadMatrixf(value_ptr(V*M));
    ParticlePiston3->setPercentY(PistonParticlePercent(angle_engine - PI, 0.1f));
    ParticlePiston3->newMoveDraw();

    // czasteczka tlok 4
    color = PistonParticleColor(-angle_engine );
    glColor3d(color.x, color.y, color.z);

    M=mPistonParticle(vec3(piston4_x, max_pistonParticle_y , 0));
    glLoadMatrixf(value_ptr(V*M));
    ParticlePiston4->setPercentY(PistonParticlePercent(angle_engine + (2*PI), 0.1f));
    ParticlePiston4->newMoveDraw();

    // tryb obiektow
    glColor3d(0.3f, 0.3f,0.3f); //Ustaw kolor rysowania
    glEnable(GL_LIGHTING); //Włącz tryb cieniowania
    glDisable(GL_COLOR_MATERIAL); //Włącz śledzenie kolorów przez materiał

    // wal korbowy
    M=rotate(mat4(1),-angle_engine + (PI / 2),vec3(1.0f,0.0f,0.0f));
    glLoadMatrixf(value_ptr(V*M));
    wal_korbowy.DrawSolid(crackshaft_color);

    // tlok 1
    M = mPiston(piston1_x, 0, max_piston_y, angle_engine);
    glLoadMatrixf(value_ptr(V*M));
    tlok.DrawSolid(piston_color);

    // tlok 2
    M = mPiston(piston2_x, 0, max_piston_y, angle_engine + PI);
    glLoadMatrixf(value_ptr(V*M));
    tlok.DrawSolid(piston_color);

    // tlok 3
    M = mPiston(piston3_x, 0, max_piston_y, angle_engine + PI);
    glLoadMatrixf(value_ptr(V*M));
    tlok.DrawSolid(piston_color);

    // tlok 4
    M = mPiston(piston4_x, 0, max_piston_y, angle_engine);
    glLoadMatrixf(value_ptr(V*M));
    tlok.DrawSolid(piston_color);

    // korbowod 1
    M = mCrank(piston1_x, 0, max_crank_y, angle_engine);
    glLoadMatrixf(value_ptr(V*M));
    korbowod.DrawSolid(crank_color);

    // korbowod 2
    M = mCrank(piston2_x, 0, max_crank_y, angle_engine + PI);
    glLoadMatrixf(value_ptr(V*M));
    korbowod.DrawSolid(crank_color);

    // korbowod 3
    M = mCrank(piston3_x, 0, max_crank_y, angle_engine + PI);
    glLoadMatrixf(value_ptr(V*M));
    korbowod.DrawSolid(crank_color);

    // korbowod 4
    M = mCrank(piston4_x, 0, max_crank_y, angle_engine);
    glLoadMatrixf(value_ptr(V*M));
    korbowod.DrawSolid(crank_color);

    // zawor we 1
    M = mValve(piston1_x, valve_z_in, min_valve_y, valve_delta_y, angle_engine , valve_in);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor we 2
    M = mValve(piston2_x, valve_z_in, min_valve_y, valve_delta_y, angle_engine - PI, valve_in);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor we 3
    M = mValve(piston3_x, valve_z_in, min_valve_y, valve_delta_y, angle_engine + (PI), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor we 4
    M = mValve(piston4_x, valve_z_in, min_valve_y, valve_delta_y, angle_engine + (2*PI), valve_in);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor wy 1
    M = mValve(piston1_x, valve_z_out, min_valve_y, valve_delta_y, angle_engine , valve_out);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor wy 2
    M = mValve(piston2_x, valve_z_out, min_valve_y, valve_delta_y, angle_engine - (PI), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor wy 3
    M = mValve(piston3_x, valve_z_out, min_valve_y, valve_delta_y, angle_engine + (PI), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);

    // zawor wy 4
    M = mValve(piston4_x, valve_z_out, min_valve_y, valve_delta_y, angle_engine + (2*PI), valve_out);
    glLoadMatrixf(value_ptr(V*M));
    zawor.DrawSolid(valve_color);


    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "Four Stroke Engine MS MW Animation", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.


	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err=glewInit()) != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące


	float angle_engine=0.0f; //Aktualny kąt obrotu obiektu wokół osi x
	float angle_y=0.0f; //Aktualny kąt obrotu obiektu wokół osi y
	float angle_x=0.0f;
	float pos_y = 2.0f;
	float pos_x = 0;
	float dimension = 15;


	glfwSetTime(0); //Wyzeruj timer

	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
	    // predkosc silnika
        engine_speed_x -= delta_engine_speed_x*glfwGetTime();
	    if (engine_speed_x > -(PI / 4)) engine_speed_x = -PI / 4;
	    else if (engine_speed_x < -(5 * PI)) engine_speed_x = -5 * PI;

        // obsluga zmiany parametrow kamery i silnika
	    angle_engine+=engine_speed_x*glfwGetTime();
	    angle_y += camera_y*glfwGetTime();
	    angle_x += camera_x*glfwGetTime();
	    pos_y += delta_pos_y*glfwGetTime();
	    pos_x += delta_pos_x*glfwGetTime();
        dimension += delta_dimension*glfwGetTime();

        // ograniczenia
        if (dimension < 5) dimension = 5;
        else if (dimension > 40) dimension = 40;

        if (angle_x > (PI / 3)) angle_x = PI / 3;
        else if (angle_x < -(PI / 3)) angle_x = -PI / 3;

        if (pos_x > 2) pos_x = 2;
        else if (pos_x < -2) pos_x = -2;

        if (pos_y > 5) pos_y = 5;
        else if (pos_y < -1) pos_y = -1;

	    glfwSetTime(0); //Wyzeruj timer
		drawScene(window,angle_engine,angle_x, angle_y, pos_x, pos_y, dimension); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	delete ParticleValveIn1;
	delete ParticleValveIn2;
	delete ParticleValveIn3;
	delete ParticleValveIn4;
	delete ParticleValveOut1;
	delete ParticleValveOut2;
	delete ParticleValveOut3;
	delete ParticleValveOut4;
	delete ParticlePiston1;
	delete ParticlePiston2;
	delete ParticlePiston3;
	delete ParticlePiston4;
	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
