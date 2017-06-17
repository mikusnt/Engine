#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <conio.h>
#include <algorithm>

using namespace std;

string nameOBJ;
string tempfilepathOBJ = "temp.obj";
string filepathOBJ;
string filepathC;


struct Model
{
    int vertices;
    int positions;
    int texels;
    int normals;
    int faces;
};

Model getOBJinfo(string fp)
{
    Model model = {0};

    // Open OBJ file
    ifstream inOBJ;
    inOBJ.open(fp.c_str());
    if(!inOBJ.good())
    {
        cout << "ERROR OPENING OBJ FILE" << endl;
        exit(1);
    }

    // Read OBJ file
    while(!inOBJ.eof())
    {
        string line;
        getline(inOBJ, line);
        string type = line.substr(0,2);

        if(type.compare("v ") == 0)
            model.positions++;
        else if(type.compare("vt") == 0)
            model.texels++;
        else if(type.compare("vn") == 0)
            model.normals++;
        else if(type.compare("f ") == 0)
            model.faces++;
    }

    model.vertices = model.faces*3;

    // Close OBJ file
    inOBJ.close();

    return model;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

void createTempFilePathOBJ(string fp) {
    fstream in;
    fstream out;
    string temp;
    in.open(fp.c_str(), ios::in);
    out.open(tempfilepathOBJ.c_str(), ios::out);
    while(!in.eof()) {
        getline(in, temp);
        temp = ReplaceAll(temp, "//", "/1/");
        out << temp << endl;
    }
    in.close();
    out.close();
}

void deleteTempFilePathOBJ(string fp) {
    remove(fp.c_str());
}

void extractOBJdata(string fp, float positions[][3], float texels[][2], float normals[][3], int faces[][9])
{
    // Counters
    int p = 0;
    int t = 0;
    int n = 0;
    int f = 0;

    // Open OBJ file
    ifstream inOBJ;
    inOBJ.open(fp.c_str());
    if(!inOBJ.good())
    {
        cout << "ERROR OPENING OBJ FILE" << endl;
        exit(1);
    }

    // Read OBJ file
    while(!inOBJ.eof())
    {
        string line;
        getline(inOBJ, line);

        string type = line.substr(0,2);

        // Positions
        if(type.compare("v ") == 0)
        {
           // cout << line << endl;
            // Copy line for parsing
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);

            // Extract tokens
            strtok(l, " ");
            for(int i=0; i<3; i++) {
                positions[p][i] = atof(strtok(NULL, " "));
            }
            // Wrap up
            delete[] l;
            p++;
        }

        // Texels
        else if(type.compare("vt") == 0)
        {
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);

            strtok(l, " ");
            for(int i=0; i<2; i++)
                texels[t][i] = atof(strtok(NULL, " "));

            delete[] l;
            t++;
        }

        // Normals
        else if(type.compare("vn") == 0)
        {
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);

            strtok(l, " ");
            for(int i=0; i<3; i++)
                normals[n][i] = atof(strtok(NULL, " "));

            delete[] l;
            n++;
        }

        // Faces
        else if(type.compare("f ") == 0)
        {
            //cout << line << endl;
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);

            strtok(l, " ");
            for(int i=0; i<9; i++)
                faces[f][i] = atof(strtok(NULL, "/ "));
                //cout << atof(strtok(NULL, "/ ")) << 'a' << endl;
                //cout << endl;

            delete[] l;
            f++;
        }
    }

    // Close OBJ file
    inOBJ.close();
}

void writeH(string fp, string name, Model model)
{
    // Create H file
    ofstream outH;
    outH.open(fp.c_str());
    if(!outH.good())
    {
        cout << "ERROR CREATING H FILE" << endl;
        exit(1);
    }

    // Write to H file
    outH << "// This is a .h file for the model: " << name << endl;
    outH << endl;

    // Write statistics
    outH << "// Positions: " << model.positions << endl;
    outH << "// Texels: " << model.texels << endl;
    outH << "// Normals: " << model.normals << endl;
    outH << "// Faces: " << model.faces << endl;
    outH << "// Vertices: " << model.vertices << endl;
    outH << endl;

    // Write declarations
    outH << "const int " << name << "Vertices;" << endl;
    outH << "const float " << name << "Positions[" << model.vertices*3 << "];" << endl;
    outH << "const float " << name << "Texels[" << model.vertices*2 << "];" << endl;
    outH << "const float " << name << "Normals[" << model.vertices*3 << "];" << endl;
    outH << endl;

    // Close H file
    outH.close();
}

