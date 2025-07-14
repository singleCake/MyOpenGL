#include "RenderManager.h"
#include "EntityManager.h"
#include "CameraManager.h"
#include "BackgroundManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "../Application.h"

#include "../entity/HitBox.h"

#include "../constant/Color.h"
#include "../constant/BackgroundConstant.h"

static inline void drawCollisionInfo() {
	// 绘制飞船碰撞信息
	RenderManager::drawText("SpaceCraft Collided with:", { -0.9, 0.9 });
	if (spaceCraftCollisionInfo) {
		std::string collisionWith = spaceCraftCollisionInfo->collisionWith;
		RenderManager::drawText(collisionWith.c_str(), { -0.9, 0.85 });
		RenderManager::drawText("Collision Time:", { -0.9, 0.8 });
		RenderManager::drawText(std::ctime(&spaceCraftCollisionInfo->collisionTime), {-0.9, 0.75});
		RenderManager::drawText((collisionWith + " Position:").c_str(), {-0.9, 0.7});
		RenderManager::drawText(spaceCraftCollisionInfo->collisionPoint.toString().c_str(), { -0.9, 0.65 });
	}
	else {
		RenderManager::drawText("Nothing", { -0.9, 0.85 });
	}

	// 绘制玩家碰撞信息
	RenderManager::drawText("Player Collided with:", { 0.4, 0.9 });
	if (playerCollisionInfo) {
		std::string collisionWith = playerCollisionInfo->collisionWith;
		RenderManager::drawText(collisionWith.c_str(), { 0.4, 0.85 });
		RenderManager::drawText("Player Position:", { 0.4, 0.8 });
		RenderManager::drawText(playerCollisionInfo->playerPosition.toString().c_str(), { 0.4, 0.75 });
		RenderManager::drawText((collisionWith + " Position:").c_str(), { 0.4, 0.7 });
		RenderManager::drawText(playerCollisionInfo->otherPosition.toString().c_str(), { 0.4, 0.65 });
		RenderManager::drawText("Collision Point:", { 0.4, 0.6 });
		RenderManager::drawText(playerCollisionInfo->collisionPoint.toString().c_str(), { 0.4, 0.55 });
	} 
	else {
		RenderManager::drawText("Nothing", { 0.4, 0.85 });
	}
}

static inline void drawReticle() {
	// 绘制十字准星
	float size = 25; // 准星尺寸
	float lineWidth = 2.0f;
	float x = size / Application::WIDTH, y = size / Application::HEIGHT;

	glColor3f(0.5f, 0.5f, 0.5f);
	glLineWidth(lineWidth);

	glBegin(GL_LINES);
	glVertex2f(-x, 0);
	glVertex2f(x, 0);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0, -y);
	glVertex2f(0, y);
	glEnd();
}

void RenderManager::render2D() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); // 备份投影矩阵
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // 备份模型视图矩阵
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	drawReticle();
	drawCollisionInfo();

	//恢复状态
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RenderManager::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	drawAxis(AXIS_ARROW_SIZE);

	EntityManager::draw();
	BackgroundManager::draw();
	render2D();
	glPopMatrix();
}

static GLfloat prevDiffuse[4], prevSpecular[4], prevShininess[1];

void RenderManager::beginRender(const CVector& color) {
	LightManager::enableAllLights();

	// 保存当前材质状态
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, prevDiffuse);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, prevSpecular);
	glGetMaterialfv(GL_FRONT, GL_SHININESS, prevShininess);

	// 设置物体材质
	GLfloat matDiffuse[] = { color.x, color.y, color.z, 1.0f };
	GLfloat matSpecular[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // 添加镜面反射
	GLfloat matShininess[] = { 64.0f }; // 高光指数

	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

void RenderManager::endRender() {
	LightManager::disableAllLights();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, prevDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, prevSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, prevShininess);
}

