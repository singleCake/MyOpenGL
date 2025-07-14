#pragma once

#include "planet/Planet.h"
#include "abstract/ViewPoint.h"
#include "astronaut/Astronaut.h"

using PlanetUPtr = std::unique_ptr<Planet>;

class SpaceCraft : public Entity, public ViewPoint {
public:
	SpaceCraft(const CVector& pos, const CVector& vel, float size);
	~SpaceCraft() override = default;

	void pitch(float angle);
	void yaw(float angle);
	void rotateMat(const CMatrix& mat) override;

	void addChildren(AbstractEntity* child, float x = 0, float y = 0);
	void setTarget(PlanetUPtr* target) { this->target = target; }

	void update(float dt) override;
	void handleKeyPress(int keyCode);
	void handleKeyClick(int keyCode);
	void handleMouseOffset(double dx, double dy);
	void draw(bool line) override;

	const HitBox& getHitBox() const { return hitbox; }

private:
	float size;
	CVector pointLight, spotLight;
	PlanetUPtr* target = nullptr;
	std::vector<AbstractEntity*> children;
	HitBox hitbox;

	void createPrimitives() override;

	void checkCollision() const;
	bool checkCollision(const Planet& planet) const;
};