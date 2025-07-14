#include "CMatrix.h"
#include "CEuler.h"
#include "CQuaternion.h"
#include <array>

CMatrix::CMatrix() {
	m00 = 0.0f; m10 = 0.0f; m20 = 0.0f; m30 = 0.0f;
	m01 = 0.0f; m11 = 0.0f; m21 = 0.0f; m31 = 0.0f;
	m02 = 0.0f; m12 = 0.0f; m22 = 0.0f; m32 = 0.0f;
	m03 = 0.0f; m13 = 0.0f; m23 = 0.0f; m33 = 0.0f;
}

CMatrix::CMatrix(float val[16]) {
	this->set(val);
}

CMatrix::CMatrix(const float val[16]) {
	m00 = val[0]; m10 = val[1]; m20 = val[2]; m30 = val[3];
	m01 = val[4]; m11 = val[5]; m21 = val[6]; m31 = val[7];
	m02 = val[8]; m12 = val[9]; m22 = val[10]; m32 = val[11];
	m03 = val[12]; m13 = val[13]; m23 = val[14]; m33 = val[15];
}

void CMatrix::set(float* val) {
	m00 = val[0]; m10 = val[1]; m20 = val[2]; m30 = val[3];
	m01 = val[4]; m11 = val[5]; m21 = val[6]; m31 = val[7];
	m02 = val[8]; m12 = val[9]; m22 = val[10]; m32 = val[11];
	m03 = val[12]; m13 = val[13]; m23 = val[14]; m33 = val[15];
}

const CMatrix& CMatrix::orthogonalize() {
	CVector& x = *(CVector*)&m00;
	CVector& y = *(CVector*)&m01;
	CVector& z = *(CVector*)&m02;

	x.normalize();
	y = y - x * (x.dotMul(y));
	y.normalize();
	z = z - x * (x.dotMul(z)) - y * (y.dotMul(z));
	z.normalize();
	m00 = x.x; m10 = x.y; m20 = x.z; m30 = 0.0f;
	m01 = y.x; m11 = y.y; m21 = y.z; m31 = 0.0f;
	m02 = z.x; m12 = z.y; m22 = z.z; m32 = 0.0f;
	m03 = 0.0f; m13 = 0.0f; m23 = 0.0f; m33 = 1.0f;

	return *this;
}

std::string CMatrix::toString() const {
	std::string str = "";
	for (int i = 0; i < 15; i++) {
		str += std::to_string(((float*)this)[i]) + ',';
	}
	str += std::to_string(((float*)this)[15]);
	return str;
}

const CMatrix& CMatrix::operator=(const CMatrix& p) {
	this->m00 = p.m00; this->m10 = p.m10; this->m20 = p.m20; this->m30 = p.m30;
	this->m01 = p.m01; this->m11 = p.m11; this->m21 = p.m21; this->m31 = p.m31;
	this->m02 = p.m02; this->m12 = p.m12; this->m22 = p.m22; this->m32 = p.m32;
	this->m03 = p.m03; this->m13 = p.m13; this->m23 = p.m23; this->m33 = p.m33;
	return *this;
}

const CMatrix& CMatrix::operator*=(const CMatrix& p) {
	this->m00 *= p.m00; this->m10 *= p.m10; this->m20 *= p.m20; this->m30 *= p.m30;
	this->m01 *= p.m01; this->m11 *= p.m11; this->m21 *= p.m21; this->m31 *= p.m31;
	this->m02 *= p.m02; this->m12 *= p.m12; this->m22 *= p.m22; this->m32 *= p.m32;
	this->m03 *= p.m03; this->m13 *= p.m13; this->m23 *= p.m23; this->m33 *= p.m33;
	return *this;
}

