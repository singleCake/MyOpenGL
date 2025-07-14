#include "Cuboid.h"

#include "../manager/RenderManager.h"
#include "../manager/TextureManager.h"

Cuboid::Cuboid(const CVector& pos, float l, float w, float h, const CVector& color)
	: Cuboid(pos, CVector(0, 1, 0), l, w, h, color) {}

Cuboid::Cuboid(const CVector& pos, const CVector& axis, float l, float w, float h, const CVector& color)
	: Geometry(pos, color), axis(axis), l(l), w(w), h(h) {
	this->axis.normalize();
	createVertices();
}

void Cuboid::createVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    if (axis.y == 0 && axis.z == 0) {
        u = CVector(0, 0, 1);
        v = CVector(0, 1, 0);
    }
    else {
        u = axis.crossMul(CVector(1, 0, 0)).normalize();
        v = axis.crossMul(u).normalize();
    }

    CVector frontNormal = u.crossMul(v).normalize();

    // 24�����㣨6���� * 4�����㣩��ÿ�������
    // ÿ����ʹ�ò�ͬ�ķ���������������

    // ����λ�� (8���ǵ�)
    CVector corners[8] = {
        pos - u * l * 0.5f - v * w * 0.5f - axis * h * 0.5f, // 0
        pos + u * l * 0.5f - v * w * 0.5f - axis * h * 0.5f, // 1
        pos + u * l * 0.5f + v * w * 0.5f - axis * h * 0.5f, // 2
        pos - u * l * 0.5f + v * w * 0.5f - axis * h * 0.5f, // 3
        pos - u * l * 0.5f - v * w * 0.5f + axis * h * 0.5f, // 4
        pos + u * l * 0.5f - v * w * 0.5f + axis * h * 0.5f, // 5
        pos + u * l * 0.5f + v * w * 0.5f + axis * h * 0.5f, // 6
        pos - u * l * 0.5f + v * w * 0.5f + axis * h * 0.5f  // 7
    };

    // ����6����Ķ��������ͷ�����
    struct Face {
        int v[4];        // 4���������� (��˳��)
        CVector normal;  // �淨����
        CVector2 uv[4];  // �������� (����, ����, ����, ����)
    };

    // ������6���涨��
    Face faces[6] = {
        // ���� (-axis)
        {{0, 1, 2, 3}, -frontNormal,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // ���� (+axis)
        {{4, 5, 6, 7}, frontNormal,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // ǰ�� (-v)
        {{0, 1, 5, 4}, -v,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // ���� (+v)
        {{3, 2, 6, 7}, v,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // ���� (-u)
        {{0, 3, 7, 4}, -u,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // ���� (+u)
        {{1, 2, 6, 5}, u,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}}
    };

    // ���ɶ�������
    for (int face = 0; face < 6; face++) {
        int startIndex = vertices.size();

        // ���4������
        for (int i = 0; i < 4; i++) {
            vertices.push_back(corners[faces[face].v[i]]);
            normals.push_back(faces[face].normal);
            texCoords.push_back(faces[face].uv[i]);
        }

        // ���2�������� (6������)
        indices.push_back(startIndex);
        indices.push_back(startIndex + 1);
        indices.push_back(startIndex + 2);

        indices.push_back(startIndex);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 3);
    }
}

void Cuboid::trans(const CMatrix& m) {
	Geometry::trans(m);

	axis = m.vecMul(axis);
	u = m.vecMul(u);
	v = m.vecMul(v);
}

bool Cuboid::checkCollision(const Ball& other) const {
	// �������ĵ����������ĵ�����
	CVector d = other.pos - pos;

	// ���������ڳ�����ֲ�����ϵ�е�ͶӰ
	float distX = std::abs(d.dotMul(axis));
	float distY = std::abs(d.dotMul(u));
	float distZ = std::abs(d.dotMul(v));

	// ��������Ƿ��ڳ�����ķ�Χ��
    if (distX > l * 0.5f + other.r || distY > w * 0.5f + other.r || distZ > h * 0.5f + other.r) {
		return false; // ����
	}

	// ����볤�������������
    if (distX <= l * 0.5f && distY <= w * 0.5f && distZ <= h * 0.5f) {
		return true; // ��ײ
	}

	// ����볤�����Ե�ľ���
	float dx = std::max(distX - l * 0.5f, 0.0f);
	float dy = std::max(distY - w * 0.5f, 0.0f);
	float dz = std::max(distZ - h * 0.5f, 0.0f);

	return (dx * dx + dy * dy + dz * dz) <= (other.r * other.r);
}

inline static void projectBox(const Cuboid& box, const CVector& axis, float& minVal, float& maxVal) {
	// ����8�����������ϵ�ͶӰֵ
	std::vector<CVector> vertices = box.vertices;
	minVal = maxVal = vertices[0].dotMul(axis);
	for (const CVector& v : vertices) {
		float proj = v.dotMul(axis);
		if (proj < minVal) minVal = proj;
		if (proj > maxVal) maxVal = proj;
	}
}

inline static bool isContains(const Cuboid& a, const Cuboid& b) {
	for (int i = 0; i < 3; i++) {
		CVector axis = *a.axes[i];
		float aMin, aMax, bMin, bMax;
		projectBox(a, axis, aMin, aMax);
		projectBox(b, axis, bMin, bMax);
		if (bMin < aMin || bMax > aMax) {
			return false;
		}
	}
	return true;
}

bool Cuboid::checkCollision(const Cuboid& other) const {
    std::vector<CVector> axes;

    for (int i = 0; i < 3; i++) {
        axes.push_back(this->axes[i]->normalize());

        axes.push_back(other.axes[i]->normalize());

        for (int j = 0; j < 3; j++) {
            CVector cross1 = this->axes[i]->crossMul(*other.axes[j]);
            if (cross1.len() > 1e-5) {
                axes.push_back(cross1.normalize());
            }
        }
    }
    bool allAxesOverlap = true;
    for (const CVector& axis : axes) {
        float aMin, aMax;
        projectBox(*this, axis, aMin, aMax);

        float bMin, bMax;
        projectBox(other, axis, bMin, bMax);

        const float epsilon = 1e-5f;
        if (aMax < bMin - epsilon || bMax < aMin - epsilon) {
            return false;
        }
    }
    return !(isContains(*this, other) || isContains(other, *this));
}

bool Cuboid::checkCollision(const Cuboid& other, CVector& collisionPoint) const {
    std::vector<CVector> axes;
    for (int i = 0; i < 3; i++) {
        axes.push_back(this->axes[i]->normalize());
        axes.push_back(other.axes[i]->normalize());

        for (int j = 0; j < 3; j++) {
            CVector cross1 = this->axes[i]->crossMul(*other.axes[j]);
            if (cross1.len() > 1e-5) {
                axes.push_back(cross1.normalize());
            }
        }
    }

    bool collision = true;
    float minOverlap = FLT_MAX;          // ��С��͸���
    CVector minAxis;                     // ��С��͸��
    float minOverlapMin = 0, minOverlapMax = 0; // ��С��͸���ϵ��ص�����

    // ������з�����
    for (const CVector& axis : axes) {
        float aMin, aMax;
        projectBox(*this, axis, aMin, aMax);  // ͶӰ��ǰ������

        float bMin, bMax;
        projectBox(other, axis, bMin, bMax);  // ͶӰ��һ��������

        // �����ص�����
        float overlapMin = std::max(aMin, bMin);
        float overlapMax = std::min(aMax, bMax);
        float overlap = overlapMax - overlapMin;

        const float epsilon = 1e-5f;
        // ����Ƿ����
        if (overlapMax < overlapMin - epsilon) {
            return false;  // ���ַ����ᣬ����ײ
        }

        // ������С��͸����Ϣ
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = axis;
            minOverlapMin = overlapMin;
            minOverlapMax = overlapMax;
        }
    }

    // ��������ϵ��һ����������ȫ������һ����
    if (isContains(*this, other) || isContains(other, *this)) {
        return false;
    }

    // === ������ײ�� ===
    CVector C1 = pos;     // ��ǰ�����������
    CVector C2 = other.pos;     // ��һ�������������
    CVector P0 = (C1 + C2) * 0.5f; // ���ĵ��е�

    // ����P0����С��͸���ϵ�ͶӰ
    float proj = P0.dotMul(minAxis);
    // �����ص����������ͶӰֵ
    float centerProj = (minOverlapMin + minOverlapMax) * 0.5f;
    // ������ײ��λ��
    collisionPoint = P0 + minAxis * (centerProj - proj);

    return true;
}

void Cuboid::drawWithTwoTextures(const char* texture1, const char* texture2) const {
    // ��������
    RenderManager::drawPrimitiveWithTexture(
        TextureManager::getTexture(texture1), vertices, indices, normals, texCoords, 30, 36);
    
    // ���Ʊ���
    RenderManager::drawPrimitiveWithTexture(
		TextureManager::getTexture(texture2), vertices, indices, normals, texCoords, 24, 30);

    // ���������棬������
    RenderManager::drawPrimitive(
		color, vertices, indices, normals, false, 0, 24);
}