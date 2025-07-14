#include "HitBox.h"
#include "../../geometry/Cuboid.h"

#include "../../manager/RenderManager.h"

void HitBox::createPrimitives(float l, float w, float h) {
	primitives.clear();
	primitives.reserve(1);

	primitives.emplace_back(std::make_unique<Cuboid>(
		getPos(), axis, l, w, h
	));
}

HitBox::HitBox(const CVector& pos, const CVector& axis, float l, float w, float h) 
	: Entity(pos, CVector(0, 0, 0), axis) {
	createPrimitives(l, w, h);
}

HitBox::HitBox(const Cuboid& box) 
	: Entity(box.pos, CVector(0, 0, 0), box.axis) {
	primitives.clear();
	primitives.reserve(1);
	primitives.emplace_back(std::make_unique<Cuboid>(box));
	dynamic_cast<Cuboid*>(primitives.back().get())->u = box.u;
	dynamic_cast<Cuboid*>(primitives.back().get())->v = box.v;
}

void HitBox::set(const CVector& pos, const CVector& axis, float l, float w, float h) {
	setPos(pos);
	this->axis = axis;
	this->axis.normalize();
	createPrimitives(l, w, h);
}

void HitBox::draw(bool line) {
	Entity::draw(line);

	RenderManager::drawArrow(0.01, RED, getPos(), 
		getPos() + dynamic_cast<Cuboid*>(primitives[0].get())->u * 0.2f);
}

bool HitBox::checkCollision(const HitBox& other) const {
	return getBox()->checkCollision(*other.getBox());
}