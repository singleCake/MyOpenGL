#pragma once

#include "Geometry.h"

struct Ring : public Geometry {
public:
	Ring(const CVector& pos, float innerRadius, float outerRadius, const CVector& axis, const CVector& color);
	~Ring();

	float innerRadius, outerRadius;
	CVector axis;

	void drawWithTexture(const char* textureName) const override;

protected:
	void createVertices() override;

private:
	int slice;
};