#pragma once

#include "Anon.h"
#include "../abstract/ViewPoint.h"

class Player : public Anon, public ViewPoint {
public:
	Player();
	~Player() override = default;

	void rotateMat(const CMatrix& mat) override;

	void update(float dt) override;
	void handleMouseOffset(double dx, double dy);

	void setPos(const CVector& pos) override;

	bool checkCollision() const override;
};