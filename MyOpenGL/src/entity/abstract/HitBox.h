#pragma once

#include "../Entity.h"
#include "../../geometry/Cuboid.h"

class HitBox : public Entity {
public:
	HitBox() = default;
	HitBox(const Cuboid& box);
	HitBox(const CVector& pos, const CVector& axis, float l, float w, float h);
	~HitBox() override = default;

	void set(const CVector& pos, const CVector& axis, float l, float w, float h);

	bool checkCollision(const HitBox& other) const;

	Cuboid* getBox() const { return dynamic_cast<Cuboid*>(primitives[0].get()); }

	void draw(bool line) override;
private:
	void createPrimitives() {}
	void createPrimitives(float l, float w, float h);
};