#include "Stick.h"

Stick::Stick(const CVector& pivot, const CVector& end, const CVector& axis, float r, const CVector& color)
	: Cylinder(pivot, end, r, color), rotationAxis(axis) {
	this->pivot = pivot;
	rotationAxis.normalize();
}

void Stick::rotate(float angle) {
	CMatrix mat = CMatrix::rotateMatrix(angle, rotationAxis, pivot);

	for (auto& vertex : vertices) {
		vertex = mat.posMul(vertex);
	}
}

void Stick::trans(const CMatrix& mat) {
	Geometry::trans(mat);

	pivot = mat.posMul(pivot);
	rotationAxis = mat.vecMul(rotationAxis);
}