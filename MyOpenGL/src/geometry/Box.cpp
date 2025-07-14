#include "Box.h"
#include "../manager/RenderManager.h"
#include "../manager/TextureManager.h"

Box::Box(const CVector& pos, float l, float w, float h, const CVector& color, bool withLine)
	: Box(pos, CVector(0, 1, 0), l, w, h, color, withLine) {}

Box::Box(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color, bool withLine)
	: Cuboid(pos, axis, l, w, h, color), withLine(withLine) {
}

Box::~Box() {}

void Box::draw() const {
	Box::draw(false);
}

void Box::draw(bool line) const {
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	if (withLine)
		RenderManager::drawPrimitive(color, vertices, indices, normals, true, 0, 6);
	RenderManager::drawPrimitive(color, vertices, indices, normals, line, 6, 0);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

void Box::drawWithTexture(const char* texture) const {
	GLuint id = TextureManager::getTexture(texture);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	if (withLine) {
		RenderManager::drawPrimitiveWithTexture(id, vertices, indices, normals, texCoords, 0, 6, true);
	}
	RenderManager::drawPrimitiveWithTexture(id, vertices, indices, normals, texCoords, 6, 0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}