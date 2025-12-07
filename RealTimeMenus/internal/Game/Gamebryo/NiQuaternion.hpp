#pragma once

#include "NiMemObject.hpp"

class NiMatrix3;

class NiQuaternion : public NiMemObject {
public:
	float m_fW;
	float m_fX;
	float m_fY;
	float m_fZ;

	NiQuaternion() : m_fW(0), m_fX(0), m_fY(0), m_fZ(0) {};
	NiQuaternion(float w, float x, float y, float z) : m_fW(w), m_fX(x), m_fY(y), m_fZ(z)  {};
	NiQuaternion(float afAngle, const NiPoint3& arAxis);

	NiQuaternion operator+(const NiQuaternion& rhs) const {
		return NiQuaternion(m_fW + rhs.m_fW, m_fX + rhs.m_fX, m_fY + rhs.m_fY, m_fZ + rhs.m_fZ);
	}

	NiQuaternion operator-(const NiQuaternion& rhs) const {
		return NiQuaternion(m_fW - rhs.m_fW, m_fX - rhs.m_fX, m_fY - rhs.m_fY, m_fZ - rhs.m_fZ);
	}

	NiQuaternion operator*(const float& rhs) const {
		return NiQuaternion(m_fW * rhs, m_fX * rhs, m_fY * rhs, m_fZ * rhs);
	}

	NiQuaternion operator/(const float& rhs) const {
		return NiQuaternion(m_fW / rhs, m_fX / rhs, m_fY / rhs, m_fZ / rhs);
	}

	NiQuaternion& operator+=(const NiQuaternion& rhs) {
		m_fW += rhs.m_fW;
		m_fX += rhs.m_fX;
		m_fY += rhs.m_fY;
		m_fZ += rhs.m_fZ;
		return *this;
	}

	NiQuaternion& operator-=(const NiQuaternion& rhs) {
		m_fW -= rhs.m_fW;
		m_fX -= rhs.m_fX;
		m_fY -= rhs.m_fY;
		m_fZ -= rhs.m_fZ;
		return *this;
	}

	NiQuaternion& operator*=(const float& rhs) {
		m_fW *= rhs;
		m_fX *= rhs;
		m_fY *= rhs;
		m_fZ *= rhs;
		return *this;
	}

	NiQuaternion& operator/=(const float& rhs) {
		m_fW /= rhs;
		m_fX /= rhs;
		m_fY /= rhs;
		m_fZ /= rhs;
		return *this;
	}

	bool operator==(const NiQuaternion& rhs) const {
		return m_fW == rhs.m_fW && m_fX == rhs.m_fX && m_fY == rhs.m_fY && m_fZ == rhs.m_fZ;
	}

	bool operator!=(const NiQuaternion& rhs) const {
		return m_fW != rhs.m_fW || m_fX != rhs.m_fX || m_fY != rhs.m_fY || m_fZ != rhs.m_fZ;
	}

	NiQuaternion& operator=(const NiQuaternion& rhs) {
		m_fW = rhs.m_fW;
		m_fX = rhs.m_fX;
		m_fY = rhs.m_fY;
		m_fZ = rhs.m_fZ;
		return *this;
	}

	NiQuaternion& operator=(const float& rhs) {
		m_fW = rhs;
		m_fX = rhs;
		m_fY = rhs;
		m_fZ = rhs;
		return *this;
	}
};