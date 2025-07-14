#pragma once

#include "CMatrix.h"

class CEuler {
public:
	float h, p, b;
	CEuler();
	CEuler(float h, float p, float b);
	CEuler(float[3]);
	~CEuler();
	void set(float h, float p, float b);
	const CEuler& normalize();

	std::string toString() const;
	CEuler operator+(const CEuler& e) const { return CEuler(h + e.h, p + e.p, b + e.b); }
	CEuler operator-(const CEuler& e) const { return CEuler(h - e.h, p - e.p, b - e.b); }
	operator const float* () const;

	CVector toVector() const;
	CMatrix toMatrix() const;
	CMatrix toMatrix(const CVector& axis, const CVector& left) const;
	CMatrix toMatrix(const CVector& axis, const CVector& left, const CVector& anchor) const;
	CQuaternion toQuaternion() const;
};