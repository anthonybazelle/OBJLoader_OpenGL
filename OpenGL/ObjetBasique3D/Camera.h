#ifndef DEF_CAMERA
#define DEF_CAMERA

#include "Vec3.h"
#include "../common/mat4.h"
#define _USE_MATH_DEFINES
#include <cmath>

// Classe

class Scene;
class Input;

enum CameraState{
	FREE, FPS, ORBITALE
};

class Camera
{
public:

	Camera(Scene *s);
	Camera(Scene *s, Esgi::Vec3 position, Esgi::Vec3 pointCible, Esgi::Vec3 axeVertical); 
	void deplacer(Input* input);
	void setPointcible(Esgi::Vec3 pointCible);
	void lookAt(Esgi::Mat4 &modelview);
	void orienter(int xRel, int yRel);
	void setState(CameraState s); 
	void calculateRadius(); 
	void reinnit();
	
	~Camera();


private:
	Scene *m_scene;
	CameraState state;
	float m_phi;
	float m_theta;
	float m_realPhi;
	float m_realTheta;
	float m_savedTheta;
	float m_rotationTheta;
	float m_radius;
	Esgi::Vec3 m_orientation;

	Esgi::Vec3 m_axeVertical;
	Esgi::Vec3 m_deplacementLateral;

	Esgi::Vec3 m_position;
	Esgi::Vec3 m_firstPosition;
	Esgi::Vec3 m_pointCible;
	Esgi::Vec3 m_savPointCible;


	Esgi::Vec3 m_initialPos;
	Esgi::Vec3 m_initialTheta;
	Esgi::Vec3 m_initialPointCible;
};

#endif