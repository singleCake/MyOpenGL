#include "Cylinder.h"

void Cylinder::createVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    vertices.reserve(slices * 2 + 2);
    normals.reserve(slices * 2 + 2);
    texCoords.reserve(slices * 2 + 2);
    indices.reserve(slices * 6);

    CVector Axis = h / 2 * axis;

    // 添加中心点
    vertices.emplace_back(pos + Axis);
    normals.emplace_back(axis);
    texCoords.emplace_back(0.5f, 0.5f);

    vertices.emplace_back(pos - Axis);
    normals.emplace_back(-axis);
    texCoords.emplace_back(0.5f, 0.5f);

    CVector u, v;
    CVector ref(1, 0, 0);
    if (fabs(axis.dotMul(ref)) > 0.9f)
        ref = CVector(0, 1, 0);

    u = ref.crossMul(axis).normalize();
    v = axis.crossMul(u).normalize();

    const float angleStep = 2 * PI / slices;

    for (int i = 0; i <= slices; ++i) {
        float theta = i * angleStep;
        float cos_t = cos(theta);
        float sin_t = sin(theta);

        CVector dir = u * cos_t + v * sin_t;

        // 顶面圆周上的点
        vertices.emplace_back(pos + dir * r + Axis);
        normals.emplace_back(dir);
        // 纹理坐标：U沿圆周方向，V固定为1（顶面）
        texCoords.emplace_back(static_cast<float>(i) / slices, 1.0f);

        // 底面圆周上的点
        vertices.emplace_back(pos + dir * r - Axis);
        normals.emplace_back(dir);
        // 纹理坐标：U沿圆周方向，V固定为0（底面）
        texCoords.emplace_back(static_cast<float>(i) / slices, 0.0f);
    }

    for (int i = 0; i < slices; ++i) {
        indices.push_back(0);
        indices.push_back(i * 2 + 2);
        indices.push_back((i + 1) * 2 + 2);

        indices.push_back(1);
        indices.push_back(i * 2 + 3);
        indices.push_back((i + 1) * 2 + 3);
    }

    for (int i = 0; i < slices; ++i) {
        indices.push_back(i * 2 + 2);
        indices.push_back(i * 2 + 3);
        indices.push_back((i + 1) * 2 + 3);

        indices.push_back(i * 2 + 2);
        indices.push_back((i + 1) * 2 + 3);
        indices.push_back((i + 1) * 2 + 2);
    }
}

Cylinder::Cylinder(const CVector& up, const CVector& down, float r, const CVector& color)
	: Geometry((up + down) * 0.5f, color), axis(down - up), r(r), h((up - down).len()) {
	this->slices = r * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(r * DEFAULT_SLICE) : MIN_SLICE;
	axis.normalize();
	createVertices();
}

Cylinder::Cylinder(const CVector& pos, const CVector& axis, float r, float h, const CVector& color)
	: Geometry(pos, color), axis(axis), r(r), h(h) {
	this->axis.normalize();
	this->slices = r * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(r * DEFAULT_SLICE) : MIN_SLICE;
	createVertices();
}

Cylinder::~Cylinder() {}