#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

using namespace std;

#include "../common/EsgiShader.h"

EsgiShader g_BasicShader;

int m_iNbVertice = 0;
// Les indices mis en commentaire correspondent à ceux de la représentation de l'exercice
GLfloat tabVertice[17*17*2];
GLint tabIndex[17*17];

// Bande lu d'en haut à gauche vers en bas à droite
void fillIndexExo1()
{
	int index1 = 0;
	int index2 = m_iNbVertice;

	for (int i = 0; i < m_iNbVertice - 1; i++)
	{
		for (int j = 0; j < m_iNbVertice * 2; j += 2)
		{
			tabIndex[i * (m_iNbVertice*2) + j] = index1;
			tabIndex[i * (m_iNbVertice*2) + j + 1] = index2;

			index1++;
			index2++;
		}
	}
}

void fillIndexExo2()
{
	int index1 = 0;
	int index2 = m_iNbVertice;
	int cmptDecal = 0;
	bool b = false;
	for (int i = 0; i < m_iNbVertice - 1; i++)
	{
		for (int j = 0; j < m_iNbVertice * 2; j += 2)
		{
			if(index2 != m_iNbVertice*m_iNbVertice - 1)
			{
				if(index1 != 0)
				{
					if(index2 == 16 + (i +1) * m_iNbVertice)
					{
						tabIndex[i * (m_iNbVertice*2) + j + cmptDecal] = index1;
						tabIndex[i * (m_iNbVertice*2) + j + 1 + cmptDecal] = index2;
						cmptDecal+=2;
						b = true;
					}
				}
			}

			if(b == true)
			{
				tabIndex[i * (m_iNbVertice*2) + j + cmptDecal] = index2;
				index1++;
				tabIndex[i * (m_iNbVertice*2) + j + 1 + cmptDecal] = index1;
				index2++;
				b = false;
			}
			else
			{
				tabIndex[i * (m_iNbVertice*2) + j + cmptDecal] = index1;
				tabIndex[i * (m_iNbVertice*2) + j + 1 + cmptDecal] = index2;

				index1++;
				index2++;
			}
		}
	}
}

void renderExo1()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, m_iNbVertice * (m_iNbVertice*2) * sizeof(GLfloat), tabVertice, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);

	for (int i = 0; i < m_iNbVertice; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_iNbVertice*2) * sizeof(GLuint), tabIndex + i * (m_iNbVertice*2), GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLE_STRIP, m_iNbVertice * (m_iNbVertice*2), GL_UNSIGNED_INT, nullptr);
	}
	glutSwapBuffers();
	
}

void renderExo2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glBufferData(GL_ARRAY_BUFFER, m_iNbVertice * m_iNbVertice * 2 * sizeof(GLfloat), tabVertice, GL_STATIC_DRAW);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_iNbVertice * (m_iNbVertice*2 + 2) * sizeof(GLuint), tabIndex, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLE_STRIP, m_iNbVertice * m_iNbVertice * 2, GL_UNSIGNED_INT, nullptr);
	
	glutSwapBuffers();
}

void fillVertices()
{
	
	float incrementX = -.9f;
	int increment = 0;

	for (int i = 0; i < m_iNbVertice; i++)
	{
		float y = 0.9f - i * 1.8f / (m_iNbVertice-1);
		for (int j = 0; j < m_iNbVertice; j++)
		{
			tabVertice[increment] = -0.9f + j * 1.8f / (m_iNbVertice-1);
			tabVertice[increment + 1] = y;
			increment += 2;
		}
	}
}

int main(int argc, char* argv[]) {

	// Paramètres :
	m_iNbVertice = 17;

	// 0 : Exo 1 
	// 1 : Exo 2
	bool bExo = 1;

	fillVertices();

	if(bExo == 0)
	{
		fillIndexExo1();
	}
	else
	{
		fillIndexExo2();
	}

	// On initialise GLUT et la fenêtre
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	// Créé la fenêtre avec le titre
	glutCreateWindow("Super quadrilatère - Anthony BAZELLE 4A I3DJV");

	// On indique à GLUT notre fonction de rendu sur laquelle il doit boucler. On peut choisir le rendu de l'exercice 1 ou rendu exercice 2 en fonction de bExo
	if(bExo == 0)
	{
		glutDisplayFunc(renderExo1);
	}
	else
	{
		glutDisplayFunc(renderExo2);
	}
	
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	glutMainLoop();
	return 0;
}