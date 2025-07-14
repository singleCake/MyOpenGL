#include "BoxWithBorder.h"
#include "../manager/RenderManager.h"
#include "../constant/Color.h"

void BoxWithBorder::addBorder() {
	borders.reserve(12);
	for (int i = 0; i < 4; ++i) {
		CVector left = vertices[i];
		CVector right = vertices[(i + 1) % 4];
		borders.emplace_back(left, right, BORDER_SIZE, BORDER_COLOR);
	}
	for (int i = 0; i < 4; ++i) {
		CVector up = vertices[i];
		CVector down = vertices[i + 4];
		borders.emplace_back(up, down, BORDER_SIZE, BORDER_COLOR);
	}
	for (int i = 0; i < 4; ++i) {
		CVector left = vertices[i + 4];
		CVector right = vertices[(i + 1) % 4 + 4];
		borders.emplace_back(left, right, BORDER_SIZE, BORDER_COLOR);
	}
}

BoxWithBorder::BoxWithBorder(const CVector& pos, float l, float w, float h, const CVector& color, bool withLine)
	: Box(pos, l, w, h, color, withLine) {
	addBorder();
}

BoxWithBorder::BoxWithBorder(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color, bool withLine)
	: Box(pos, axis, l, w, h, color, withLine) {
	addBorder();
}

BoxWithBorder::~BoxWithBorder() {}

void BoxWithBorder::trans(const CMatrix& mat) {
	Box::trans(mat);

	for (auto& border : borders) {
		border.trans(mat);
	}
}

void BoxWithBorder::draw(bool line) const {
	Box::draw();
	for (const auto& border : borders) {
		border.draw(line);
	}
}

void BoxWithBorder::drawWithTexture(const char* texture) const {
	Box::drawWithTexture(texture);
	for (const auto& border : borders) {
		border.drawWithTexture(texture);
	}
}