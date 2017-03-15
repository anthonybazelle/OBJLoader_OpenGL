#include "Camera.h"
#include "Input.h"
#include "Scene.h"

Camera::Camera(Scene *s) : m_scene(s), m_phi(0.0), m_theta(0.0), m_orientation(), m_axeVertical(0, 0, 1), m_deplacementLateral(), m_position(), m_pointCible()
{

}

Camera::Camera(Scene *s, Esgi::Vec3 position, Esgi::Vec3 pointCible, Esgi::Vec3 axeVertical) : m_scene(s), m_phi(0.0), m_theta(0.0), m_orientation(), m_axeVertical(axeVertical),
m_deplacementLateral(), m_position(position), m_pointCible(pointCible)
{
	state = FREE;
	m_savedTheta = 0;
	m_savPointCible = pointCible;
	setPointcible(pointCible);
}

void Camera::setPointcible(Esgi::Vec3 pointCible)
{
	// Calcul du vecteur orientation

	m_orientation = Esgi::Vec3::soustraction(m_pointCible, m_position);
	m_orientation = Esgi::Vec3::normalize(m_orientation);


	// Si l'axe vertical est l'axe X

	if (m_axeVertical.getX() == 1.0)
	{
		// Calcul des angles

		m_phi = asin(m_orientation.getX());
		m_theta = acos(m_orientation.getY() / cos(m_phi));

		if (m_orientation.getY() < 0)
			m_theta *= -1;
	}


	// Si c'est l'axe Y

	else if (m_axeVertical.getY() == 1.0)
	{
		// Calcul des angles

		m_phi = asin(m_orientation.getY());
		m_theta = acos(m_orientation.getZ() / cos(m_phi));

		if (m_orientation.getZ() < 0)
			m_theta *= -1;
	}


	// Sinon c'est l'axe Z

	else
	{
		// Calcul des angles

		m_phi = asin(m_orientation.getX());
		m_theta = acos(m_orientation.getZ() / cos(m_phi));

		if (m_orientation.getZ() < 0)
			m_theta *= -1;
	}


	// Conversion en degrés

	m_phi = m_phi * 180 / M_PI;
	m_theta = m_theta * 180 / M_PI;

	m_realPhi = m_phi;
	m_realTheta = m_theta;
	m_savedTheta = m_theta;
}


void Camera::lookAt(Esgi::Mat4 &modelview)
{

	Esgi::Vec3 X, Y, Z;

	Z = Esgi::Vec3::soustraction(m_position ,m_pointCible);
	Z = Esgi::Vec3::normalize(Z);
	Y = m_axeVertical;
	X = Esgi::Vec3::crossProduct(Y, Z);

	Y = Esgi::Vec3::crossProduct(Z, X);

	Y = Esgi::Vec3::normalize(Y);
	X = Esgi::Vec3::normalize(X);

	modelview.m[0] = X.getX();
	modelview.m[4] = X.getY();
	modelview.m[8] = X.getZ();
	modelview.m[12] = -Esgi::Vec3::dotProduct(X, m_position);
	modelview.m[1] = Y.getX();
	modelview.m[5] = Y.getY();
	modelview.m[9] = Y.getZ();
	modelview.m[13] = -Esgi::Vec3::dotProduct(Y, m_position);
	modelview.m[2] = Z.getX();
	modelview.m[6] = Z.getY();
	modelview.m[10] = Z.getZ();
	modelview.m[14] = -Esgi::Vec3::dotProduct(Z, m_position);
	modelview.m[3] = 0;
	modelview.m[4] = 0;
	modelview.m[11] = 0;
	modelview.m[15] = 1.0f;
}

