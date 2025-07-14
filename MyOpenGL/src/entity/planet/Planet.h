#pragma once

#include "../Entity.h"
#include "../abstract/ViewPoint.h"
#include "../../geometry/Ball.h"

static CVector DEFAULT_ANCHOR(0, 0, 0);

class Planet : public Entity, public ViewPoint {
public:
	Planet(float r, const CVector& color = WHITE, bool src = false);
	Planet(float r, float autoRotation, const CVector& color, bool src = false);
	Planet(CVector& anchor, float r, const CVector& color, bool src = false);
	Planet(CVector& anchor, float r, float autoRotation, const CVector& color, bool src = false);
	virtual ~Planet() = default;

	void init();
	virtual void update(float dt) override;
	virtual void draw(bool line) override;
	
	void hover() { hovered = true; }
	void cancelHover() { hovered = false; }
	void select() { selected = true; }
	void cancelSelect() { selected = false; }

	const Ball& getBall() const { return *dynamic_cast<Ball*>(primitives[0].get()); }
	float getRadius() const { return radius; }

	bool raySphereIntersect(const CVector& rayOrigin, const CVector& rayDir, float& t) const;

	virtual const char* getID() const = 0;
	virtual const char* getName() const = 0;

protected:
	CVector& anchor;
	CVector rotationAxis;

	enum BALL_TYPE { DEFAULT, HOVER, SELECT };
	virtual void createPrimitives() override;

private:
	CVector color;
	CVector orbitalColor;

	float radius;
	float orbitalRadius;

	float omega;
	float autoRotation;
	CVector lastAnchor;

	bool hovered = false;
	bool selected = false;

	bool src;
};