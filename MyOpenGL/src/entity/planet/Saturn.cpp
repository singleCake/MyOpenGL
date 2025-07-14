#include "Saturn.h"
#include "../../geometry/Ring.h"

Saturn::Saturn(const CVector& pos, const CVector& vel, float r, const CVector& color)
	: Saturn(pos, vel, r, DEFAULT_AUTO_ROTATION, color) {}

Saturn::Saturn(const CVector& pos, const CVector& vel, float r, float autoRotation, const CVector& color)
	: PlanetBase(r, autoRotation, color), AbstractEntity(pos, vel) {
	createPrimitives();
}

void Saturn::createPrimitives() {
	primitives.reserve(4);
	primitives.emplace_back(std::make_unique<Ring>(
		getPos(), 
		getRadius() * SATURN_RING_INNER_SCALE, getRadius() * SATURN_RING_OUTER_SCALE, 
		rotationAxis, CVector(0.5f, 0.5f, 0.5f)
	));
}

void Saturn::draw(bool line) {
	Planet::draw(line);
	primitives[3]->drawWithTexture("saturn_ring");
}