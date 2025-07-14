#include "Cone.h"

void Cone::createVertices() {
    vertices.clear();
    normals.clear();
    indices.clear();
    texCoords.clear();

    vertices.reserve(slice * 2 + 2);
    normals.reserve(slice * 2 + 2);
    texCoords.reserve(slice * 2 + 2);
    indices.reserve(slice * 3 + slice * 3);

    CVector p1 = pos + axis * h;

    // �ײ����ĵ�
    vertices.emplace_back(pos);
    normals.emplace_back(-axis);
    texCoords.emplace_back(0.5f, 0.5f);

    // �ײ���Ե��
    for (int i = 0; i <= slice; ++i) {
        float theta = i * 2 * PI / slice;
        float cos_t = cos(theta);
        float sin_t = sin(theta);

        CVector u = axis.crossMul(CVector(1, 0, 0)).normalize();
        CVector v = axis.crossMul(u).normalize();
        CVector dir = u * cos_t + v * sin_t;

        vertices.emplace_back(pos + dir * r);
        normals.emplace_back(-axis);
        texCoords.emplace_back(0.5f + 0.5f * cos_t, 0.5f + 0.5f * sin_t);
    }

    // ׶����
    vertices.emplace_back(p1);
    normals.emplace_back(axis);
    texCoords.emplace_back(0.5f, 1.0f);

    // ����ײ���Ե��
    for (int i = 0; i <= slice; ++i) {
        float theta = i * 2 * PI / slice;
        float cos_t = cos(theta);
        float sin_t = sin(theta);

        CVector u = axis.crossMul(CVector(1, 0, 0)).normalize();
        CVector v = axis.crossMul(u).normalize();
        CVector dir = u * cos_t + v * sin_t;

        vertices.emplace_back(pos + dir * r);
        normals.emplace_back(dir);
        texCoords.emplace_back(static_cast<float>(i) / slice, 0.0f);
    }

    // ����������
    for (int i = 0; i < slice; ++i) {
        indices.emplace_back(0); // �ײ�����
        indices.emplace_back(i + 1);
        indices.emplace_back(i + 2);
    }

    // ����������
    for (int i = 0; i < slice; ++i) {
        indices.emplace_back(slice + 2); // ׶��
        indices.emplace_back(slice + 3 + i);
        indices.emplace_back(slice + 4 + i);
    }
}

Cone::Cone(const CVector& pos, const CVector& axis, float r, float h, const CVector& color)
	: Geometry(pos, color), axis(axis), r(r), h(h) {
	this->slice = r * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(r * DEFAULT_SLICE) : MIN_SLICE;
	this->axis.normalize();
	createVertices();
}

Cone::~Cone() {}