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

void Scene::menuCallBack(int i)
{
	Scene::currentInstance->menu(i);
}


void Scene::changeCamera()
{

}

void Scene::hideShowGrid()
{

}

void Scene::fillLineDraw()
{
	bLine = !bLine;
}

void Scene::backculling()
{
	if(!bCulling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
		
	bCulling = !bCulling;
}

void Scene::onOffTexture()
{
	if(!bTexture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
		
	bTexture = !bTexture;
}

void Scene::onOffIllumination()
{
	if(bLight == 1)
		bLight = 0;
	else
		bLight = 1;
}

void Scene::createMenu()
{

	// ATTENDS JE SUIS AU TEL AVEC UN COLLEGUE
	mainMenu = glutCreateMenu(Scene::menuCallBack);

	glutAddMenuEntry("Exit", 0);
	glutAddMenuEntry("Change camera        C", 1);
	glutAddMenuEntry("Hide/Show grid       G", 2);
	glutAddMenuEntry("Fill/Line draw       N", 3);
	glutAddMenuEntry("On/Off backculling   V", 4);
	glutAddMenuEntry("On/Off illumination  I", 6);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// On traite ici le choix de l'utilisateur dans le menu contextuel
void Scene::menu(int num) {
	switch (num)
	{
	case 0:
		glutDestroyWindow(windowId);
		exit(0);
		break;
	case 1:
		input->checkKeyboardInputs('c', 0, 0);
		break;
	case 2:
		input->checkKeyboardInputs('g', 0, 0);
		break;
	case 3:
		input->checkKeyboardInputs('n', 0, 0);
		break;
	case 4:
		input->checkKeyboardInputs('v', 0, 0);
		break;
	case 5:
		input->checkKeyboardInputs('t', 0, 0);
		break;
	case 6:
		input->checkKeyboardInputs('i', 0, 0);
		break;
	case 7:
		input->checkKeyboardInputs('l', 0, 0);
		break;
	case 8:
		input->checkKeyboardInputs('b', 0, 0);
		break;
	case 9:
		input->checkKeyboardInputs('p', 0, 0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
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
	createMenu();


	glewInit();
	glutDisplayFunc(Scene::drawCallBack);
	glutIdleFunc(Scene::updateCallBack);
	
	input->init();
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");
	pObjLoader = new COBJLoader();
	g_BasicShader.LoadVertexShader("basicLight.vs");
	g_BasicShader.LoadFragmentShader("basicLight.fs");
	//g_BasicShader.LoadGeometryShader("grid.gs");
	g_BasicShader.CreateProgram();
	programID = g_BasicShader.GetProgram();
	bool res = pObjLoader->LoadOBJ("../data/cylinder.obj", objVertices, objUvs, objNormals);
	Texture = loadDDS("../data/uvmap.DDS");
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
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

			// Clear the screen
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
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
	//worldMatrix.MakeRotationY(TimeInSeconds*72.0f);
	// Use our shader
	glUseProgram(programID);

	// Compute the MVP matrix from keyboard and mouse input
	//computeMatricesFromInputs();
	Mat4 ProjectionMatrix = projectionMatrix;
	Mat4 ViewMatrix = modelviewMatrix;
	Mat4 ModelMatrix = worldMatrix;
	Mat4 MVP = (ProjectionMatrix.mult(ViewMatrix)).mult(ModelMatrix);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, MVP.m);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, worldMatrix.m);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, modelviewMatrix.m);

	
	GLuint KaID = glGetUniformLocation(programID, "Ka");
	GLuint KdID = glGetUniformLocation(programID, "Kd");
	GLuint KsID = glGetUniformLocation(programID, "Ks");
	
	if(bTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);
	}
	/*glUniform3f(KaID, pObjLoader->ambiants[0], pObjLoader->ambiants[1], pObjLoader->ambiants[2]);
	glUniform3f(KdID, pObjLoader->diffuses[0], pObjLoader->diffuses[1], pObjLoader->diffuses[2]);
	glUniform3f(KsID, pObjLoader->speculars[0], pObjLoader->speculars[1], pObjLoader->speculars[2]);

	Vector3 lightPos;
	lightPos.x = 4;
	lightPos.y = 4;
	lightPos.z = 4;

	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	GLuint boolLight = glGetUniformLocation(programID, "bLight");
	glUniform1i(boolLight, bLight);
	*/
	// 1rst attribute buffer : vertices
	auto vertexPosition_modelspace = glGetAttribLocation(programID, "vertexPosition_modelspace");
	glEnableVertexAttribArray(vertexPosition_modelspace);

	int nbLines = 16;
	int espace = 1;
	int sizeGrid = nbLines * 4;

	Vector3 *grid;
	grid = (Vector3*) malloc(sizeGrid * sizeof(Vector3));

	glVertexAttribPointer(
		vertexPosition_modelspace,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		grid);

	int x = -nbLines / 2;
	for (int i = 0; i < nbLines*2; i++)
	{
		Vector3 v;
		if (i % 2 == 0)
		{
			v.x = x;
			v.y = nbLines / 2;
			v.z = 0;
			grid[i] = v;
		}
		else
		{
			v.x = x;
			v.y = -nbLines / 2;
			v.z = 0;
			grid[i] = v;
			x+= espace;
		}
	}
	int y = -nbLines / 2;
	for (int i = nbLines*2; i < nbLines*4; i++)
	{
		Vector3 v;
		if (i % 2 == 0)
		{
			v.y = y;
			v.x = nbLines / 2;
			v.z = 0;
			grid[i] = v;
		}
		else
		{
			v.y = y;
			v.x = -nbLines / 2;
			v.z = 0;
			grid[i] = v;
			y+= espace;
		}
	}
	
	
	/*glVertexAttribPointer(
		vertexPosition_modelspace,                  
		3,                  
		GL_FLOAT,           
		GL_FALSE,           
		0,                 
		objVertices.data()           
	);*/
	/*
	// 2nd attribute buffer : UVs
	auto vertexUV = glGetAttribLocation(programID, "vertexUV");
	glEnableVertexAttribArray(vertexUV);
	
	glVertexAttribPointer(
		vertexUV,                                
		2,                                
		GL_FLOAT,                         
		GL_FALSE,                         
		0,                                
		objUvs.data()                         
	);

	// 3rd attribute buffer : normals
	auto vertexNormal_modelspace = glGetAttribLocation(programID, "vertexNormal_modelspace");
	glEnableVertexAttribArray(vertexNormal_modelspace);
	//glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		vertexNormal_modelspace,                                
		3,                                
		GL_FLOAT,                         
		GL_FALSE,                         
		0,                                
		objNormals.data()                    
	);*/

	// Draw the triangles !
	/*if(bLine)
		glDrawArrays(GL_LINES, 0, objVertices.size());
	else
		glDrawArrays(GL_TRIANGLES, 0, objVertices.size() );*/

	glDrawArrays(GL_LINES, 0, sizeGrid);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);


	//draw poly

	/*glUseProgram(0);

	glViewport(0, 0, width, height);
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);


	auto program = g_BasicShader.GetProgram();
	glUseProgram(program);

	auto color_position = glGetAttribLocation(program, "a_Color");
	auto position_location = glGetAttribLocation(program, "a_Position");


	GLuint colorID = glGetUniformLocation(program, "myColor");
	glUniform4f(colorID, color[0], color[1], color[2], color[3]);*/


	/*if (state == DRAW)
	{
		if (!polygons->empty())
		{
			for (int i = 0; i < polygons->size(); i++)
			{
				polygons->at(i).recalculateBezierPoints(width, height);

				const maths::Point *bezierPoints = polygons->at(i).getBezierPoints()->data();
				unsigned int bezierSize = polygons->at(i).getBezierPoints()->size();

				//if (i == polygonSelected)
					//glUniform4f(colorID, 1.0, 0.0, 0.0, 1.0);
				//else
				//glUniform4f(colorID, color[0], color[1], color[2], color[3]);


				glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, bezierPoints);
				glEnableVertexAttribArray(vertexPosition_modelspace);

				glPointSize(5);

				glDrawArrays(GL_LINE_STRIP, 0, bezierSize);
				glDisableVertexAttribArray(vertexPosition_modelspace);

				if (pointSelected != -1 && i == polygonSelected)
				{
					const maths::Point *points = polygons->at(i).getPoints()->data();
					unsigned int size = pointSelected;

					glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, points);
					glEnableVertexAttribArray(vertexPosition_modelspace);
					glPointSize(5);
					glDrawArrays(GL_POINTS, 0, size);
					glDisableVertexAttribArray(vertexPosition_modelspace);

					//glUniform4f(colorID, 0.0, 1.0, 0.0, 1.0);
					size = 1;

					glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, &points[pointSelected]);
					glEnableVertexAttribArray(vertexPosition_modelspace);
					glPointSize(5);
					glDrawArrays(GL_POINTS, 0, size);
					glDisableVertexAttribArray(vertexPosition_modelspace);

					//glUniform4f(colorID, 1.0, 0.0, 0.0, 1.0);
					size = polygons->at(i).getPoints()->size() - pointSelected - 1;

					if (size > 0)
					{
						glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, &points[pointSelected + 1]);
						glEnableVertexAttribArray(vertexPosition_modelspace);
						glPointSize(5);
						glDrawArrays(GL_POINTS, 0, size);
						glDisableVertexAttribArray(vertexPosition_modelspace);
					}
				}
				else
				{
					const maths::Point *points = polygons->at(i).getPoints()->data();
					unsigned int size = polygons->at(i).getPoints()->size();

					glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, points);
					glEnableVertexAttribArray(vertexPosition_modelspace);

					glPointSize(5);

					glDrawArrays(GL_POINTS, 0, size);
					glDisableVertexAttribArray(vertexPosition_modelspace);
				}
			}
		}

	}

	else if (state == ENTER_POLYGON)
	{

		for (int i = 0; i < polygons->size() - 1; i++)
		{
			//polygons->at(i).recalculateBezierPoints();

			const maths::Point *bezierPoints = polygons->at(i).getBezierPoints()->data();
			unsigned int bezierSize = polygons->at(i).getBezierPoints()->size();

			glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, bezierPoints);
			glEnableVertexAttribArray(vertexPosition_modelspace);

			glPointSize(5);

			glDrawArrays(GL_LINE_STRIP, 0, bezierSize);
			glDisableVertexAttribArray(vertexPosition_modelspace);

			const maths::Point *points = polygons->at(i).getPoints()->data();
			unsigned int size = polygons->at(i).getPoints()->size();

			glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, points);
			glEnableVertexAttribArray(vertexPosition_modelspace);

			glPointSize(5);

			glDrawArrays(GL_POINTS, 0, size);
			glDisableVertexAttribArray(vertexPosition_modelspace);
		}

		const maths::Point *points = polygons->back().getPoints()->data();
		unsigned int size = polygons->back().getPoints()->size();

		for (int i = 0; i < size; i++)
		{
			glVertexAttribPointer(vertexPosition_modelspace, 3, GL_FLOAT, GL_FALSE, 0, &points[i]);
			glEnableVertexAttribArray(vertexPosition_modelspace);

			glPointSize(10);

			glDrawArrays(GL_POINTS, 0, 1);
			glDisableVertexAttribArray(vertexPosition_modelspace);
		}
	}*/

	glUseProgram(0);

	glutSwapBuffers();
}

