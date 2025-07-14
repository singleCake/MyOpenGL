#pragma once
#include <string>

class CVector2 {
public:
	float x, y;

	constexpr CVector2() : x(0), y(0) {}
	constexpr CVector2(float v[2]) : x(v[0]), y(v[1]) {}
	constexpr CVector2(float x, float y) : x(x), y(y) {}
	~CVector2() = default;

	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	const CVector2& normalize() {
		float l = this->len();
		this->x /= l;
		this->y /= l;
		return *this;
	}

	std::string toString() const {
		return std::to_string(x) + ','
			+ std::to_string(y);
	}
	float dotMul(const CVector2& b) const {
		return this->x * b.x + this->y * b.y;
	}
	float len() const { return sqrt(x * x + y * y); }

	CVector2 operator-() const { return CVector2(-this->x, -this->y); }
	CVector2 operator+(const CVector2& b) const {
		return CVector2(this->x + b.x, this->y + b.y);
	}
	CVector2 operator-(const CVector2& b) const { return *this + -b; }
	CVector2 operator*(float b) const {
		return CVector2(this->x * b, this->y * b);
	}
	friend CVector2 operator*(float k, const CVector2& b);

	const CVector2& operator*=(float k) {
		this->set(this->x * k, this->y * k);
		return *this;
	}
	const CVector2& operator+=(const CVector2& b) {
		this->set(this->x + b.x, this->y + b.y);
		return *this;
	}
	const CVector2& operator=(const CVector2& b) {
		this->set(b.x, b.y);
		return *this;
	}

	bool operator==(const CVector2& b) const {
		return this->x == b.x && this->y == b.y;
	}
	bool operator!=(const CVector2& b) const {
		return !(*this == b);
	}

	operator const float* () const { return &x; }
};

inline CVector2 operator*(float b, const CVector2& a) {
	return a * b;
}