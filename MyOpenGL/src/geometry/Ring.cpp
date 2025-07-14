#include "Ring.h"

#include <iostream>

Ring::Ring(const CVector& pos, float innerRadius, float outerRadius, const CVector& axis, const CVector& color)
	: Geometry(pos, color), innerRadius(innerRadius), outerRadius(outerRadius), axis(axis) {
	slice = outerRadius * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(outerRadius * DEFAULT_SLICE) : MIN_SLICE;
	createVertices();
}

Ring::~Ring() {}

void Ring::createVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    vertices.reserve(slice * 2 + 2);
    indices.reserve(slice * 6);
    normals.reserve(slice * 2 + 2);
	texCoords.reserve(slice * 2 + 2);

    CVector normalizedAxis = axis;
    normalizedAxis.normalize();
    CVector u, v;

    CVector ref(1, 0, 0);
    if (fabs(normalizedAxis.dotMul(ref)) > 0.9f)
        ref = CVector(0, 1, 0);

    u = ref.crossMul(normalizedAxis).normalize();
    v = normalizedAxis.crossMul(u).normalize();

    const float angleStep = 2 * PI / slice;

    CVector ringNormal = normalizedAxis;

    for (int i = 0; i <= slice; ++i) {
        float theta = i * angleStep;
        float cos_t = cos(theta);
        float sin_t = sin(theta);

        CVector dir = u * cos_t + v * sin_t;

        // 内环顶点
        vertices.emplace_back(pos + dir * innerRadius);
        normals.emplace_back(ringNormal);  // 内环法向量
        texCoords.emplace_back(static_cast<float>(i) / slice, 0.0f); // 纹理坐标 (u, 0)

        // 外环顶点
        vertices.emplace_back(pos + dir * outerRadius);
        normals.emplace_back(ringNormal);  // 外环法向量
        texCoords.emplace_back(static_cast<float>(i) / slice, 1.0f); // 纹理坐标 (u, 1)
    }

    // 生成索引
    for (int i = 0; i < slice; ++i) {
        indices.push_back(i * 2);
        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2);

        indices.push_back(i * 2 + 1);
        indices.push_back((i + 1) * 2 + 1);
        indices.push_back((i + 1) * 2);
    }
}

void Ring::drawWithTexture(const char* ID) const {
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    Geometry::drawWithTexture(ID);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}