CMatrix CMatrix::operator+(const CMatrix& m) const {
	float val[16] = {
		m00 + m.m00, m10 + m.m10, m20 + m.m20, m30 + m.m30,
		m01 + m.m01, m11 + m.m11, m21 + m.m21, m31 + m.m31,
		m02 + m.m02, m12 + m.m12, m22 + m.m22, m32 + m.m32,
		m03 + m.m03, m13 + m.m13, m23 + m.m23, m33 + m.m33
	};
	return CMatrix(val);
}

CMatrix CMatrix::operator*(float d) const {
	float val[16] = {
		m00 * d, m10 * d, m20 * d, m30 * d,
		m01 * d, m11 * d, m21 * d, m31 * d,
		m02 * d, m12 * d, m22 * d, m32 * d,
		m03 * d, m13 * d, m23 * d, m33 * d
	};
	return CMatrix(val);
}

CMatrix operator*(float d, const CMatrix& m) {
	return m * d;
}

CMatrix CMatrix::operator*(const CMatrix& p) const {
	float val[16] = {
		m00 * p.m00 + m01 * p.m10 + m02 * p.m20 + m03 * p.m30,
		m10 * p.m00 + m11 * p.m10 + m12 * p.m20 + m13 * p.m30,
		m20 * p.m00 + m21 * p.m10 + m22 * p.m20 + m23 * p.m30,
		m30 * p.m00 + m31 * p.m10 + m32 * p.m20 + m33 * p.m30,
		m00 * p.m01 + m01 * p.m11 + m02 * p.m21 + m03 * p.m31,
		m10 * p.m01 + m11 * p.m11 + m12 * p.m21 + m13 * p.m31,
		m20 * p.m01 + m21 * p.m11 + m22 * p.m21 + m23 * p.m31,
		m30 * p.m01 + m31 * p.m11 + m32 * p.m21 + m33 * p.m31,
		m00 * p.m02 + m01 * p.m12 + m02 * p.m22 + m03 * p.m32,
		m10 * p.m02 + m11 * p.m12 + m12 * p.m22 + m13 * p.m32,
		m20 * p.m02 + m21 * p.m12 + m22 * p.m22 + m23 * p.m32,
		m30 * p.m02 + m31 * p.m12 + m32 * p.m22 + m33 * p.m32,
		m00 * p.m03 + m01 * p.m13 + m02 * p.m23 + m03 * p.m33,
		m10 * p.m03 + m11 * p.m13 + m12 * p.m23 + m13 * p.m33,
		m20 * p.m03 + m21 * p.m13 + m22 * p.m23 + m23 * p.m33,
		m30 * p.m03 + m31 * p.m13 + m32 * p.m23 + m33 * p.m33
	};
	return CMatrix(val);
}

CVector CMatrix::vecMul(const CVector& v) const {
	float x = m00 * v.x + m01 * v.y + m02 * v.z;
	float y = m10 * v.x + m11 * v.y + m12 * v.z;
	float z = m20 * v.x + m21 * v.y + m22 * v.z;
	return { x, y, z };
}

CVector CMatrix::posMul(const CVector& v) const {
	float x = m00 * v.x + m01 * v.y + m02 * v.z + m03;
	float y = m10 * v.x + m11 * v.y + m12 * v.z + m13;
	float z = m20 * v.x + m21 * v.y + m22 * v.z + m23;
	return { x, y, z };
}

float CMatrix::det() const {
	auto calculate3x3 = [](const std::array < std::array < float , 3 > , 3 > &m) {
		return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
			- m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
			+ m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
		};

	float det = 0.0;
	int sign = 1;
	for (int col = 0; col < 4; ++col) {
		// 生成子矩阵（排除第0行和第col列）
		std::array<std::array<float, 3>, 3> minor{};
		int minor_row = 0;
		for (int i = 1; i < 4; ++i) {
			int minor_col = 0;
			for (int j = 0; j < 4; ++j) {
				if (j == col) continue;
				minor[minor_row][minor_col] = ((float*)this)[i * 4 + j];
				minor_col++;
			}
			minor_row++;
		}
		// 累加行列式值
		det += sign * ((float*)this)[col] * calculate3x3(minor);
		sign *= -1;
	}
	return det;
}

