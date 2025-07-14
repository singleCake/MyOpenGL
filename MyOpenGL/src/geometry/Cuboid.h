#pragma once

#include "Ball.h"
#include "../constant/Color.h"

#include <array>

struct Cuboid : public Geometry {
public:
	Cuboid(const CVector& pos, float l, float w, float h, const CVector& color = WHITE);
	Cuboid(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color = WHITE);
	virtual ~Cuboid() override = default;

	float l, w, h;
	CVector axis, u, v;
	std::array<CVector*, 3> axes = {&axis, &u, &v};

	bool checkCollision(const Cuboid& other) const;
	bool checkCollision(const Ball& other) const;
	bool checkCollision(const Cuboid& other, CVector& collisionPoint) const;

	void trans(const CMatrix& m) override;

	void drawWithTwoTextures(const char* texture1, const char* texture2) const;

private:
	void createVertices() override;

	friend inline static void projectBox(const Cuboid& box, const CVector& axis, float& minVal, float& maxVal);
};