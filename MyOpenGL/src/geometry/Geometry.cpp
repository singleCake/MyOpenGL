#include "Geometry.h"
#include "../manager/RenderManager.h"
#include "../constant/Color.h"
#include "../manager/TextureManager.h"

Geometry::Geometry()
	: Geometry(CVector(0, 0, 0), WHITE) {}

Geometry::Geometry(const CVector& pos, const CVector& color) 
	: pos(pos), color(color) {
}

Geometry::~Geometry() {}

void Geometry::trans(const CMatrix& mat) {
	pos = mat.posMul(pos);
	for (auto& vertex : vertices) {
		vertex = mat.posMul(vertex);
	}
	for (auto& normal : normals) {
		normal = mat.vecMul(normal);
	}
}

void Geometry::draw(bool line) const {
	RenderManager::drawPrimitive(color, vertices, indices, normals, line);
}

void Geometry::drawWithTexture(const char* textureID) const {
	RenderManager::drawPrimitiveWithTexture(TextureManager::getTexture(textureID), vertices, indices, normals, texCoords);
}