void writeCvertices(string fp, string name, Model model)
{
    // Create C file
    ofstream outC;
    outC.open(fp.c_str());
    if(!outC.good())
    {
        cout << "ERROR CREATING C FILE" << endl;
        exit(1);
    }

    // Vertices
    outC << "const int " << name << "Vertices = " << model.vertices << ";" << endl;
    outC << endl;

    // Close C file
    outC.close();
}

void writeCpositions(string fp, string name, Model model, int faces[][9], float positions[][3])
{
    // Append C file
    ofstream outC;
    outC.open(fp.c_str(), ios::app);

    // Positions
    outC << "const float " << name << "Positions[" << model.vertices*3 << "] = " << endl;
    outC << "{" << endl;
    for(int i=0; i<model.faces; i++)
    {
        int vA = faces[i][0] - 1;
        int vB = faces[i][3] - 1;
        int vC = faces[i][6] - 1;

        outC << positions[vA][0] << ", " << positions[vA][1] << ", " << positions[vA][2] << ", " << endl;
        outC << positions[vB][0] << ", " << positions[vB][1] << ", " << positions[vB][2] << ", " << endl;
        outC << positions[vC][0] << ", " << positions[vC][1] << ", " << positions[vC][2] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;

    // Close C file
    outC.close();
}

void writeCtexels(string fp, string name, Model model, int faces[][9], float texels[][2])
{
    // Append C file
    ofstream outC;
    outC.open(fp.c_str(), ios::app);

    // Texels
    outC << "const float " << name << "Texels[" << model.vertices*2 << "] = " << endl;
    outC << "{" << endl;
    for(int i=0; i<model.faces; i++)
    {
        int vtA = faces[i][1] - 1;
        int vtB = faces[i][4] - 1;
        int vtC = faces[i][7] - 1;

        outC << texels[vtA][0] << ", " << texels[vtA][1] << ", " << endl;
        outC << texels[vtB][0] << ", " << texels[vtB][1] << ", " << endl;
        outC << texels[vtC][0] << ", " << texels[vtC][1] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;

    // Close C file
    outC.close();
}

void writeCnormals(string fp, string name, Model model, int faces[][9], float normals[][3])
{
    // Append C file
    ofstream outC;
    outC.open(fp.c_str(), ios::app);

    // Normals
    outC << "const float " << name << "Normals[" << model.vertices*3 << "] = " << endl;
    outC << "{" << endl;
    for(int i=0; i<model.faces; i++)
    {
        int vnA = faces[i][2] - 1;
        int vnB = faces[i][5] - 1;
        int vnC = faces[i][8] - 1;

        outC << normals[vnA][0] << ", " << normals[vnA][1] << ", " << normals[vnA][2] << ", " << endl;
        outC << normals[vnB][0] << ", " << normals[vnB][1] << ", " << normals[vnB][2] << ", " << endl;
        outC << normals[vnC][0] << ", " << normals[vnC][1] << ", " << normals[vnC][2] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;

    // Close C file
    outC.close();
}

int main(void)
{
    cout << "Podaj nazwe pliku (bez .obj): ";
    cin >> nameOBJ;
    filepathOBJ = nameOBJ + ".obj";
    filepathC = nameOBJ + ".h";
    createTempFilePathOBJ(filepathOBJ);
    Model model = getOBJinfo(tempfilepathOBJ);
    cout << "Model Info" << endl;
    cout << "Positions: " << model.positions << endl;
    cout << "Texels: " << model.texels << endl;
    cout << "Normals: " << model.normals << endl;
    cout << "Faces: " << model.faces << endl;
    cout << "Vertices: " << model.vertices << endl;
    //return 0;
        // Model Data
    float positions[model.positions][3];    // XYZ
    float texels[model.texels][2];          // UV
    float normals[model.normals][3];        // XYZ
    int faces[model.faces][9];              // PTN PTN PTN
        extractOBJdata(tempfilepathOBJ, positions, texels, normals, faces);

    deleteTempFilePathOBJ(tempfilepathOBJ);

    cout << "Model Data" << endl;
    cout << "P1: " << positions[0][0] << "x " << positions[0][1] << "y " << positions[0][2] << "z" << endl;
    cout << "T1: " << texels[0][0] << "u " << texels[0][1] << "v " << endl;
    cout << "N1: " << normals[0][0] << "x " << normals[0][1] << "y " << normals[0][2] << "z" << endl;
    cout << "F1v1: " << faces[0][0] << "p " << faces[0][1] << "t " << faces[0][2] << "n" << endl;

     //tworzenie plików .c i .h z modelu .obj
    writeCvertices(filepathC, nameOBJ, model);
    writeCpositions(filepathC, nameOBJ, model, faces, positions);
    writeCtexels(filepathC, nameOBJ, model, faces, texels);
    writeCnormals(filepathC, nameOBJ, model, faces, normals);

    getch();
}
