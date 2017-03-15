#include "Scene.h"


Scene* Scene::currentInstance = nullptr;

void Scene::drawCallBack()
{
	Scene::currentInstance->mainLoop();
}

void Scene::updateCallBack()
{
	Scene::currentInstance->update();
}

Camera* Scene::getCamera()
{
	return camera;
}

Input* Scene::getInput()
{
	return input;
}

void Scene::lauchOpenGLLoop()
{
	glutMainLoop();
}

void Scene::initOpenGl(int argc, const char* argv)
{

	glutInit(&argc, (char **)argv);
	// defini deux color buffers (un visible, un cache) RGBA
	// GLUT_DEPTH alloue egalement une zone memoire pour le depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(width, height);
	windowId = glutCreateWindow("Controle continu"); 

#ifdef FREEGLUT
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	glewInit();

	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	//g_BasicShader.LoadGeometryShader("grid.gs");
	g_BasicShader.CreateProgram();

	glutDisplayFunc(Scene::drawCallBack);
	glutIdleFunc(Scene::updateCallBack);

	input->init();
}

void Scene::update()
{
	glutPostRedisplay();
}

float Scene::getHeight()
{
	return height;
}

float Scene::getWidth()
{
	return width;
}

void Scene::mainLoop()
{
	camera->deplacer(input);

	glViewport(0, 0, width, height);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	Esgi::Mat4 projectionMatrix, modelviewMatrix;
	modelviewMatrix.Identity();

	projectionMatrix.Perspective(70.0, (double)width / height, 0.1, 1000.0);
	camera->lookAt(modelviewMatrix);

	//cube->afficher(projectionMatrix, modelviewMatrix, g_BasicShader.GetProgram());

	//grid->afficher(projectionMatrix, modelviewMatrix, g_BasicShader.GetProgram());


	int TimeSinceAppStartedInMS = glutGet(GLUT_ELAPSED_TIME);
	float TimeInSeconds = TimeSinceAppStartedInMS / 1000.0f;

	Esgi::Mat4 translationMatrix;
	translationMatrix.MakeTranslation(0.0f, 0.0f, -20.0f);

	Esgi::Mat4 rotationMatrix;
	rotationMatrix.Identity();
	rotationMatrix.MakeRotationY(TimeInSeconds*72.0f);

	//Esgi::Mat4 worldMatrix = translationMatrix.mult(rotationMatrix);
	Esgi::Mat4 worldMatrix;
	worldMatrix.Identity();

	glUseProgram(g_BasicShader.GetProgram());


	auto color_position = glGetAttribLocation(g_BasicShader.GetProgram(), "a_Color");
	//glVertexAttrib3f(program, .0f, 1.f, .0f);
	auto position_location = glGetAttribLocation(g_BasicShader.GetProgram(), "a_Position");

	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, objVertices.data());
	glEnableVertexAttribArray(position_location);

	glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 0, objUvs.data());
	glEnableVertexAttribArray(color_position);


	// Envoi des matrices
	glUniformMatrix4fv(glGetUniformLocation(g_BasicShader.GetProgram(), "world"), 1, GL_FALSE, worldMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(g_BasicShader.GetProgram(), "projection"), 1, GL_FALSE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(g_BasicShader.GetProgram(), "modelview"), 1, GL_FALSE, modelviewMatrix.m);

	glUseProgram(0);


	glutSwapBuffers();


	/*float g_tab[] = {
		-0.7f, -.5f,
		1.0f,  0.0f,  0.0f,
		-.5f, .5f,
		0.0f,  1.0f,  0.0f,
		.5f, -.5f,
		0.0f,  0.0f,  1.0f,
		.7f, .5f,
		1.0f,  0.0f,  0.0f
	};

	glViewport(0, 0, width, height);
	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	//glVertexAttrib3f(program, .0f, 1.f, .0f);
	auto position_location = glGetAttribLocation(program, "a_Position");

	glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), &g_tab[2]);
	glEnableVertexAttribArray(color_position);

	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), g_tab);
	glEnableVertexAttribArray(position_location);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(position_location);
	
	glUseProgram(0);

	glutSwapBuffers();*/
}

Scene::Scene(int w, int h)
{
	height = h;
	width = w;
	Scene::currentInstance = this;
	input = new Input(this);
	camera = new Camera(this, Esgi::Vec3(3, 3, 3), Esgi::Vec3(0, 0, 0), Esgi::Vec3(0, 1, 0));
	cube = new Cube(2);
	grid = new Grid(10);

	pObjLoader = new COBJLoader();
	bool res = pObjLoader->LoadOBJ("../data/Monkey.obj", objVertices, objUvs, objNormals);

}

Scene::~Scene()
{
	g_BasicShader.DestroyProgram();

	glDeleteTextures(1, &TexObj);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);
}
