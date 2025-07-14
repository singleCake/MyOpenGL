#pragma once
#include <cmath>
#include <string>

#include "../constant/Constant.h"

class CEuler;

class CVector {
public:
	float x, y, z;

	constexpr CVector() : x(0), y(0), z(0) {}
	constexpr CVector(float v[3]) : x(v[0]), y(v[1]), z(v[2]) {}
	constexpr CVector(float x, float y, float z) : x(x), y(y), z(z) {}
	~CVector() = default;

	void set(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	const CVector& normalize() {
		float l = this->len();
		this->x /= l;
		this->y /= l;
		this->z /= l;
		return *this;
	}

	std::string toString() const {
		return std::to_string(x) + ','
			+ std::to_string(y) + ','
			+ std::to_string(z);
	}
	float dotMul(const CVector& b) const {
		return this->x * b.x + this->y * b.y + this->z * b.z;
	}
	CVector crossMul(const CVector& b) const {
		return CVector(this->y * b.z - this->z * b.y, this->z * b.x - this->x * b.z, this->x * b.y - this->y * b.x);
	}
	float len() const { return sqrt(x * x + y * y + z * z); }
	CVector project(const CVector& b) const {
		return b * (this->dotMul(b) / b.len() / b.len());
	}

	CVector operator-() const { return CVector(-this->x, -this->y, -this->z); }
	CVector operator+(const CVector& b) const {
		return CVector(this->x + b.x, this->y + b.y, this->z + b.z);
	}
	CVector operator-(const CVector& b) const { return *this + -b; }
	CVector operator*(float b) const {
		return CVector(this->x * b, this->y * b, this->z * b);
	}
	friend CVector operator*(float k, const CVector& b);

	const CVector& operator*=(float k) {
		this->set(this->x * k, this->y * k, this->z * k);
		return *this;
	}
	const CVector& operator+=(const CVector& b) {
		this->set(this->x + b.x, this->y + b.y, this->z + b.z);
		return *this;
	}
	const CVector& operator=(const CVector& b) {
		this->set(b.x, b.y, b.z);
		return *this;
	}

	bool operator==(const CVector& b) const {
		return this->x == b.x && this->y == b.y && this->z == b.z;
	}
	bool operator!=(const CVector& b) const {
		return !(*this == b);
	}

	operator const float* () const { return &x; }

	CEuler toEuler() const;
};

inline CVector operator*(float b, const CVector& a) {
	return a * b;
}