void Scene::scalePoint(maths::Polygon *poly, float ratio)
{
	float pivotX = 0;
	float pivotY = 0;

	int nbPoints = poly->getPoints()->size();

	for (int i = 0; i < nbPoints; i++)
	{
		pivotX += poly->getPoints()->at(i).x;
		pivotY += poly->getPoints()->at(i).y;
	}

	pivotX = pivotX / nbPoints;
	pivotY = pivotY / nbPoints;

	// TODO : Test ratio

	for (int i = 0; i <poly->getPoints()->size(); i++)
	{
		maths::Point p = poly->getPoints()->at(i);
		// application formule
		maths::Point p2;
		p2.x = p.x - pivotX;
		p2.y = p.y - pivotY;

		p2.x *= ratio;
		p2.y *= ratio;

		p.x = pivotX + p2.x;
		p.y = pivotY + p2.y;

		poly->setPoint(p, i);
	}

}

void Scene::translatePoint(maths::Polygon *poly, float translateX, float translateY)
{
	for (int i = 0; i <poly->getPoints()->size(); i++)
	{
		maths::Point p = poly->getPoints()->at(i);
		// application formule
		p.x = p.x + translateX;
		p.y = p.y + translateY;

		poly->setPoint(p, i);
	}
}

void Scene::rotate_point(maths::Polygon *poly, float angle)
{
	float s = sin(angle);
	float c = cos(angle);

	// On calcul la moyenne des coordonnées des sommets du polygon
	float pivotX = 0;
	float pivotY = 0;

	int nbPoints = poly->getPoints()->size();

	for (int i = 0; i < nbPoints; i++)
	{
		pivotX += poly->getPoints()->at(i).x;
		pivotY += poly->getPoints()->at(i).y;
	}

	pivotX = pivotX / nbPoints;
	pivotY = pivotY / nbPoints;

	for (int i = 0; i <poly->getPoints()->size(); i++)
	{
		maths::Point p = poly->getPoints()->at(i);
		// application formule
		p.x -= pivotX;
		p.y -= pivotY;

		// rotation du point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		p.x = xnew + pivotX;
		p.y = ynew + pivotY;

		poly->setPoint(p, i);
	}


}

