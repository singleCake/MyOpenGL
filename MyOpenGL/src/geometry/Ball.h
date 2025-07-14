#pragma once

#include "Geometry.h"

struct Ball : public Geometry {
public:
	Ball(const CVector& pos, float r, const CVector& color = WHITE, bool src = false);
	Ball(const CVector& pos, float r, const CVector& axis, const CVector& color = WHITE, bool src = false);
	~Ball() = default;

	bool raySphereIntersect(const CVector& rayOrigin, const CVector& rayDir, float& out_t) const;

	float r; // °ë¾¶
	CVector axis; // ÖÐÐÄÖá

protected:
	void createVertices() override;

private:
	int slice;
	bool src;
};