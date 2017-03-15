#pragma once

#ifndef ESGI_VEC3
#define ESGI_VEC3

#define _USE_MATH_DEFINES
#include <cmath>

namespace Esgi
{
	class Vec3
	{
	public:
		Vec3();
		Vec3(float x, float y, float z);
		void setX(float x);
		void setY(float y);
		void setZ(float z);
		const float getX();
		const float getY();
		const float getZ();
		float getNorme();
		static Vec3 crossProduct( Vec3& v1,  Vec3& v2);
		static float dotProduct(Vec3 & v1, Vec3 & v2);
		static Vec3 normalize(Vec3& v1);
		static Vec3 addition(Vec3 &a, Vec3 &b);
		static Vec3 soustraction(Vec3 &a, Vec3 &b);
		static Vec3 multiplication(Vec3 &a, float b);

	private:
		float m_x, m_y, m_z;
		float m_norme;
		void calculateNorme();
	};

}

#endif // !ESGI_VEC3
