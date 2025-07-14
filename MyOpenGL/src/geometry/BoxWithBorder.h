#pragma once

#include "Box.h"
#include "Cylinder.h"

struct BoxWithBorder : public Box {
public:
	BoxWithBorder(const CVector& pos, float l, float w, float h, const CVector& color, bool withLine = true);
	BoxWithBorder(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color, bool withLine = true);
	~BoxWithBorder() override;

	void draw(bool line) const override;
	void drawWithTexture(const char* texture) const override;

	void trans(const CMatrix& mat) override;

private:
	void addBorder();

	std::vector<Cylinder> borders;
};