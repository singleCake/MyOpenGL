#pragma once

#include "CVector.h"
#include <string>

class CQuaternion;

class CMatrix {
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;

	CMatrix();
	CMatrix(float[16]);
	CMatrix(const float[16]);
	
	void set(float* val);
	const CMatrix& orthogonalize();
	std::string toString() const;

	const CMatrix& operator=(const CMatrix& p);
	const CMatrix& operator*=(const CMatrix& p);
	CMatrix operator+(const CMatrix& m) const;
	CMatrix operator*(float d) const;
	friend CMatrix operator*(float d, const CMatrix& m);
	CMatrix operator*(const CMatrix& p) const;

	CVector vecMul(const CVector& v) const;
	CVector posMul(const CVector& v) const;
	float det() const;
	CMatrix getInverse() const;

	static CMatrix rotateMatrix(float angle, const CVector& axis);
	static CMatrix rotateMatrix(float angle, const CVector& axis, const CVector& anchor);
	static CMatrix rotateMatrix(const CVector& v1, const CVector& v2);
	static CMatrix rotateMatrix(const CVector& v1, const CVector& v2, const CVector& anchor);
	static CMatrix transMatrix(const CVector& trans);
	static CMatrix scaleMatrix(const CVector& scale);
	static CMatrix perspectiveMatrix(float fov, float aspect, float near, float far);
	static CMatrix lookAtMatrix(const CVector& eye, const CVector& center, const CVector& up);
	static CMatrix lookAtMatrix(const CVector& front, const CVector& up, const CVector& right, const CVector& pos);

	operator const float* () const;

	CEuler toEuler() const;
	CQuaternion toQuaternion() const;
};