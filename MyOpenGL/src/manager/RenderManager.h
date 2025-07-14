#pragma once

#include "../math/CVector.h"
#include "../math/CVector2.h"
#include <vector>

typedef unsigned int GLuint;

namespace RenderManager {
	inline bool LINE_MODE = false;

	void init();

	void beginRender(const CVector& color);
	void endRender();
	void beginRenderWithTexture(const GLuint& texture);
	void endRenderWithTexture();
	void render();
	void render2D();

	// 绘制特定图元
	void drawPrimitive(const CVector&, const std::vector<CVector>&, const std::vector<int>&, const std::vector<CVector>&, bool line = false, size_t begin = 0, size_t end = 0);
	void drawPrimitiveWithTexture(const GLuint&, const std::vector<CVector>&, const std::vector<int>&, const std::vector<CVector>&, const std::vector<CVector2>&, size_t begin = 0, size_t end = 0, bool line = false);

	// 绘制点
	void drawPoint(const CVector& color, const CVector& pos, float size = 1.0f);

	// 绘制线段
	void drawLine(const CVector& color, const CVector& start, const CVector& end);

	// 绘制圆
	void drawCircle(const CVector& color, const CVector& center, const CVector& axis, float radius);

	// 绘制三角形
	void drawTriangle(const CVector& color, const CVector& A, const CVector& B, const CVector& C);

	// 绘制箭头
	void drawArrow(float size, const CVector& color, const CVector& start, const CVector& end);

	// 绘制坐标轴
	void drawAxis(float arrowSize);

	// 绘制文字
	void drawText(const char* text, const CVector2& pos, const CVector& color = { 1, 1, 1 });
}