void Camera::deplacer(Input* input)
{
	// Gestion de l'orientation

	float coef = .1f;

	if (input->mouseHasMove() )
	{
		//récupérer le mouse relatif à la dernière fois je penses
		orienter(input->getRelMouseX(), input->getRelMouseY());
		input->SetMouseMove(false);
	}
		


	// Avancée de la caméra

	if (input->isLetterPressed('z'))
	{
		m_position = Esgi::Vec3::addition(m_position , Esgi::Vec3::multiplication(m_orientation , coef));
		if (state != ORBITALE)
			m_pointCible = Esgi::Vec3::addition(m_position, m_orientation);
	}


	// Recul de la caméra

	if (input->isLetterPressed('s'))
	{
		m_position = Esgi::Vec3::soustraction(m_position, Esgi::Vec3::multiplication(m_orientation, coef));
		if (state != ORBITALE)
			m_pointCible = Esgi::Vec3::addition(m_position, m_orientation);
	}


	// Déplacement vers la gauche

	if (input->isLetterPressed('q'))
	{
		m_position = Esgi::Vec3::addition(m_position, Esgi::Vec3::multiplication(m_deplacementLateral, coef));
		if (state != ORBITALE)
			m_pointCible = Esgi::Vec3::addition(m_position, m_orientation);
	}


	// Déplacement vers la droite

	if (input->isLetterPressed('d'))
	{
		m_position = Esgi::Vec3::soustraction(m_position, Esgi::Vec3::multiplication(m_deplacementLateral, coef));
		if(state!=ORBITALE)
			m_pointCible = Esgi::Vec3::addition(m_position, m_orientation);
	}
}

Camera::~Camera()
{
}


void Camera::orienter(int xRel, int yRel)
{
	// Modification des angles
	//std::cout << "m_realPhi=" << m_realPhi << "     m_realTheta=" << m_realTheta << std::endl;

	//std::cout << "m_phi=" << m_phi << "     m_theta=" << m_theta << std::endl;


	m_realPhi += -yRel * 0.5;
	m_realTheta += -xRel * 0.5;

	m_phi = m_realPhi;
	m_theta = m_realTheta;

	// Limitation de l'angle phi par rapport au calcul de la normale au plan

	if (m_phi > 89.0)
		m_phi = 89.0;

	else if (m_phi < -89.0)
		m_phi = -89.0;

	if (state == FPS)
	{
		if (m_theta > m_savedTheta+89.0)
			m_theta = m_savedTheta + 89.0;

		else if (m_theta < m_savedTheta -89.0)
			m_theta = m_savedTheta - 89.0;
	}

	// Conversion des angles en radian

	float phiRadian = m_phi * M_PI / 180;
	float thetaRadian = m_theta * M_PI / 180;

	// Si l'axe vertical est l'axe X

	if (m_axeVertical.getX() == 1.0)
	{
		// Calcul des coordonnées sphériques

		m_orientation.setX(sin(phiRadian));
		m_orientation.setY(cos(phiRadian) * cos(thetaRadian));
		m_orientation.setZ(cos(phiRadian) * sin(thetaRadian));
	}


	// Si c'est l'axe Y

	else if (m_axeVertical.getY() == 1.0)
	{
		// Calcul des coordonnées sphériques

		m_orientation.setX(cos(phiRadian) * sin(thetaRadian));
		m_orientation.setY(sin(phiRadian));
		m_orientation.setZ(cos(phiRadian) * cos(thetaRadian));
	}


	// Sinon c'est l'axe Z

	else
	{
		// Calcul des coordonnées sphériques

		m_orientation.setX(cos(phiRadian) * cos(thetaRadian));
		m_orientation.setY(cos(phiRadian) * sin(thetaRadian));
		m_orientation.setZ(sin(phiRadian));
	}

	// Calcul de la normale

	m_deplacementLateral = Esgi::Vec3::crossProduct(m_axeVertical, m_orientation);
	m_deplacementLateral = Esgi::Vec3::normalize(m_deplacementLateral);


	// Calcul du point ciblé pour OpenGL

	m_pointCible = Esgi::Vec3::addition(m_position, m_orientation);

}

void Camera::setState(CameraState s)
{
	state = s;
}
