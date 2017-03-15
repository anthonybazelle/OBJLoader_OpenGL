#pragma once

#ifndef ESGI_MAT4_H
#define ESGI_MAT4_H

#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>

namespace Esgi
{
	class Mat4
	{
	public :
		float m[16];
		void Identity();
		void MakeScale(float sx, float sy, float sz);
		void SetLocalScale(float sx, float sy, float sz);
		void MakeRotation2D(float angleDegree);
		void MakeTranslation(float tx, float ty, float tz);
		void SetTranslate(float tx, float ty, float tz);

		void Perspective(float fovy, float aspect, float nearZ, float farZ);
		void MakeRotationY(float angleDegree);

		Mat4 Mat4::mult(Mat4& B);
		
	};



}; // Esgi


#endif // ESGI_MAT4_H
