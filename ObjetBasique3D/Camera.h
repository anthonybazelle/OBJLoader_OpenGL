#ifndef DEF_CCAMERA
#define DEF_CCAMERA
#endif // !DEF_CCAMERA

#ifndef STRING_H
#define STRING_H
#include <string>
#endif

#ifndef MAT4_H
#define MAT4_H
#include "../common/mat4.h"
#endif

//#define M_PI 3.14159265359f

using namespace std;
using namespace Esgi;

#ifndef VECTOR3
#define VECTOR3
struct Vector3
{
	float x;
	float y;
	float z;
};
#endif

class CCamera
{
	public:
		CCamera(void);
		CCamera(Vector3 position, Vector3 target, Vector3 axeForward);
		~CCamera(void);

		void Orienter(int xRel, int yRel);
		void Deplacer(string action, int xRel, int yRel);
		void LookAt(Mat4 &modelview);
		Vector3 CrossProduct(Vector3 vec1, Vector3 vec2);
		Vector3 NormalizeVector(Vector3 vec);

		Vector3 AdditionVect(Vector3 vec1, Vector3 vec2);
		Vector3 SoustractionVect(Vector3 vec1, Vector3 vec2);
		Vector3 MultiplicationVect(Vector3 vec1, float f);
		float DotProduct(Vector3 vec1, Vector3 vec2);

		void SetXCam(int x);
		void SetYCam(int y);

	private:
		// Orientation
		float m_phi;
		float m_theta;
		Vector3 m_orientation;

		// Déplacements
		Vector3 m_axeForward;
		Vector3 m_axeLateral;

		// Camera
		Vector3 m_position;
		Vector3 m_target;

		int m_lastXCam;
		int m_lastYCam;

		int xCam;	
		int yCam;
};