Scene::Scene(int w, int h)
{
	height = h;
	width = w;
	Scene::currentInstance = this;
	input = new Input(this);
	int distY = width / 2 / 35;

	camera = new Camera(this, Esgi::Vec3(0, 0, -distY), Esgi::Vec3(0, 0, 0), Esgi::Vec3(0, 1, 0));
	cube = new Cube(2);
	grid = new Grid(10);
	bCulling = false;
	bTexture = false;
	bLine = false;

	pointSelected = -1;
	polygonSelected = -1;
	polygons = new std::vector<maths::Polygon>();
	activeTransformation = NO_TRANS;
}

void Scene::changeState(State s)
{
	if (state == s)
		return;
	state = s;
	switch (state)
	{
	case ENTER_POLYGON:
		polygons->push_back(*(new maths::Polygon()));
		break;
		break;
	case DRAW:
		break;
	default:
		break;
	}
}

State Scene::getState()
{
	return state;
}

void Scene::addPoint(maths::Point p)
{
	switch (state)
	{
	case ENTER_POLYGON:
		if (!polygons->empty())
		{
			//std::cout << "point added x=" << p.x << " y=" << p.y << std::endl;
			p.y = height - p.y;
			p.x -= width / 2;
			p.y -= height / 2;
			p.x /= width / 2;
			p.y /= height / 2;
			//std::cout << "point normalized x=" << p.x << " y=" << p.y << std::endl;

			polygons->back().addPoint(p);
		}
		break;
	case DRAW:
		break;
	case FILL:
		break;
	case COLOR:
		break;
	default:
		break;
	}

}

Scene::~Scene()
{
	//g_BasicShader.DestroyProgram();

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteTextures(1, &Texture);
	//glDeleteProgram(programID);
	g_BasicShader.GetProgram();
	glDeleteTextures(1, &TexObj);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);
}
