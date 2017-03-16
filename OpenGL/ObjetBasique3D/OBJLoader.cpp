#include "OBJLoader.h"


COBJLoader::COBJLoader(void)
{
}


COBJLoader::~COBJLoader(void)
{
}

string get_directory(string s)
{
    string s1="",s2="";
    for(unsigned int i=0;i<s.size();i++)
    {
        if(s[i]=='/'||s[i]=='\\')
        {
			std::string s3(s2.c_str() + '/');
            s1 += s3;
            s2="";
        }
        else
            s2+=s[i];
    }
    return s1;
}

vector<string> splitSpace(string s)
{
    vector<string> ret;

    string s1="";

    for(unsigned int i=0;i<s.size();i++)
	{
        if(s[i]==' '||i==s.size()-1)
		{
            if(i==s.size()-1)
                s1+=s[i];
            ret.push_back(s1);
            s1="";
        }
        else
            s1+=s[i];
    }
    return ret;
}


void COBJLoader::extractMTLdata(string fp)
{/*
    int m = 0;
    int d = 0;
    int s = 0;
    int a = 0;*/

	string* materials = new string[1];
    ifstream inMTL;
    inMTL.open(fp);
    if(!inMTL.good())
    {
        cout << "ERROR OPENING MTL FILE" << endl;
        exit(1);
    }
    
    while(!inMTL.eof())
    {
        string line;
        getline(inMTL, line);
        string type = line.substr(0,2);
        
        // Nom material
        if(type.compare("ne") == 0)
        {
            string l = "newmtl ";
            materials[0] = line.substr(l.size());
            //m++;
        }

		// Ambiantes
        else if(type.compare("Ka") == 0)
        {
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);
            
            strtok(l, " ");
            for(int i=0; i<3; i++)
                ambiants[i] = atof(strtok(NULL, " "));
            
            delete[] l;
            //d++;
        }
        
        // Diffuses
        else if(type.compare("Kd") == 0)
        {
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);
            
            strtok(l, " ");
            for(int i=0; i<3; i++)
                diffuses[i] = atof(strtok(NULL, " "));
            
            delete[] l;
            //d++;
        }
        
        // Speculaires
        else if(type.compare("Ks") == 0)
        {
            char* l = new char[line.size()+1];
            memcpy(l, line.c_str(), line.size()+1);
            
            strtok(l, " ");
            for(int i=0; i<3; i++)
                speculars[i] = atof(strtok(NULL, " "));
            
            delete[] l;
            //s++;
        }
    }
    
    inMTL.close();
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
		else if(strcmp( lineHeader, "mtllib" ) == 0 )
		{//fichier MTL et si c'est la première frame (comme ça on ne charge pas plusieurs fois le même MTL et la même texture)
			string s = "";
			//fscanf(file, "%s\n", &s);
			extractMTLdata("cylinder.mtl");
		}/*
		else if(strcmp( lineHeader, "u" ) == 0 )//utiliser un MTL
		{
			curname=ligne.substr(7);
		}*/
	}
}