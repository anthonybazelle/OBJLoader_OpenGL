#ifndef OPENGL_1_MAT4_H
#define OPENGL_1_MAT4_H

#include <cstring>
#include <cmath>

#define M_PI 3.1416

namespace Esgi {
    struct Mat4 {

        Mat4();

        float m[16];

        Mat4(float diagonal);

        void Identity();

        void MakeScale(float sx, float sy, float sz);

        void SetLocalScale(float sx, float sy, float sz);

        void MakeRotationY(float angleDeg);

        void SetTranslate(float tx, float ty, float tz);

        void MakeTranslation(float tx, float ty, float tz);
        void Perspective(float fov, float aspect, float near, float far);

        void RotateY(float angle);
    };

    Mat4::Mat4() {
        Identity();
    }

    Mat4::Mat4(float diagonal) {
        memset(m, 0, sizeof(float) * 16);
        m[0] = diagonal;
        m[5] = diagonal;
        m[10] = diagonal;
        m[15] = 1.0f;
    }

    void Mat4::Identity() {
        memset(m, 0, sizeof(float) * 16);
        m[0] = 1.0f;
        m[5] = 1.0f;
        m[10] = 1.0f;
        m[15] = 1.0f;
    }

    void Mat4::MakeScale(float sx, float sy, float sz) {
        memset(m, 0, sizeof(float) * 16);
        m[0] = sx;
        m[5] = sy;
        m[10] = sz;
        m[15] = 1.0f;
    }

    void Mat4::SetLocalScale(float sx, float sy, float sz) {
        m[0] = sx;
        m[5] = sy;
        m[10] = sz;
        m[15] = 1.0f;
    }

    void Mat4::MakeRotationY(float angleDeg) {
        float rad = angleDeg * (float)(M_PI / 180.0f);
        Identity();
        m[0] = (float)cos(rad);
        m[1] = (float)sin(rad);
        m[4] = (float)-sin(rad);
        m[5] = (float)cos(rad);
    }

    void Mat4::MakeTranslation(float tx, float ty, float tz) {
        Identity();
        SetTranslate(tx, ty, tz);
    }

    void Mat4::SetTranslate(float tx, float ty, float tz) {
        m[12] = tx;
        m[13] = ty;
        m[14] = tz;
    }

    void Mat4::Perspective(float fov, float aspect, float nearZ, float farZ) {

        float fovyRad = fov * (M_PI / 180.0f);
        float focalLength = 1.0f / tan(fovyRad * 0.5f);
        float range = 1.0f / (nearZ - farZ);

        Identity();

        m[0] = focalLength / aspect;
        m[5] = focalLength;
        m[10] = (nearZ + farZ) * range;
        m[11] = -1.0f;
        m[14] = (2.0f * nearZ * farZ) * range;
        m[15] = 0.0f;
    }

	Mat4 operator *(Mat4 mat1, Mat4 mat2)
	{

		Mat4 matrix = Mat4();

		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				float total = 0;
				for (int k = 0; k<4; k++)
				{
					total += mat1.m[i * 4 + k] * mat2.m[k * 4 + j];
				}
				matrix.m[i * 4 + j] = total;
			}
		}

		return matrix;
	}

    void Mat4::RotateY(float angle)
    {
        float rad = angle * (M_PI / 180.0f);
        m[0] = cos(rad);
        m[2] = -sin(rad);
        m[8] = sin(rad);
        m[10] = cos(rad);
    }

}


#endif //OPENGL_1_MAT4_H