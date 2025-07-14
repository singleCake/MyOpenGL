#pragma once
#include <GLFW/glfw3.h>
#include <vector>

#include "../math/CEuler.h"
#include "../math/CVector2.h"
#include "../constant/GeometryConstant.h"
#include "../constant/Color.h"

struct Geometry {
	Geometry();
	Geometry(const CVector& pos, const CVector& color);
	virtual ~Geometry();

	virtual void draw(bool lineMode = false) const;
	virtual void drawWithTexture(const char* textureID) const;

	virtual void trans(const CMatrix& mat);

	CVector pos;
	CVector color;
protected:
	virtual void createVertices() = 0;
	std::vector<CVector> vertices;
	std::vector<int> indices;
	std::vector<CVector> normals;
	std::vector<CVector2> texCoords;
};