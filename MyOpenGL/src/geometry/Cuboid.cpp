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

    // 24个顶点（6个面 * 4个顶点），每个面独立
    // 每个面使用不同的法向量和纹理坐标

    // 顶点位置 (8个角点)
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

    // 定义6个面的顶点索引和法向量
    struct Face {
        int v[4];        // 4个顶点索引 (按顺序)
        CVector normal;  // 面法向量
        CVector2 uv[4];  // 纹理坐标 (左下, 右下, 右上, 左上)
    };

    // 立方体6个面定义
    Face faces[6] = {
        // 底面 (-axis)
        {{0, 1, 2, 3}, -frontNormal,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // 顶面 (+axis)
        {{4, 5, 6, 7}, frontNormal,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // 前面 (-v)
        {{0, 1, 5, 4}, -v,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // 后面 (+v)
        {{3, 2, 6, 7}, v,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // 左面 (-u)
        {{0, 3, 7, 4}, -u,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}},
        // 右面 (+u)
        {{1, 2, 6, 5}, u,
        {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}}}
    };

    // 生成顶点数据
    for (int face = 0; face < 6; face++) {
        int startIndex = vertices.size();

        // 添加4个顶点
        for (int i = 0; i < 4; i++) {
            vertices.push_back(corners[faces[face].v[i]]);
            normals.push_back(faces[face].normal);
            texCoords.push_back(faces[face].uv[i]);
        }

        // 添加2个三角形 (6个索引)
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
	// 计算球心到长方体中心的向量
	CVector d = other.pos - pos;

	// 计算球心在长方体局部坐标系中的投影
	float distX = std::abs(d.dotMul(axis));
	float distY = std::abs(d.dotMul(u));
	float distZ = std::abs(d.dotMul(v));

	// 检查球心是否在长方体的范围内
    if (distX > l * 0.5f + other.r || distY > w * 0.5f + other.r || distZ > h * 0.5f + other.r) {
		return false; // 分离
	}

	// 检查与长方体的最近点距离
    if (distX <= l * 0.5f && distY <= w * 0.5f && distZ <= h * 0.5f) {
		return true; // 碰撞
	}

	// 检查与长方体边缘的距离
	float dx = std::max(distX - l * 0.5f, 0.0f);
	float dy = std::max(distY - w * 0.5f, 0.0f);
	float dz = std::max(distZ - h * 0.5f, 0.0f);

	return (dx * dx + dy * dy + dz * dz) <= (other.r * other.r);
}

inline static void projectBox(const Cuboid& box, const CVector& axis, float& minVal, float& maxVal) {
	// 计算8个顶点在轴上的投影值
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
    float minOverlap = FLT_MAX;          // 最小穿透深度
    CVector minAxis;                     // 最小穿透轴
    float minOverlapMin = 0, minOverlapMax = 0; // 最小穿透轴上的重叠区间

    // 检查所有分离轴
    for (const CVector& axis : axes) {
        float aMin, aMax;
        projectBox(*this, axis, aMin, aMax);  // 投影当前长方体

        float bMin, bMax;
        projectBox(other, axis, bMin, bMax);  // 投影另一个长方体

        // 计算重叠区间
        float overlapMin = std::max(aMin, bMin);
        float overlapMax = std::min(aMax, bMax);
        float overlap = overlapMax - overlapMin;

        const float epsilon = 1e-5f;
        // 检查是否分离
        if (overlapMax < overlapMin - epsilon) {
            return false;  // 发现分离轴，无碰撞
        }

        // 更新最小穿透轴信息
        if (overlap < minOverlap) {
            minOverlap = overlap;
            minAxis = axis;
            minOverlapMin = overlapMin;
            minOverlapMax = overlapMax;
        }
    }

    // 检查包含关系（一个长方体完全包含另一个）
    if (isContains(*this, other) || isContains(other, *this)) {
        return false;
    }

    // === 计算碰撞点 ===
    CVector C1 = pos;     // 当前长方体的中心
    CVector C2 = other.pos;     // 另一个长方体的中心
    CVector P0 = (C1 + C2) * 0.5f; // 中心点中点

    // 计算P0在最小穿透轴上的投影
    float proj = P0.dotMul(minAxis);
    // 计算重叠区间的中心投影值
    float centerProj = (minOverlapMin + minOverlapMax) * 0.5f;
    // 调整碰撞点位置
    collisionPoint = P0 + minAxis * (centerProj - proj);

    return true;
}

void Cuboid::drawWithTwoTextures(const char* texture1, const char* texture2) const {
    // 绘制正面
    RenderManager::drawPrimitiveWithTexture(
        TextureManager::getTexture(texture1), vertices, indices, normals, texCoords, 30, 36);
    
    // 绘制背面
    RenderManager::drawPrimitiveWithTexture(
		TextureManager::getTexture(texture2), vertices, indices, normals, texCoords, 24, 30);

    // 绘制其他面，无纹理
    RenderManager::drawPrimitive(
		color, vertices, indices, normals, false, 0, 24);
}