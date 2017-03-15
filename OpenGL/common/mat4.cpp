#include "mat4.h"

void Esgi::Mat4::Identity()
{
	memset(m, 0, sizeof(float) * 16);
	m[0] = 1.0f; m[5] = 1.0f; m[10] = 1.0f; m[15] = 1.0f;
	// equivalent a MakeScale(1.0f, 1.0f, 1.0f);
}

void Esgi::Mat4::MakeScale(float sx, float sy, float sz)
{
	memset(m, 0, sizeof(float) * 16);
	m[0] = sx; m[5] = sy; m[10] = sz; m[15] = 1.0f;
}

void Esgi::Mat4::SetLocalScale(float sx, float sy, float sz)
{
	m[0] *= sx; m[5] *= sy; m[10] *= sz;
}

void Esgi::Mat4::MakeRotation2D(float angleDegree)
{
	float theta = angleDegree * (M_PI / 180.0f);
	Identity();
	m[0] = cos(theta);
	m[1] = sin(theta);
	m[4] = -sin(theta);
	m[5] = cos(theta);
}

void Esgi::Mat4::MakeTranslation(float tx, float ty, float tz)
{
	Identity();
	SetTranslate(tx, ty, tz);
}

void Esgi::Mat4::SetTranslate(float tx, float ty, float tz)
{
	m[12] = tx; m[13] = ty; m[14] = tz;
}

void Esgi::Mat4::Perspective(float fovy, float aspectRatio, float nearZ, float farZ)
{
	// Projection Matrix
	float fovy_rad = fovy * (M_PI / 180.f);
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

void Esgi::Mat4::MakeRotationY(float angleDegree)
{
	float theta = angleDegree * (M_PI / 180.0f);
	Identity();
	m[0] = cos(theta);
	m[2] = -sin(theta);
	m[8] = sin(theta);
	m[10] = cos(theta);
}

Esgi::Mat4 Esgi::Mat4::mult(Esgi::Mat4& B)
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
				M.m[i + j * 4] += m[i + k * 4] * B.m[k + j * 4];
			}
		}
	}

	return M;
}