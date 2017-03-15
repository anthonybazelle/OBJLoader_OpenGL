
#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#include <windows.h>
#endif

#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#endif

#include "freeglut.h"

#include "../common/EsgiShader.h"
#include "../common/mat4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

// format des vertices : X, Y, Z, ?, ?, ?, ?, ? = 8 floats
#include "../data/DragonData.h"

#if _MSC_VER
uint32_t dragonVertexCount = _countof(DragonVertices);
uint32_t dragonIndexCount = _countof(DragonIndices);
#endif

EsgiShader g_BasicShader;

int width = 800;
int height = 600;

GLuint VBO;	// identifiant du Vertex Buffer Object
GLuint IBO;	// identifiant du Index Buffer Object
GLuint TexObj; // identifiant du Texture Object

bool Initialize()
{
	glewInit();

	g_BasicShader.LoadVertexShader("basicLight.vs");
	g_BasicShader.LoadFragmentShader("basicLight.fs");
	g_BasicShader.CreateProgram();

	glGenTextures(1, &TexObj);
	glBindTexture(GL_TEXTURE_2D, TexObj);
	int w, h, c; // largeur, hauteur, #de composantes du fichier
	uint8_t* bitmapRGBA = stbi_load("../data/dragon.png",
					&w, &h, &c, STBI_rgb_alpha);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0	// destination
				, GL_RGBA, GL_UNSIGNED_BYTE, bitmapRGBA); // source
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(bitmapRGBA);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 1. Creation d'un Buffer Object (BO) pour stocker les sommets
	// d'ou le nom Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);
	// 2. Binding = specifier explicitement que l'on va manipuler un BO 
	// 'target' indique le type de BO que l'on va manipuler
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 3 et 4. Allocation d'une zone memoire et transfert des donnees
	glBufferData(GL_ARRAY_BUFFER, dragonVertexCount * sizeof(float)
								, DragonVertices, GL_STATIC_DRAW);
	// GL_STATIC_DRAW car le triangle n'est pas modifie par le CPU

	// rendu indexe
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dragonIndexCount * sizeof(GLushort)
									, DragonIndices, GL_STATIC_DRAW);
	
	// le fait de specifier 0 comme BO desactive l'usage des BOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

void Terminate()
{
	glDeleteTextures(1, &TexObj);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);

	g_BasicShader.DestroyProgram();
}

void update()
{
	glutPostRedisplay();
}

void animate()
{
	int TimeSinceAppStartedInMS = glutGet(GLUT_ELAPSED_TIME);
	float TimeInSeconds = TimeSinceAppStartedInMS / 1000.0f;

	glViewport(0, 0, width, height);
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	//glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	uint32_t texUnit = 0;
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, TexObj);
	auto textureLocation = glGetUniformLocation(program, "u_Texture");
	glUniform1i(textureLocation, texUnit);

	// UNIFORMS
	Esgi::Mat4 rotationMatrix;
	rotationMatrix.MakeRotationY(TimeInSeconds*72.0f);
	Esgi::Mat4 translationMatrix;
	translationMatrix.MakeTranslation(0.0f, 0.0f, -20.0f);//-100.0f*abs(sin(TimeInSeconds / 2)));
	
	//Esgi::Mat4 worldMatrix = translationMatrix.mult(rotationMatrix);
	Esgi::Mat4 worldMatrix = Esgi::Mat4();
	worldMatrix =  translationMatrix;

	auto world_location = glGetUniformLocation(program, "u_WorldMatrix");
	glUniformMatrix4fv(world_location, 1, GL_FALSE, worldMatrix.m); 
	
	Esgi::Mat4 projectionMatrix;

	float w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	float aspectRatio = w / h;	// facteur d'aspect
	float fovy = 45.0f;			// degre d'ouverture du field of view
	projectionMatrix.Perspective(fovy, aspectRatio, nearZ, farZ);

	auto projection_location = glGetUniformLocation(program, "u_ProjectionMatrix");
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, projectionMatrix.m);

	auto time_location = glGetUniformLocation(program, "u_Time");
	glUniform1f(time_location, TimeInSeconds);

	auto lightDir_location = glGetUniformLocation(program
						, "u_PositionOrDirection");
	float lightVector[4] = { 0.0, 1.0, 0.0, 0.0 };
	glUniform4fv(lightDir_location, 1, lightVector);

	// ATTRIBUTES
	auto position_location = glGetAttribLocation(program, "a_Position");
	auto texcoords_location = glGetAttribLocation(program, "a_TexCoords");
	auto normal_location = glGetAttribLocation(program, "a_Normal");

	// Le fait de specifier la ligne suivante va modifier le fonctionnement interne de glVertexAttribPointer
	// lorsque GL_ARRAY_BUFFER != 0 cela indique que les donnees sont stockees sur le GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// le VBO contient des vertex au format suivant:
	// X,Y,Z,NX,NY,NZ,U,V
	struct Vertex
	{
		float x, y, z;		// offset = 0
		float nx, ny, nz;	// offset = 3
		float u, v;			// offset = 6
	};
	// 1er cas : on a l'adresse du tableau original
	Vertex* v = (Vertex*)DragonVertices;
	float *adresse = &v->u;
	size_t adresseRelative = (adresse - DragonVertices) * sizeof(float);
	// 2nd cas: on ne dispose de l'adresse du tableau DragonVertices
	size_t rel = offsetof(Vertex, u);

	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE
		, 8 * sizeof(float)
		, reinterpret_cast<const void *>(0 * sizeof(float)));
	glEnableVertexAttribArray(position_location);
	// on interprete les 3 valeurs inconnues comme du RGB
	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE
		, 8 * sizeof(float)
		, reinterpret_cast<const void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(normal_location);
	glVertexAttribPointer(texcoords_location, 2, GL_FLOAT, GL_FALSE
		, 8 * sizeof(float)
		, reinterpret_cast<const void *>(6 * sizeof(float)));
	glEnableVertexAttribArray(texcoords_location);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, dragonIndexCount, GL_UNSIGNED_SHORT, nullptr);
	
	glDisableVertexAttribArray(position_location);
	glUseProgram(0);

	glutSwapBuffers();
}

int main(int argc, const char* argv[])
{
	// passe les parametres de la ligne de commande a glut
	glutInit(&argc, (char**)argv);
	// defini deux color buffers (un visible, un cache) RGBA
	// GLUT_DEPTH alloue egalement une zone memoire pour le depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	// positionne et dimensionne la fenetre
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	// creation de la fenetre ainsi que du contexte de rendu
	glutCreateWindow("Objet Basique 3D");

#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	if (Initialize() == false)
		return -1;

	glutIdleFunc(update);
	glutDisplayFunc(animate);
	glutMainLoop();

	Terminate();

	return 1;
}