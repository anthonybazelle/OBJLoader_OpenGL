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
#include "../common/shader.h"
#include "OBJLoader.h"
#include "texture.h"

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
	GLuint TextureID;
	GLuint Texture;
	Input *input;
	Camera *camera;
	Cube *cube;
	Grid *grid;

	vector<Vector3> objVertices;
	vector<Vector2> objUvs;
	vector<Vector3> objNormals; 
	COBJLoader* pObjLoader;
	Shader* pShader;
	GLuint programID;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint LightID;
	GLuint normalbuffer; 
	void menu(int num);
	int mainMenu;

	bool bCulling;
	bool bTexture;
	int bLight;
	int bLine;

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
	void createMenu();
	static void menuCallBack(int i); 
	void changeCamera();
	void hideShowGrid();
	void fillLineDraw();
	void backculling();
	void onOffTexture();
	void onOffIllumination();
	void lambert();
	void blinn();
	void blinnPhong();
};

#endif // ! SCENE






