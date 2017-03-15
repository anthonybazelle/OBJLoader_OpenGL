#include "OBJLoader.h"


COBJLoader::COBJLoader(void)
{
}


COBJLoader::~COBJLoader(void)
{
}

bool COBJLoader::LoadOBJ(const char * path, std::vector<Vector3> &out_vertices, std::vector<Vector2> &out_uvs, std::vector<Vector3> &out_normals)
{
	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	while( 1 ){
		char lineHeader[128];
		// On lit le premier mot de la ligne
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;

		if ( strcmp( lineHeader, "v" ) == 0 )
		{ // Dans le cas où l'on a 'v' on aura donc 3 float, que l'on récupère et que l'on ajoute dans le tableau de vertices
			Vector3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{ // Dans le cas où l'on a 'vt' on aura donc 2 float, que l'on récupère et que l'on ajoute dans le tableau des uv (textures)
			Vector2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{ // Dans le cas où l'on a 'vn' on aura donc 3 float, que l'on récupère et que l'on ajoute dans le tableau des normales (les normales correspondent aux normales des sommets en question)
			Vector3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{ // Dans le cas où l'on a 'f', on est sur une face donc on récupère les indices et on les traites en fonctions des 3 tableaux précédents qui ont dû bien se remplir
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
		
			if (matches != 9){
				printf("File can't be read : (Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

			for( unsigned int i=0; i < vertexIndices.size(); i++ )
			{
				unsigned int vertexIndex = vertexIndices[i];
				Vector3 vertex = temp_vertices[vertexIndex-1];
				out_vertices.push_back(vertex);
			}

			for( unsigned int i=0; i < uvIndices.size(); i++ )
			{
				unsigned int uvIndex = uvIndices[i];
				Vector2 uv = temp_uvs[uvIndex-1];
				out_uvs.push_back(uv);
			}

			for( unsigned int i=0; i < normalIndices.size(); i++ )
			{
				unsigned int normalIndex = normalIndices[i];
				Vector3 normal = temp_normals[normalIndex-1];
				out_normals.push_back(normal);
			}
		}
	}
}