void RenderManager::beginRenderWithTexture(const GLuint& texture) {
	LightManager::enableAllLights();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void RenderManager::endRenderWithTexture() {
	LightManager::disableAllLights();
	glDisable(GL_TEXTURE_2D);
}

void RenderManager::drawPrimitive(
	const CVector& color,
	const std::vector<CVector>& vertices, const std::vector<int>& indices, const std::vector<CVector>& normals,
	bool line, size_t begin, size_t end) {
	if (end == 0)
		end = indices.size();
	bool lineMode = LINE_MODE || line;
	if (lineMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	beginRender(color);

	glBegin(GL_TRIANGLES);
	for (size_t i = begin; i < end; ++i) {
		int index = indices[i];
		if (index >= 0 && index < vertices.size()) {
			glNormal3fv(normals[index]);
			glVertex3fv(vertices[index]);
		}
	}
	glEnd();
	endRender();
}

void RenderManager::drawPrimitiveWithTexture(
	const GLuint& texture,
	const std::vector<CVector>& vertices, const std::vector<int>& indices, 
	const std::vector<CVector>& normals, const std::vector<CVector2>& texCoords, 
	size_t begin, size_t end, bool line) {
	if (end == 0)
		end = indices.size();
	beginRenderWithTexture(texture);
	if (LINE_MODE || line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (size_t i = begin; i < end; ++i) {
		int index = indices[i];
		if (index >= 0 && index < vertices.size()) {
			glTexCoord2fv(texCoords[index]);
			glNormal3fv(normals[index]);
			glVertex3fv(vertices[index]);
		}
	}
	glEnd();

	endRenderWithTexture();
}

void RenderManager::drawPoint(const CVector& color, const CVector& pos, float size) {
	glBegin(GL_POINT);
	glPointSize(size);
	glColor3fv(color);
	glVertex3fv(pos);
	glEnd();
}

void RenderManager::drawLine(const CVector& color, const CVector& start, const CVector& end) {
	glBegin(GL_LINES);
	glColor3fv(color);
	glVertex3fv(start);
	glVertex3fv(end);
	glEnd();
}

void RenderManager::drawCircle(const CVector& color, const CVector& center, const CVector& axis, float radius) {
	CVector normalizedAxis = axis;
	normalizedAxis.normalize();
	CVector u, v;

	CVector ref(1, 0, 0);
	if (fabs(normalizedAxis.dotMul(ref)) > 0.9f)
		ref = CVector(0, 1, 0);

	u = ref.crossMul(normalizedAxis).normalize();
	v = normalizedAxis.crossMul(u).normalize();

	int numSegments = radius * DEFAULT_SLICE > MIN_SLICE ? static_cast<int>(radius * DEFAULT_SLICE) : MIN_SLICE;

	const float angleStep = 2 * PI / numSegments;
	glBegin(GL_LINE_LOOP);
	glColor3fv(color);

	for (int i = 0; i <= numSegments; ++i) {
		float theta = i * angleStep;
		CVector offset = (u * cos(theta) + v * sin(theta)) * radius;
		glVertex3fv(center + offset);
	}

	glEnd();
}

void RenderManager::drawTriangle(const CVector& color, const CVector& A, const CVector& B, const CVector& C) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	glColor3fv(color);
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glEnd();
}

void RenderManager::drawArrow(float size, const CVector& color, const CVector& start, const CVector& end) {
	RenderManager::drawLine(color, start, end);

	float L = (end - start).len();
	CVector d = (start - end).normalize();
	float tmp = size / L;
	CVector mid = (1 - tmp) * end + tmp * start;

	CVector a = (d.y == 0 && d.z == 0) ? CVector(0, 1, 0) : CVector(1, 0, 0);
	CVector u = a.crossMul(d).normalize();
	CVector v = d.crossMul(u).normalize();
	float h = size, s = size / 2;

	CVector C = end + d * h;
	CVector D = C + u * s;
	CVector E = C - u * s;
	CVector F = C + v * s;
	CVector G = C - v * s;

	RenderManager::drawTriangle(color, end, mid, D);
	RenderManager::drawTriangle(color, end, mid, E);
	RenderManager::drawTriangle(color, end, mid, F);
	RenderManager::drawTriangle(color, end, mid, G);
}

void RenderManager::drawAxis(float arrowSize) {
	drawArrow(arrowSize, RED, CVector(0, 0, 0), CVector(AXIS_LENGTH, 0, 0));
	drawArrow(arrowSize, GREEN, CVector(0, 0, 0), CVector(0, AXIS_LENGTH, 0));
	drawArrow(arrowSize, BLUE, CVector(0, 0, 0), CVector(0, 0, AXIS_LENGTH));
}

void RenderManager::drawText(const char* text, const CVector2& pos, const CVector& color) {
	glColor3fv(color);
	//输出字符串
	int len = (int)strlen(text);
	glRasterPos2fv(pos);
	for (size_t i = 0; i < strlen(text); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	
}