using Matrix3x3 = std::array<std::array<float, 3>, 3>;

// 生成排除指定行和列的3x3子矩阵
Matrix3x3 getMinor(const CMatrix& mat, int excluded_row, int excluded_col) {
	Matrix3x3 minor{};
	int minor_row = 0;
	for (int i = 0; i < 4; ++i) {
		if (i == excluded_row) continue;
		int minor_col = 0;
		for (int j = 0; j < 4; ++j) {
			if (j == excluded_col) continue;
			minor[minor_row][minor_col] = mat[i * 4 + j];
			++minor_col;
		}
		++minor_row;
	}
	return minor;
}

// 计算3x3矩阵的行列式
inline static float determinant3x3(const Matrix3x3& mat) {
	return mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
		- mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
		+ mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
}

CMatrix CMatrix::getInverse() const {
	const float det = this->det();

	// 计算余因子矩阵
	float cofactor_matrix[16]{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			const Matrix3x3 minor = getMinor(*this, i, j);
			const float sign = ((i + j) % 2 == 0) ? 1.0f : -1.0f;
			cofactor_matrix[i * 4 + j] = sign * determinant3x3(minor);
		}
	}

	// 转置余因子矩阵得到伴随矩阵
	float adjugate_matrix[16]{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			adjugate_matrix[j * 4 + i] = cofactor_matrix[i * 4 + j];
		}
	}

	// 计算逆矩阵：伴随矩阵 / 行列式
	const float inv_det = 1.0f / det;
	float inverse_matrix[16]{};
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			inverse_matrix[i * 4 + j] = adjugate_matrix[i * 4 + j] * inv_det;
		}
	}

	return CMatrix(inverse_matrix);
}

CMatrix CMatrix::rotateMatrix(float angle, const CVector& axis) {
	CVector normAxis = axis;
	normAxis.normalize();
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;
	float x = normAxis.x;
	float y = normAxis.y;
	float z = normAxis.z;
	float val[16] = {
		t * x * x + c, t * x * y + s * z, t * x * z - s * y, 0,
		t * x * y - s * z, t * y * y + c, t * y * z + s * x, 0,
		t * x * z + s * y, t * y * z - s * x, t * z * z + c, 0,
		0, 0, 0, 1
	};
	return CMatrix(val);
}

CMatrix CMatrix::rotateMatrix(float angle, const CVector& axis, const CVector& anchor) {
	return CMatrix::transMatrix(anchor) *
		CMatrix::rotateMatrix(angle, axis) *
		CMatrix::transMatrix(-anchor);
}

CMatrix CMatrix::rotateMatrix(const CVector& v1, const CVector& v2) {
	CVector a = v1, b = v2;
	a.normalize();
	b.normalize();

	if ((a - b).len() < 1e-4) return CMatrix(IDENTITY);
	if ((a + b).len() < 1e-4) { return CMatrix::rotateMatrix(PI, CVector(0, 1, 0)); }

	CVector n = a.crossMul(b).normalize();
	double cos_theta = a.dotMul(b);
	double theta = acos(cos_theta);

	CMatrix I = CMatrix(IDENTITY);
	float val[16] = {
		0,	n.z, -n.y, 0,
		-n.z, 0, n.x, 0,
		n.y, -n.x, 0, 0,
		0, 0, 0, 0
	};
	CMatrix K(val);

	return I + (float)sin(theta) * K + (1 - cos_theta) * K * K;
}

CMatrix CMatrix::rotateMatrix(const CVector& v1, const CVector& v2, const CVector& anchor) {
	return CMatrix::transMatrix(anchor) *
		CMatrix::rotateMatrix(v1, v2) *
		CMatrix::transMatrix(-anchor);
}

