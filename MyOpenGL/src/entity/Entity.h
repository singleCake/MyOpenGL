#pragma once

#include "abstract/AbstractEntity.h"

using PrimitiveUPtr = std::unique_ptr<Geometry>;

class Entity : virtual public AbstractEntity {
public:
	Entity() = default;
	virtual ~Entity() override = default;

	virtual void rotateMat(const CMatrix& mat) override;

	virtual void update(float dt) override;
	virtual void draw(bool line) override;

	virtual void setPos(const CVector& pos) override;

protected:
	virtual void createPrimitives() = 0;

	std::vector<PrimitiveUPtr> primitives;
};