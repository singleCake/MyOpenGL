#include "Pyramid.h"

void Pyramid::createVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    // Ϊÿ��������洢���㣨4 * 3 = 12��
    vertices.reserve(12);
    normals.reserve(12);
    texCoords.reserve(12);
    indices.reserve(12);

    // ����λ��
    CVector v0 = pos;           // 0��׶��
    CVector v1 = pos + u * x;   // 1
    CVector v2 = pos + v * y;   // 2
    CVector v3 = pos + w * z;   // 3

    // ���㷨����
    CVector normalFront = (v1 - v0).crossMul(v2 - v0).normalize(); // ǰ�淨��
    CVector normalRight = (v2 - v0).crossMul(v3 - v0).normalize(); // ���淨��
    CVector normalBack = (v3 - v0).crossMul(v1 - v0).normalize(); // ���淨��
    CVector normalBase = (v2 - v1).crossMul(v3 - v1).normalize(); // ���淨��

    // ���ǰ�������� (v0, v1, v2)
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    normals.push_back(normalFront);
    normals.push_back(normalFront);
    normals.push_back(normalFront);
    texCoords.emplace_back(0.5f, 1.0f); // ׶��
    texCoords.emplace_back(0.0f, 0.0f); // ����
    texCoords.emplace_back(1.0f, 0.0f); // ����

    // ������������� (v0, v2, v3)
    vertices.push_back(v0);
    vertices.push_back(v2);
    vertices.push_back(v3);
    normals.push_back(normalRight);
    normals.push_back(normalRight);
    normals.push_back(normalRight);
    texCoords.emplace_back(0.5f, 1.0f); // ׶��
    texCoords.emplace_back(0.0f, 0.0f); // ����
    texCoords.emplace_back(1.0f, 0.0f); // ����

    // ��Ӻ��������� (v0, v3, v1)
    vertices.push_back(v0);
    vertices.push_back(v3);
    vertices.push_back(v1);
    normals.push_back(normalBack);
    normals.push_back(normalBack);
    normals.push_back(normalBack);
    texCoords.emplace_back(0.5f, 1.0f); // ׶��
    texCoords.emplace_back(0.0f, 0.0f); // ����
    texCoords.emplace_back(1.0f, 0.0f); // ����

    // ��ӵ��������� (v1, v2, v3)
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    normals.push_back(normalBase);
    normals.push_back(normalBase);
    normals.push_back(normalBase);
    texCoords.emplace_back(0.0f, 0.0f); // ����
    texCoords.emplace_back(1.0f, 0.0f); // ����
    texCoords.emplace_back(0.5f, 1.0f); // ����

    for (int i = 0; i < 12; i++) {
        indices.push_back(i);
    }
}

Pyramid::Pyramid(const CVector& pos,
	const CVector& u, const CVector& v, const CVector& w,
	float x, float y, float z, const CVector& color)
	: Geometry(pos, color), u(u), v(v), w(w), x(x), y(y), z(z) {
	this->u.normalize();
	this->v.normalize();
	this->w.normalize();
	createVertices();
}

Pyramid::~Pyramid() {
}