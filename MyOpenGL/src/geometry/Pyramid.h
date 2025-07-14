#pragma once

#include "Geometry.h"

struct Pyramid : public Geometry {
	Pyramid(const CVector& pos, 
		const CVector& u, const CVector& v, const CVector& w,
		float x, float y, float z, const CVector& color);
	~Pyramid() override;

	float x, y, z;
	CVector u, v, w;

private:
	void createVertices() override;
};