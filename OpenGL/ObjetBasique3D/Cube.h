#ifndef DEF_CUBE
#define DEF_CUBE


#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <windows.h>
#endif

#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif

#include "freeglut.h"
#include "../common/mat4.h"
#include <cstdint>
#include <iostream>


// Classe Cube

class Cube
{
    public:

    Cube(float taille);
    ~Cube();

    void afficher(Esgi::Mat4 &projection, Esgi::Mat4 &modelview, uint32_t shaderId);


    private:

    float m_vertices[108];
    float m_couleurs[108];
};

#endif
