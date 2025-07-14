#pragma once

#include "Cuboid.h"

struct Box : public Cuboid {
	Box(const CVector& pos, float l, float w, float h, const CVector& color, bool withLine = true);
	Box(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color, bool withLine = true);
	virtual ~Box();

	bool withLine;

	virtual void draw() const;
	virtual void drawWithTexture(const char* texture) const override;

private:
	void draw(bool line) const override;
};