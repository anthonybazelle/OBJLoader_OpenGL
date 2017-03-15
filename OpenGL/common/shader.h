#ifndef SHADER_HPP
#define SHADER_HPP


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glew.h>

class Shader
{
public:
	Shader();
	GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
};
#endif
