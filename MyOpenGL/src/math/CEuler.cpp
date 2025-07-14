#include "CEuler.h"
#include "CQuaternion.h"
#include <cmath>

CEuler::CEuler() : h(0), p(0), b(0) {}

CEuler::CEuler(float h, float p, float b) : h(h), p(p), b(b) {}

CEuler::CEuler(float euler[3]) : h(euler[0]), p(euler[1]), b(euler[2]) {}

CEuler::~CEuler() {}

void CEuler::set(float h, float p, float b) {
	this->h = h;
	this->p = p;
	this->b = b;
}

const CEuler& CEuler::normalize() {
	while (h > 180) h -= 360;
	while (h < -180) h += 360;
	while (p > 90) p -= 180;
	while (p < -90) p += 180;
	while (b > 180) b -= 360;
	while (b < -180) b += 360;
	return *this;
}

std::string CEuler::toString() const {
	return std::to_string(h) + ','
		+ std::to_string(p) + ','
		+ std::to_string(b);
}

CEuler::operator const float* () const {
	return &h;
}

CVector CEuler::toVector() const {
	CMatrix m = this->toMatrix();
	CVector v = m.vecMul(CVector(0, 0, -1));
	v.normalize();
	return v;
}

CMatrix CEuler::toMatrix() const {
	CMatrix m = CMatrix::rotateMatrix(h * PI / 180, CVector(0, 1, 0)) *
			CMatrix::rotateMatrix(p * PI / 180, CVector(1, 0, 0)) *
		CMatrix::rotateMatrix(b * PI / 180, CVector(0, 0, 1));
	return m;
}

CMatrix CEuler::toMatrix(const CVector& axis, const CVector& left) const {

	CMatrix m = CMatrix::rotateMatrix(h * PI / 180, axis) *
		CMatrix::rotateMatrix(p * PI / 180, left) *
		CMatrix::rotateMatrix(b * PI / 180, axis.crossMul(left).normalize());
	return m;
}

CMatrix CEuler::toMatrix(const CVector& axis, const CVector& left, const CVector& anchor) const {
	return CMatrix::transMatrix(anchor) *
		toMatrix(axis, left) *
		CMatrix::transMatrix(-anchor);
}

CQuaternion CEuler::toQuaternion() const {
	CQuaternion q;
	float cy = cos(h * 0.5f * PI / 180);
	float sy = sin(h * 0.5f * PI / 180);
	float cp = cos(p * 0.5f * PI / 180);
	float sp = sin(p * 0.5f * PI / 180);
	float cr = cos(b * 0.5f * PI / 180);
	float sr = sin(b * 0.5f * PI / 180);

	q.w = cy * cp * cr + sy * sp * sr;
	q.x = sy * cp * cr - cy * sp * sr;
	q.y = cy * sp * cr + sy * cp * sr;
	q.z = cy * cp * sr - sy * sp * cr;

	return q;
}

CEuler CVector::toEuler() const {
	CVector v = *this;
	v.normalize();
	float h = atan(v.x / v.z) * 180 / PI;
	float p = asin(v.y) * 180 / PI;
	float b = 0;
	return CEuler(h, p, b).normalize();
}