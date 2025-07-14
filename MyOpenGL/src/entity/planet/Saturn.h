#pragma once

#include "PlanetBase.h"

class Saturn : public PlanetBase<Saturn> {
public:
	Saturn(const CVector& pos, const CVector& vel, float r, const CVector& color = WHITE);
	Saturn(const CVector& pos, const CVector& vel, float r, float autoRotation, const CVector& color = WHITE);
	~Saturn() = default;

	void draw(bool line) override;

protected:
	void createPrimitives() override;
};