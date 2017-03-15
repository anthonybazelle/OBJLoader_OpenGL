#include "Grid.h"


// Constructeur et Destructeur

Grid::Grid(float taille)
{
    // Division de la taille

    taille /= 2;


    // Vertices temporaires

    float verticesTmp[] = {-taille, 0, -taille,   
							-taille, 0, taille,   
							taille, 0, taille, 
							taille, 0, -taille}; 
	
	float couleursTmp[] = { 0, 0, 0,
							0, 0, 0,
							0, 0, 0,
							0, 0, 0 };

	for (int i(0); i < 12; i++)
	{
		m_vertices[i] = verticesTmp[i];
		m_couleurs[i] = couleursTmp[i];
	}

}


Grid::~Grid()
{

}


// Méthodes

void Grid::afficher(Esgi::Mat4 &projection, Esgi::Mat4 &modelview, uint32_t shaderId)
{
    // Activation du shader
	//std::cout << "afficher cube" << std::endl;

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

    glUseProgram(shaderId);


	auto color_position = glGetAttribLocation(shaderId, "a_Color");
	//glVertexAttrib3f(program, .0f, 1.f, .0f);
	auto position_location = glGetAttribLocation(shaderId, "a_Position");

	glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
	glEnableVertexAttribArray(position_location);

	glVertexAttribPointer(color_position, 3, GL_FLOAT, GL_FALSE, 0, m_couleurs);
	glEnableVertexAttribArray(color_position);


    // Envoi des matrices
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "world"), 1, GL_FALSE, worldMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, projection.m);
    glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelview"), 1, GL_FALSE, modelview.m);


    // Rendu

    glDrawArrays(GL_LINES, 0, 4);


    // Désactivation des tableaux

    glDisableVertexAttribArray(color_position);
    glDisableVertexAttribArray(position_location);


    // Désactivation du shader

    glUseProgram(0);
}

