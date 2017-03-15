#include "Camera.h"

CCamera::CCamera(void)
{
	this->m_phi = 0.0f;
	this->m_theta = 0.0f;
	this->m_axeForward.x = .0f;
	this->m_axeForward.y = .0f;
	
	this->m_axeForward.z = 1.0f;

	this->m_lastXCam = 0;
	this->m_lastYCam = 0;
}

CCamera::CCamera(Vector3 position, Vector3 target, Vector3 axeForward)
{
	this->m_phi = 0.0f;
	this->m_theta = 0.0f;
	this->m_axeForward.x = .0f;
	this->m_axeForward.y = .0f;
	this->m_axeForward.z = 1.0f;

	this->m_position = position;
	this->m_target = target;
	this->m_axeForward = axeForward;

	this->m_phi = -35.26f;
	this->m_theta = -135.f;

	this->m_lastXCam = 0;
	this->m_lastYCam = 0;
}

CCamera::~CCamera(void)
{
}

void CCamera::SetXCam(int x)
{
	this->xCam = x;
}

void CCamera::SetYCam(int y)
{
	this->yCam = y;
}

void CCamera::Orienter(int xRel, int yRel)
{
	// - pour sens trigonométrique
	// Ajout de 0.5 pour réduire la vitesse de rotation
	this->m_phi += -yRel * 0.5f;
	this->m_theta += -xRel * 0.5f;

	// On limite la rotation de l'angle phi pour eviter d'avoir deux vecteur parallèle (colinéaire même car
	// on va utiliser l'axe y en guise de second vecteur pour calculer la normale au vecteur d'orientation, pour
	// permettre un déplacement horizontal de la camera)

	if(this->m_phi > 89.0f)
	{
		this->m_phi = 89.0f;
	}
	else if(this->m_phi < -89.0f)
	{
		this->m_phi = -89.0f;
	}

	float phiRadian = this->m_phi * 3.14159265359f / 180;
	float thetaRadian = this->m_theta * 3.14159265359f / 180;

	// On calcul ici les coordonnées sphériques en fonction de l'axe vertical utilisé

	// Pour X
	if(this->m_axeForward.x == 1.0)
	{
		this->m_orientation.x = sin(phiRadian);
		this->m_orientation.y = cos(phiRadian) * cos(thetaRadian);
		this->m_orientation.z = cos(phiRadian) * sin(thetaRadian);
	}
	// Pour Y
	else if(this->m_axeForward.y == 1.0)
	{
		this->m_orientation.x = cos(phiRadian) * sin(thetaRadian);
		this->m_orientation.y = sin(phiRadian);
		this->m_orientation.z = cos(phiRadian) * cos(thetaRadian);
	}
	// Pour Z
	else
	{
		this->m_orientation.x = cos(phiRadian) * cos(thetaRadian);
		this->m_orientation.y = cos(phiRadian) * sin(thetaRadian);
		this->m_orientation.z = sin(phiRadian);
	}

	// Calcul la normal, qui correspond au vecteur de déplacement latéral en fait
	this->m_axeLateral = CrossProduct(this->m_axeForward, this->m_orientation);
	this->m_axeLateral = NormalizeVector(this->m_axeLateral);
	
	// Calcul de ce que la camera va pointer
	this->m_target = (this->m_position, this->m_orientation);
}

void CCamera::Deplacer(string action, int xRel, int yRel)
{
	if(this->m_lastXCam == 0 && this->m_lastYCam == 0)
	{
		this->m_lastXCam = this->xCam;
		this->m_lastYCam =  this->yCam;
	}

	if(this->m_lastXCam != this->xCam || this->m_lastYCam !=  this->yCam)
	{
		this->m_lastXCam =  this->xCam;
		this->m_lastYCam =  this->yCam;
        Orienter(this->m_lastXCam, this->m_lastYCam);
	}

	if(action == "FORWARD")
	{
		this->m_position = AdditionVect(this->m_position, MultiplicationVect(this->m_orientation, 2.0f));
		this->m_target = AdditionVect(this->m_position, this->m_orientation);
	}
	
	if(action == "BACK")
	{
		this->m_position = SoustractionVect(this->m_position, MultiplicationVect(this->m_orientation, 0.5f));
		this->m_target = AdditionVect(this->m_position, this->m_orientation);
	}
	
	if(action == "LEFT")
	{
		this->m_position = AdditionVect(this->m_position, MultiplicationVect(this->m_axeLateral, 0.5f));
		this->m_target = AdditionVect(this->m_position, this->m_orientation);
	}
	
	if(action == "RIGHT")
	{
		this->m_position = SoustractionVect(this->m_position, MultiplicationVect(this->m_axeLateral, 0.5f));
		this->m_target = AdditionVect(this->m_position, this->m_orientation);
	}
}

Vector3 CCamera::CrossProduct(Vector3 vec1, Vector3 vec2)
{
	Vector3 vectResult;

	vectResult.x = (vec1.y * vec2.z) - (vec2.y * vec1.z);
	vectResult.y = (vec1.z * vec2.x) - (vec2.z * vec1.x);
	vectResult.z = (vec1.x * vec2.y) - (vec2.x * vec1.y);

	return vectResult;
}

Vector3 CCamera::NormalizeVector(Vector3 vec)
{
	Vector3 vectResult;

	float normeVec = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));

	vectResult.x = vec.x / normeVec;
	vectResult.y = vec.y / normeVec;
	vectResult.z = vec.z / normeVec;

	return vectResult;
}

void CCamera::LookAt(Mat4 &modelview)
{
	Vector3 X, Y, Z;

	Z = SoustractionVect(this->m_position ,this->m_target);
	Z = NormalizeVector(Z);
	Y = this->m_axeForward;
	X = CrossProduct(Y, Z);

	Y = CrossProduct(Z, X);

	Y = NormalizeVector(Y);
	X = NormalizeVector(X);

	modelview.m[0] = X.x;
	modelview.m[4] = X.y;
	modelview.m[8] = X.z;
	modelview.m[12] = -DotProduct(X, m_position);
	modelview.m[1] = Y.x;
	modelview.m[5] = Y.y;
	modelview.m[9] = Y.z;
	modelview.m[13] = -DotProduct(Y, m_position);
	modelview.m[2] = Z.x;
	modelview.m[6] = Z.y;
	modelview.m[10] = Z.z;
	modelview.m[14] = -DotProduct(Z, m_position);
	modelview.m[3] = 0;
	modelview.m[4] = 0;
	modelview.m[11] = 0;
	modelview.m[15] = 1.0f;
	}

Vector3 CCamera::AdditionVect(Vector3 vec1, Vector3 vec2)
{
	Vector3 vectResult;

	vectResult.x = vec1.x + vec2.x;
	vectResult.y = vec1.y + vec2.y;
	vectResult.z = vec1.z + vec2.z;

	return vectResult;
}

float CCamera::DotProduct(Vector3 vec1, Vector3 vec2)
{
	float res = (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
	return res;
}

Vector3 CCamera::SoustractionVect(Vector3 vec1, Vector3 vec2)
{
	Vector3 vectResult;

	vectResult.x = vec1.x - vec2.x;
	vectResult.y = vec1.y - vec2.y;
	vectResult.z = vec1.z - vec2.z;

	return vectResult;
}

Vector3 CCamera::MultiplicationVect(Vector3 vec, float f)
{
	Vector3 vectResult;

	vectResult.x = f * vec.x;
	vectResult.y = f * vec.y;
	vectResult.z = f * vec.z;

	return vectResult;
}