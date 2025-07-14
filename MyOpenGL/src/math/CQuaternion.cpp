#include "CQuaternion.h"
#include "CEuler.h"
#include "CMatrix.h"
#include <cmath>

CQuaternion::CQuaternion() : CQuaternion(1, 0, 0, 0) {}

CQuaternion::CQuaternion(float w, float x, float y, float z) {
	set(w, x, y, z);
}

CQuaternion::CQuaternion(float w, const CVector& axis) {
	set(w, axis);
}

CQuaternion::CQuaternion(float arr[4]) : CQuaternion(arr[0], arr[1], arr[2], arr[3]) {}

CQuaternion::~CQuaternion() {}

void CQuaternion::set(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;

	this->angle = acos(w) * 2;
	if (angle > PI)
		angle -= 2 * PI;
	float sinHalfAngle = sqrt(1 - w * w);
	if (fabs(sinHalfAngle) > 1.0E-5) {
		this->axis.x = x / sinHalfAngle;
		this->axis.y = y / sinHalfAngle;
		this->axis.z = z / sinHalfAngle;
	}
	else {
		this->axis.x = 0;
		this->axis.y = 0;
		this->axis.z = 0;
	}
}

void CQuaternion::set(float angle, const CVector& axis) {
	this->angle = angle;
	this->axis = axis;
	this->axis.normalize();
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sin(halfAngle);
	w = cos(halfAngle);
	x = this->axis.x * sinHalfAngle;
	y = this->axis.y * sinHalfAngle;
	z = this->axis.z * sinHalfAngle;
}

const CQuaternion& CQuaternion::normalize() {
	float l = len();
	if (l > 0) {
		set(w / l, x / l, y / l, z / l);
		return *this;
	}
	else {
		return NULL;
	}
}

const CQuaternion& CQuaternion::inverse() {
	float l = len();
	l *= l;
	set(w / l, -x / l, -y / l, -z / l);
	return *this;
}

std::string CQuaternion::toString() const {
	return std::to_string(w) + ','
		+ std::to_string(x) + ','
		+ std::to_string(y) + ','
		+ std::to_string(z);
}

float CQuaternion::dotMul(const CQuaternion& q) const {
	return w * q.w + x * q.x + y * q.y + z * q.z;
}

float CQuaternion::len() const {
	return sqrt(w * w + x * x + y * y + z * z);
}

CQuaternion CQuaternion::getInverse() const {
	float l = len();
	l *= l;
	return { w / l, -x / l, -y / l, -z / l };
}

std::pair<float, CVector> CQuaternion::getAngleAxis() const {
	return std::make_pair(angle, axis);
}

CQuaternion CQuaternion::slerp(const CQuaternion& q, float t) const {
	CQuaternion p = *this, q0 = q;
	p.normalize();
	q0.normalize();
	float cosTheta = p.dotMul(q0);
	if (cosTheta < 0) {
		q0 = -q0;
		cosTheta = -cosTheta;
	}

	float theta = acos(cosTheta);
	float sinTheta = sin(theta);

	if (fabs(sinTheta) < 1.0E-5) {
		return ((1.0f - t) * p + t * q0).normalize();
	}

	float a = sin((1.0f - t) * theta) / sinTheta;
	float b = sin(t * theta) / sinTheta;

	return (a * p + b * q0).normalize();
}

std::unique_ptr<CQuaternion[]> CQuaternion::slerp(const CQuaternion& q, int n, float* t) const {
	std::unique_ptr<CQuaternion[]> quaternions(new CQuaternion[n]);
	for (int i = 0; i < n; ++i) {
		quaternions[i] = slerp(q, t[i]);
	}
	return quaternions;
}

const CQuaternion& CQuaternion::operator=(const CQuaternion& q) {
	if (this != &q) {
		set(q.w, q.x, q.y, q.z);
	}
	return *this;
}

CQuaternion CQuaternion::operator-() const {
	return CQuaternion(-w, -x, -y, -z);
}

CQuaternion CQuaternion::operator+(const CQuaternion& q) const {
	return CQuaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

CQuaternion CQuaternion::operator-(const CQuaternion& q) const {
	return q * this->getInverse();
}

CQuaternion CQuaternion::operator*(float s) const {
	return CQuaternion(w * s, x * s, y * s, z * s);
}

CQuaternion CQuaternion::operator*(const CQuaternion& q) const {
	return CQuaternion(
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w
	);
}

CQuaternion operator*(float k, const CQuaternion& q) {
	return q * k;
}

CQuaternion::operator const float* () const {
	return &w;
}

CMatrix CQuaternion::toMatrix() const {
	float m[16] = {
		1 - 2 * y * y - 2 * z * z, 2 * x * y + 2 * w * z, 2 * x * z - 2 * w * y, 0,
		2 * x * y - 2 * w * z, 1 - 2 * x * x - 2 * z * z, 2 * y * z + 2 * w * x, 0,
		2 * x * z + 2 * w * y, 2 * y * z - 2 * w * x, 1 - 2 * x * x - 2 * y * y, 0,
		0, 0, 0, 1
	};
	return CMatrix(m);
}

CEuler CQuaternion::toEuler() const {
	float p = asin(2 * (x * w - z * y));
	if (fabs(cos(p)) > 1.0E-5) {
		float h = atan2(2 * (x * z + w * y), 1 - 2 * (x * x + y * y));
		float b = atan2(2 * (x * y + w * z), 1 - 2 * (x * x + z * z));
		return CEuler(h * 180 / PI, p * 180 / PI, b * 180 / PI).normalize();
	}
	else {
		float h = atan2(2 * (y * w - z * x), 1 - 2 * (z * z + y * y));
		float b = 0;
		return CEuler(h * 180 / PI, p * 180 / PI, b).normalize();
	}
}