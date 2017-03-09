#pragma once

#include <math.h>

namespace Esgi
{
	struct Mat4
	{
		float m[16];

		//Mat4(float diagonal);

		void Identity();
		void MakeScale(float sx, float sy, float sz);
		void SetLocalScale(float sx, float sy, float sz);
		void MakeRotation2D(float angleDegree);
		void MakeTranslation(float tx, float ty, float tz);
		void SetTranslate(float tx, float ty, float tz);

		void Perspective(float fovy, float aspect, float nearZ, float farZ);
		void MakeRotationY(float angleDegree);
		public:
			Mat4 MultiplicationMatrice(const Esgi::Mat4& A, Esgi::Mat4& B);
	};
	
	void Mat4::Identity()
	{
		memset(m, 0, sizeof(float) * 16);
		m[0] = 1.0f; m[5] = 1.0f; m[10] = 1.0f; m[15] = 1.0f;
		// equivalent a MakeScale(1.0f, 1.0f, 1.0f);
	}
	
	void Mat4::MakeScale(float sx, float sy, float sz)
	{
		memset(m, 0, sizeof(float) * 16);
		m[0] = sx; m[5] = sy; m[10] = sz; m[15] = 1.0f;
	}

	void Mat4::SetLocalScale(float sx, float sy, float sz)
	{
		m[0] *= sx; m[5] *= sy; m[10] *= sz;
	}

	void Mat4::MakeRotation2D(float angleDegree)
	{
		float theta = angleDegree * (3.14159265359f / 180.0f);
		Identity();
		m[0] = cos(theta);
		m[1] = sin(theta);
		m[4] = -sin(theta);
		m[5] = cos(theta);
	}

	void Mat4::MakeTranslation(float tx, float ty, float tz)
	{
		Identity();
		SetTranslate(tx, ty, tz);
	}

	void Mat4::SetTranslate(float tx, float ty, float tz)
	{
		m[12] = tx; m[13] = ty; m[14] = tz;
	}

	void Mat4::Perspective(float fovy, float aspectRatio, float nearZ, float farZ)
	{
		// Projection Matrix
		float fovy_rad = fovy * (3.14159265359f / 180.f);
		float d = 1.0f / tan(fovy_rad * 0.5f);	// Distance focale

		float range = 1.0f / (nearZ - farZ);

		Identity();
		m[0] = d / aspectRatio;
		m[5] = d;
		m[10] = (nearZ + farZ)*range;
		m[11] = -1.0f;
		m[14] = (2.0f * nearZ * farZ)*range;
		m[15] = 0.0f;
	}

	void Mat4::MakeRotationY(float angleDegree)
	{
		float theta = angleDegree * (3.14159265359f / 180.0f);
		Identity();
		m[0] = cos(theta);
		m[2] = -sin(theta);
		m[8] = sin(theta);
		m[10] = cos(theta);
	}

}; // Esgi

Esgi::Mat4 MultiplicationMatrice(const Esgi::Mat4& A, Esgi::Mat4& B)
{
	// Mij = Sum(k) Aik * Bkj
	Esgi::Mat4 M;
	memset(M.m, 0, sizeof(float) * 16);
	// les boucles j et i sont inversees car on itere
	// colonne apres colonne, mais en memoire les donnees
	// sont justement representees en colonne d'abord 
	// (column major) cad colonne apres colonne
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				M.m[i + j*4] += A.m[i + k*4] * B.m[k+ j*4];
			}
		}
	}

	return M;
}

Esgi::Mat4 operator*(const Esgi::Mat4& A, Esgi::Mat4& B)
{
	// Mij = Sum(k) Aik * Bkj
	Esgi::Mat4 M;
	memset(M.m, 0, sizeof(float) * 16);
	// les boucles j et i sont inversees car on itere
	// colonne apres colonne, mais en memoire les donnees
	// sont justement representees en colonne d'abord 
	// (column major) cad colonne apres colonne
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			for (int k = 0; k < 4; k++) {
				M.m[i + j*4] += A.m[i + k*4] * B.m[k+ j*4];
			}
		}
	}

	return M;
}

