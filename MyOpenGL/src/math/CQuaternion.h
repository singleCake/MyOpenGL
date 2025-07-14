#pragma once

#include "CVector.h"
#include <memory>

class CMatrix;

class CQuaternion {
public:
	float w, x, y, z;
	CQuaternion();
	CQuaternion(float w, float x, float y, float z);
	CQuaternion(float angle, const CVector& axis);
	CQuaternion(float[4]);
	~CQuaternion();

	void set(float w, float x, float y, float z);
	void set(float angle, const CVector& axis);
	const CQuaternion& normalize();
	const CQuaternion& inverse();

	std::string toString() const;
	float dotMul(const CQuaternion& q) const;
	float len() const;
	CQuaternion getPowerd(float k) const;
	CQuaternion getInverse() const;
	std::pair<float, CVector> getAngleAxis() const;
	CQuaternion slerp(const CQuaternion& q, float t) const;
	std::unique_ptr<CQuaternion[]> slerp(const CQuaternion& q, int n, float* t) const;

	const CQuaternion& operator=(const CQuaternion& q);
	CQuaternion operator-() const;
	CQuaternion operator+(const CQuaternion& q) const;
	CQuaternion operator-(const CQuaternion& q) const;
	CQuaternion operator*(float k) const;
	CQuaternion operator*(const CQuaternion& q) const;
	friend CQuaternion operator*(float k, const CQuaternion& q);

	operator const float*() const;

	CMatrix toMatrix() const;
	CEuler toEuler() const;

private:
	float angle;
	CVector axis;
};