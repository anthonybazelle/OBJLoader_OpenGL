
#include "EsgiShader.h"
#include "GL/glew.h"
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

void EsgiShader::DestroyProgram()
{
	glDetachShader(m_ProgramObject, m_VertexShader);
	glDetachShader(m_ProgramObject, m_FragmentShader);
	glDeleteProgram(m_ProgramObject);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);
}

bool checkProgram(GLuint program)
{
	int didLink = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &didLink);
	if (!didLink)
	{
		char logBuffer[4096];
		int len = 0;
		glGetProgramInfoLog(program, 4096, &len, logBuffer);
#ifdef _WIN32
		OutputDebugStringA(logBuffer);
#endif
		std::cout << logBuffer << std::endl;
	}
	return didLink;
}

bool EsgiShader::CreateProgram()
{
	m_ProgramObject = glCreateProgram();
	glAttachShader(m_ProgramObject, m_VertexShader);
	glAttachShader(m_ProgramObject, m_FragmentShader);
	glLinkProgram(m_ProgramObject);
	checkProgram(m_ProgramObject);
	
	return true;
}

uint32_t EsgiShader::CompileShader(uint32_t type, const char* code)
{
	uint32_t shaderObject = glCreateShader(type);
	if (shaderObject != 0) {
		glShaderSource(shaderObject, 1, &code, nullptr);
		glCompileShader(shaderObject);
	}
	return shaderObject;
}

bool checkShader(GLuint shader)
{
	int didCompile = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &didCompile);
	if (!didCompile)
	{
		char logBuffer[4096];
		int len = 0;
		glGetShaderInfoLog(shader, 4096, &len, logBuffer);
#ifdef _WIN32
		OutputDebugStringA(logBuffer);
#endif
		std::cout << logBuffer << std::endl;
	}
	return didCompile;
}


bool EsgiShader::LoadVertexShader(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	auto len = ftell(file);
	rewind(file);
	auto buffer = new char[len+1];
	fread(buffer, len, 1, file);
	buffer[len] = '\0'; // = 0
	m_VertexShader = CompileShader(GL_VERTEX_SHADER, buffer);
	delete[] buffer;

	checkShader(m_VertexShader);

	return (m_VertexShader != 0);
}

bool EsgiShader::LoadFragmentShader(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	fseek(file, 0, SEEK_END);
	auto len = ftell(file);
	rewind(file);
	auto buffer = new char[len+1];
	fread(buffer, len, 1, file);
	buffer[len] = '\0';
	m_FragmentShader = CompileShader(GL_FRAGMENT_SHADER, buffer);
	delete[] buffer;
	
	checkShader(m_FragmentShader);

	return (m_FragmentShader != 0);
}