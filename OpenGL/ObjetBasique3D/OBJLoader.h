#pragma once

#ifndef CLASS_OBJ_LOADER
#define CLASS_OBJ_LOADER
#endif

#ifndef MAT4_H
#define MAT4_H
#include "../common/mat4.h"
#endif

#include <vector>
#include <ios>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
//#define M_PI 3.14159265359f

using namespace std;
using namespace Esgi;

#ifndef VECTOR3
#define VECTOR3
struct Vector3
{
	float x;
	float y;
	float z;
};
#endif

struct Material{
     char* m_name;
     float* m_Ka;
	 float* m_Kd;
	 float* m_Ks;
     float m_d, m_Tr;
     float m_Ns;
     float m_illum;
     char* m_map_Kd;

	 Material::Material()
	 {
		 m_d = 0;
		 m_illum = 0;
		 m_Ka = 0;
		 m_Kd = 0;
		 m_Ks = 0;
		 m_map_Kd = "";
		 m_name = "";
		 m_Ns = 0;
		 m_Tr = 0;
	 }

	 Material::Material(char* name, float* Ka, float* Kd, float* Ks, float d, float Tr, float Ns, float illum, char* map_kd)
	 {
		 m_d = d;
		 m_illum = illum;
		 m_Ka = Ka;
		 m_Kd = Kd;
		 m_Ks = Ks;
		 m_map_Kd = map_kd;
		 m_name = name;
		 m_Ns = Ns;
		 m_Tr = Tr;
	 }
};

#ifndef VECTOR2
#define VECTOR2
struct Vector2
{
	float x;
	float y;
};
#endif
using namespace std;
class COBJLoader
{
public:
	COBJLoader(void);
	~COBJLoader(void);

	float ambiants[3];
	float diffuses[3];
	float speculars[3];

	bool LoadOBJ(const char * path, std::vector<Vector3> &out_vertices, std::vector<Vector2> &out_uvs, std::vector<Vector3> &out_normals);
	void LoadMTL(std::string pathMtl);
	void extractMTLdata(string fp);
private:
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;
	std::vector<Vector3> colors;
	std::vector<Material> materiaux;

};

