#include "Vec3.h"

Esgi::Vec3::Vec3():m_x(0), m_y(0), m_z(0)
{
	calculateNorme();
}

Esgi::Vec3::Vec3(float x, float y, float z):m_x(x), m_y(y), m_z(z)
{
	calculateNorme();
}

void Esgi::Vec3::setX(float x)
{
	m_x = x;
	calculateNorme();
}

void Esgi::Vec3::setY(float y)
{
	m_y = y;
	calculateNorme();
}

void Esgi::Vec3::setZ(float z)
{
	m_z = z;
	calculateNorme();
}

const float Esgi::Vec3::getX()
{
	return m_x;
}

const float Esgi::Vec3::getY()
{
	return m_y;
}

const float Esgi::Vec3::getZ()
{
	return m_z;
}

Esgi::Vec3 Esgi::Vec3::crossProduct( Esgi::Vec3 & v1,  Esgi::Vec3 & v2)
{
	return Esgi::Vec3((v1.getY()*v2.getZ())-(v1.getZ()*v2.getY()),
		(v1.getZ()*v2.getX()) - (v1.getX()*v2.getZ()),
		(v1.getX()*v2.getY()) - (v1.getY()*v2.getX()));
}

float Esgi::Vec3::dotProduct(Esgi::Vec3 & v1, Esgi::Vec3 & v2)
{
	return ((v1.getX()*v2.getX()) + (v1.getY()*v2.getY()) + (v1.getZ()*v2.getZ()));
}

Esgi::Vec3 Esgi::Vec3::normalize(Vec3 & v1)
{
	if (v1.m_norme == 0)
		return Esgi::Vec3(0,0,0);
	return Esgi::Vec3(v1.m_x/v1.m_norme, v1.m_y / v1.m_norme, v1.m_z / v1.m_norme);
}

Esgi::Vec3 Esgi::Vec3::addition(Esgi::Vec3 & a, Esgi::Vec3 & b)
{
	return Esgi::Vec3(a.m_x+b.m_x,a.m_y+b.m_y, a.m_z+b.m_z);
}

Esgi::Vec3 Esgi::Vec3::soustraction(Vec3 & a, Vec3 & b)
{
	return Esgi::Vec3(a.m_x - b.m_x, a.m_y - b.m_y, a.m_z - b.m_z);
}

Esgi::Vec3 Esgi::Vec3::multiplication(Vec3 & a, float b)
{
	return Esgi::Vec3(a.m_x *b, a.m_y *b, a.m_z *b);
}

float Esgi::Vec3::getNorme()
{
	return m_norme;
}

void Esgi::Vec3::calculateNorme()
{
	m_norme = sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}
