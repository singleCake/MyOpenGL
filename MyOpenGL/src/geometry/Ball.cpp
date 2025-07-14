#include "Ball.h"
#include "../manager/RenderManager.h"

Ball::Ball(const CVector& pos, float r, const CVector& color, bool src)
	: Ball(pos, r, CVector(0, 1, 0), color, src) {}

Ball::Ball(const CVector& pos, float r, const CVector& axis, const CVector& color, bool src)
	: r(r), src(src), Geometry(pos, color) {
	slice = r * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(r * DEFAULT_SLICE) : MIN_SLICE;
	createVertices();
}

void Ball::createVertices() {
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();

    vertices.reserve((slice + 1) * (slice + 1) + 2);
    normals.reserve((slice + 1) * (slice + 1) + 2);
    texCoords.reserve((slice + 1) * (slice + 1) + 2);
    indices.reserve(slice * slice * 6);

    // 南极顶点
    vertices.emplace_back(pos.x, pos.y - r, pos.z);
    if (!src)
        normals.emplace_back(0.0f, -1.0f, 0.0f);
    else 
        normals.emplace_back(0.0f, 1.0f, 0.0f);
    texCoords.emplace_back(0.5f, 0.0f);

    for (int i = 1; i < slice; ++i) {
        float phi = -PI / 2.0f + i * PI / slice;
        float y_local = r * sin(phi);
        float r_xy = r * cos(phi);

        for (int j = 0; j <= slice; ++j) {
            float theta = j * 2 * PI / slice;
            float x_local = r_xy * cos(theta);
            float z_local = r_xy * sin(theta);

            // 顶点位置
            float x = x_local + pos.x;
            float y = y_local + pos.y;
            float z = z_local + pos.z;
            vertices.emplace_back(x, y, z);

            // 法向量（局部坐标归一化）
            float length = sqrt(x_local * x_local + y_local * y_local + z_local * z_local);
            if (!src)
                normals.emplace_back(x_local / length, y_local / length, z_local / length);
            else // 如果是光源，法向量需要反向
                normals.emplace_back(-x_local / length, -y_local / length, -z_local / length);

            // 纹理坐标（球面映射）
            float u = static_cast<float>(j) / slice;
            float v = static_cast<float>(i - 1) / (slice - 2);
            texCoords.emplace_back(u, v);
        }
    }

    // 北极顶点
    vertices.emplace_back(pos.x, pos.y + r, pos.z);
    if (!src)
        normals.emplace_back(0.0f, 1.0f, 0.0f);
    else
        normals.emplace_back(0.0f, -1.0f, 0.0f);
    texCoords.emplace_back(0.5f, 1.0f);

    // 南极索引
    for (int j = 0; j < slice; ++j) {
        indices.push_back(0);
        indices.push_back(1 + j);
        indices.push_back(1 + (j + 1) % (slice + 1));
    }

    // 中间层索引
    for (int i = 0; i < slice - 2; ++i) {
        int rowStart = 1 + i * (slice + 1);
        int nextRowStart = 1 + (i + 1) * (slice + 1);

        for (int j = 0; j <= slice; ++j) {
            int nextJ = (j + 1) % (slice + 1);

            // 第一个三角形 (A-B-C)
            indices.push_back(rowStart + j);
            indices.push_back(nextRowStart + j);
            indices.push_back(rowStart + nextJ);

            // 第二个三角形 (B-D-C)
            indices.push_back(nextRowStart + j);
            indices.push_back(nextRowStart + nextJ);
            indices.push_back(rowStart + nextJ);
        }
    }

    // 北极索引
    int northPoleIndex = vertices.size() - 1; // 北极点索引
    int lastRowStart = 1 + (slice - 2) * (slice + 1); // 最后一层起始索引

    for (int j = 0; j < slice; ++j) {
        indices.push_back(lastRowStart + j);
        indices.push_back(lastRowStart + (j + 1) % (slice + 1));
        indices.push_back(northPoleIndex);
    }
}

bool Ball::raySphereIntersect(const CVector& rayOrigin, const CVector& rayDir, float& out_t) const {
	CVector oc = rayOrigin - CVector(pos.x, pos.y, pos.z);
	float a = rayDir.dotMul(rayDir);
	float b = 2.0f * oc.dotMul(rayDir);
	float c = oc.dotMul(oc) - r * r;
	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) return false;

	float sqrtDisc = sqrt(discriminant);
	float t0 = (-b - sqrtDisc) / (2 * a);
	float t1 = (-b + sqrtDisc) / (2 * a);

	out_t = (t0 < t1) ? t0 : t1;
	return out_t >= 0;
}