CMatrix CMatrix::transMatrix(const CVector& trans) {
	float val[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		trans.x, trans.y, trans.z, 1
	};
	return CMatrix(val);
}

CMatrix CMatrix::scaleMatrix(const CVector& scale) {
	float val[16] = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};
	return CMatrix(val);
}

CMatrix CMatrix::perspectiveMatrix(float fov, float aspect, float near, float far) {
	float f = 1.0f / tan(fov / 2);
	float val[16] = {
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / (near - far), -1,
		0, 0, (2 * far * near) / (near - far), 0
	};
	return CMatrix(val);
}

CMatrix CMatrix::lookAtMatrix(const CVector& eye, const CVector& center, const CVector& up) {
	CVector f = (center - eye).normalize();
	CVector s = f.crossMul(up).normalize();
	CVector u = s.crossMul(f);

	float tx = -s.dotMul(eye);
	float ty = -u.dotMul(eye);
	float tz = f.dotMul(eye);

	float m[16] = {
		s.x,  u.x,  -f.x,  0,
		s.y,  u.y,  -f.y,  0,
		s.z,  u.z,  -f.z,  0,
		tx,   ty,   tz,    1
	};

	return CMatrix(m);
}

CMatrix CMatrix::lookAtMatrix(const CVector& front, const CVector& up, const CVector& right, const CVector& pos) {
	float m[16] = {
		right.x, up.x, -front.x, 0,
		right.y, up.y, -front.y, 0,
		right.z, up.z, -front.z, 0,
		-right.dotMul(pos), -up.dotMul(pos), front.dotMul(pos), 1
	};
	return CMatrix(m);
}

CMatrix::operator const float* () const {
	return &m00;
}

CEuler CMatrix::toEuler() const {
	float p = asin(-m12);
	if (fabs(cos(p)) > 1.0E-5) {
		float h = atan2(m02, m22);
		float b = atan2(m10, m11);
		return CEuler(h * 180 / PI, p * 180 / PI, b * 180 / PI).normalize();
	}
	else {
		float h = atan2(-m20, m00);
		float b = 0;
		return CEuler(h * 180 / PI, p * 180 / PI, b).normalize();
	}
}

CQuaternion CMatrix::toQuaternion() const {
	float trace[4] = {
		m00 + m11 + m22,
		m00 - m11 - m22,
		m11 - m00 - m22,
		m22 - m00 - m11
	};
	
	int maxIndex;
	if (trace[0] >= trace[1] && trace[0] >= trace[2] && trace[0] >= trace[3]) {
		maxIndex = 0;
	}
	else if (trace[1] >= trace[0] && trace[1] >= trace[2] && trace[1] >= trace[3]) {
		maxIndex = 1;
	}
	else if (trace[2] >= trace[0] && trace[2] >= trace[1] && trace[2] >= trace[3]) {
		maxIndex = 2;
	}
	else {
		maxIndex = 3;
	}
	float w, x, y, z;
	switch (maxIndex)
	{
	case 0:
		w = sqrt(trace[maxIndex] + 1) / 2;
		x = (m21 - m12) / (4 * w);
		y = (m02 - m20) / (4 * w);
		z = (m10 - m01) / (4 * w);
		break;
	case 1:
		x = sqrt(trace[maxIndex] + 1) / 2;
		w = (m21 - m12) / (4 * x);
		y = (m01 + m10) / (4 * x);
		z = (m02 + m20) / (4 * x);
		break;
	case 2:
		y = sqrt(trace[maxIndex] + 1) / 2;
		w = (m02 - m20) / (4 * y);
		x = (m01 + m10) / (4 * y);
		z = (m12 + m21) / (4 * y);
		break;
	case 3:
		z = sqrt(trace[maxIndex] + 1) / 2;
		w = (m10 - m01) / (4 * z);
		x = (m02 + m20) / (4 * z);
		y = (m12 + m21) / (4 * z);
		break;
	default:
		break;
	}
	return { w, x, y, z };
}