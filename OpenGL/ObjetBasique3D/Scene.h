#ifndef  DEF_SCENE
#define DEF_SCENE

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
#include "../common/EsgiShader.h"
#include "Input.h"
#include "Camera.h"
#include "Cube.h"
#include "Grid.h"

// format des vertices : X, Y, Z, ?, ?, ?, ?, ? = 8 floats

class Scene
{
	static Scene* currentInstance;
	static void drawCallBack();
	static void updateCallBack();
	EsgiShader g_BasicShader;
	int width;
	int height;
	int windowId;

	uint32_t dragonVertexCount;
	uint32_t dragonIndexCount;

	GLuint VBO;	// identifiant du Vertex Buffer Object
	GLuint IBO;	// identifiant du Index Buffer Object
	GLuint TexObj; // identifiant du Texture Object

	Input *input;
	Camera *camera;
	Cube *cube;
	Grid *grid;

public:
	Camera* getCamera();
	Input* getInput();
	void lauchOpenGLLoop();
	void initOpenGl(int argc, const char* argv);
	void mainLoop();
	void update();
	float getHeight();
	float getWidth();
	Scene(int w, int h);
	~Scene();
};

#endif // ! SCENE






