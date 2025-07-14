#pragma once
#include "Geometry.h"

struct Cylinder : public Geometry {
public:
	Cylinder(const CVector& up, const CVector& down, float r, const CVector& color);
	Cylinder(const CVector& pos, const CVector& axis, float r, float h, const CVector& color);
	virtual ~Cylinder() override;

	int slices;
	float r, h;
	CVector axis;

private:
	void createVertices() override;
};