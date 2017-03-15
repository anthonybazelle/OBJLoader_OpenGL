#include "Cube.h"


// Constructeur et Destructeur

Cube::Cube(float taille)
{
    // Division de la taille

    taille /= 2;


    // Vertices temporaires

    float verticesTmp[] = {-taille, -taille, -taille,   taille, -taille, -taille,   taille, taille, -taille,     // Face 1
                           -taille, -taille, -taille,   -taille, taille, -taille,   taille, taille, -taille,     // Face 1

                           taille, -taille, taille,   taille, -taille, -taille,   taille, taille, -taille,       // Face 2
                           taille, -taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 2

                           -taille, -taille, taille,   taille, -taille, taille,   taille, -taille, -taille,      // Face 3
                           -taille, -taille, taille,   -taille, -taille, -taille,   taille, -taille, -taille,    // Face 3

                           -taille, -taille, taille,   taille, -taille, taille,   taille, taille, taille,        // Face 4
                           -taille, -taille, taille,   -taille, taille, taille,   taille, taille, taille,        // Face 4

                           -taille, -taille, -taille,   -taille, -taille, taille,   -taille, taille, taille,     // Face 5
                           -taille, -taille, -taille,   -taille, taille, -taille,   -taille, taille, taille,     // Face 5

                           -taille, taille, taille,   taille, taille, taille,   taille, taille, -taille,         // Face 6
                           -taille, taille, taille,   -taille, taille, -taille,   taille, taille, -taille};      // Face 6


    // Couleurs temporaires

    float couleursTmp[] = {1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1
                           1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,           // Face 1

                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2
                           0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,           // Face 2

                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3
                           0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,           // Face 3

                           1.0, 0.0, 1.0,   1.0, 0.0, 1.0,   1.0, 0.0, 1.0,           // Face 4
							1.0, 0.0, 1.0,   1.0, 0.0, 1.0,   1.0, 0.0, 1.0,           // Face 4

                           1.0, 1.0, 0.0,   1.0, 1.0, 0.0,   1.0, 1.0, 0.0,           // Face 5
                           1.0, 1.0, 0.0,   1.0, 1.0, 0.0,   1.0, 1.0, 0.0,           // Face 5

                           0.0, 1.0, 1.0,   0.0, 1.0, 1.0,   0.0, 1.0, 1.0,           // Face 6
                           0.0, 1.0, 1.0,   0.0, 1.0, 1.0,   0.0, 1.0, 1.0};          // Face 6


    // Copie des valeurs dans les tableaux finaux

    for(int i(0); i < 108; i++)
    {
        m_vertices[i] = verticesTmp[i];
        m_couleurs[i] = couleursTmp[i];
    }
}


Cube::~Cube()
{

}


// Méthodes

void Cube::afficher(Esgi::Mat4 &projection, Esgi::Mat4 &modelview, uint32_t shaderId)
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
	Esgi::Mat4 worldMatrix = rotationMatrix;

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

        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Désactivation des tableaux

        glDisableVertexAttribArray(color_position);
        glDisableVertexAttribArray(position_location);


    // Désactivation du shader

    glUseProgram(0);
}

