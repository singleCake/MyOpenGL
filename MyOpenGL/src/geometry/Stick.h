#pragma once

#include "Cylinder.h"

struct Stick : public Cylinder {
	// axis Îª×ªÖá
	Stick(const CVector& pivot, const CVector& end, const CVector& axis, float r, const CVector& color);
	~Stick() override = default;

	void rotate(float angle);
	void trans(const CMatrix& mat) override;

private:
	CVector rotationAxis;
	CVector pivot;
};