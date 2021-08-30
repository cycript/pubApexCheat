#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <windows.h>

struct Time
{
	int Hour;
	int Minute;
	int Second;
};

struct Vector2
{
    float X;
    float Y;
    inline Vector2 operator+(Vector2 pVec)
    {
        return { X + pVec.X, Y + pVec.Y };
    }
    inline Vector2 operator-(Vector2 pVec)
    {
        return { X - pVec.X, Y - pVec.Y };
    }
    inline Vector2 operator*(Vector2 pVec)
    {
        return { X * pVec.X, Y * pVec.Y };
    }
    inline Vector2 operator/(Vector2 pVec)
    {
        return { X / pVec.X, Y / pVec.Y };
    }
};

struct Vector3
{
	float X;
	float Y;
	float Z;
	inline Vector3 operator+(Vector3 pVec)
	{
		return { X + pVec.X, Y + pVec.Y, Z + pVec.Z };
	}
	inline Vector3 operator-(Vector3 pVec)
	{
		return { X - pVec.X, Y - pVec.Y, Z - pVec.Z };
	}
	inline Vector3 operator*(Vector3 pVec)
	{
		return { X * pVec.X, Y * pVec.Y, Z * pVec.Z };
	}
	inline void operator*=(float flNum)
	{
		X = X * flNum;
		Y = Y * flNum;
		Z = Z * flNum;
	}
	inline Vector3 operator/(float flNum)
	{
		return { X / flNum, Y / flNum, Z / flNum };
	}
	inline Vector3 operator*(float flNumber)
	{
		return { X * flNumber, Y * flNumber, Z * flNumber };
	}
	inline float Dot(Vector3 pVec)
	{
		return (X * pVec.X) + (Y * pVec.Y) + (Z * pVec.Z);
	}
	inline float Distance(const Vector3 pVec)
	{
		return sqrtf((X - pVec.X) * (X - pVec.X) + (Y - pVec.Y) * (Y - pVec.Y) + (Z - pVec.Z) * (Z - pVec.Z));
	}
	inline float Length()
	{
		return sqrtf(Dot({ X, Y, Z }));
	}
};

class Vector4
{
public:
	float X;
	float Y;
	float Z;
	float W;
	inline Vector4 operator+(Vector4 pVec)
	{
		float vecData[4];
		vecData[0] = pVec.X + X;
		vecData[1] = pVec.Y + Y;
		vecData[2] = pVec.Z + Z;
		vecData[3] = pVec.W + W;
		return *(Vector4*)&vecData;
	}
	inline Vector4 operator-(Vector4 pVec)
	{
		float vecData[4];
		vecData[0] = pVec.X - X;
		vecData[1] = pVec.Y - Y;
		vecData[2] = pVec.Z - Z;
		vecData[3] = pVec.W - W;
		return *(Vector4*)&vecData;
	}
	inline Vector4 operator/(float flNum)
	{
		float vecData[4];
		vecData[0] = X / flNum;
		vecData[1] = Y / flNum;
		vecData[2] = Z / flNum;
		vecData[3] = W / flNum;
		return *(Vector4*)&vecData;
	}
	inline Vector4 operator*(float flNum)
	{
		float vecData[4];
		vecData[0] = X * flNum;
		vecData[1] = Y * flNum;
		vecData[2] = Z * flNum;
		vecData[3] = W * flNum;
		return *(Vector4*)&vecData;
	}
	inline float Dot(Vector4 pVec)
	{
		return (X * pVec.X) + (Y * pVec.Y) + (Z * pVec.Z) + (W * pVec.W);
	}
	inline float Length()
	{
		return sqrtf(Dot({ X, Y, Z }));
	}
};

struct matrix3x4_t
{
	matrix3x4_t() = default;
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_matrix[0][0] = m00;
		m_matrix[0][1] = m01;
		m_matrix[0][2] = m02;
		m_matrix[0][3] = m03;
		m_matrix[1][0] = m10;
		m_matrix[1][1] = m11;
		m_matrix[1][2] = m12;
		m_matrix[1][3] = m13;
		m_matrix[2][0] = m20;
		m_matrix[2][1] = m21;
		m_matrix[2][2] = m22;
		m_matrix[2][3] = m23;
	}
	matrix3x4_t(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis, const Vector3& vec_origin)
	{
		init(x_axis, y_axis, z_axis, vec_origin);
	}

	void init(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis, const Vector3& vec_origin)
	{
		m_matrix[0][0] = x_axis.X;
		m_matrix[0][1] = y_axis.X;
		m_matrix[0][2] = z_axis.X;
		m_matrix[0][3] = vec_origin.X;
		m_matrix[1][0] = x_axis.Y;
		m_matrix[1][1] = y_axis.Y;
		m_matrix[1][2] = z_axis.Y;
		m_matrix[1][3] = vec_origin.Y;
		m_matrix[2][0] = x_axis.Z;
		m_matrix[2][1] = y_axis.Z;
		m_matrix[2][2] = z_axis.Z;
		m_matrix[2][3] = vec_origin.Z;
	}

	float* operator[](int i)
	{
		return m_matrix[i];
	}

	const float* operator[](int i) const
	{
		return m_matrix[i];
	}

	float m_matrix[3][4];
};

struct RGB
{
    float R, G, B;
};

#endif // !STRUCTS_H

