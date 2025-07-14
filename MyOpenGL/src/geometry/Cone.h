#pragma once

#include "Geometry.h"

struct Cone : public Geometry {
	// pos 为底面圆心坐标
	Cone(const CVector& pos, const CVector& axis, float r, float h, const CVector& color);
	~Cone() override;

	int slice;
	float r, h;
	CVector axis;

private:
	void createVertices() override;
};