#pragma once

#ifndef CLASS_OBJ_LOADER
#define CLASS_OBJ_LOADER
#endif

#ifndef MAT4_H
#define MAT4_H
#include "../common/mat4.h"
#endif

#include <vector>

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

#ifndef VECTOR2
#define VECTOR2
struct Vector2
{
	float x;
	float y;
};
#endif

class COBJLoader
{
public:
	COBJLoader(void);
	~COBJLoader(void);

	bool LoadOBJ(const char * path, std::vector<Vector3> &out_vertices, std::vector<Vector2> &out_uvs, std::vector<Vector3> &out_normals);

private:
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector3> temp_vertices;
	std::vector<Vector2> temp_uvs;
	std::vector<Vector3> temp_normals;


};

