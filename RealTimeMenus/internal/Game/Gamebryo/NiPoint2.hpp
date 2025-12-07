#pragma once

#include "NiPoint3.hpp"

class NiPoint2 {
public:
	float x;
	float y;

	NiPoint2() : x(0.0f), y(0.0f) {};
	NiPoint2(float x, float y) : x(x), y(y) {};
	NiPoint2(float xy) : x(xy), y(xy) {};
	NiPoint2(const NiPoint3& point) : x(point.x), y(point.y) {};
	NiPoint2(const NiPoint3* point) : x(point->x), y(point->y) {};

	auto operator<=>(const NiPoint2&) const = default;

	const float& operator[] (int32_t i) const { return reinterpret_cast<const float*>(this)[i]; };
	float& operator[] (int32_t i) { return reinterpret_cast<float*>(this)[i]; };

	operator float* () const { return (float*)this; };

	// Point operations
	NiPoint2 operator+(const NiPoint2& arPoint) const { return NiPoint2(x + arPoint.x, y + arPoint.y); }

	NiPoint2 operator-(const NiPoint2& arPoint) const { return NiPoint2(x - arPoint.x, y - arPoint.y); }

	NiPoint2 operator*(const NiPoint2& arPoint) const { return NiPoint2(x * arPoint.x, y * arPoint.y); }

	NiPoint2 operator/(const NiPoint2& arPoint) const { return NiPoint2(x / arPoint.x, y / arPoint.y); }

	NiPoint2 operator-() const { return NiPoint2(-x, -y); }

	NiPoint2& operator+=(const NiPoint2& arPoint) {
		x += arPoint.x;
		y += arPoint.y;
		return *this;
	}

	NiPoint2& operator-=(const NiPoint2& arPoint) {
		x -= arPoint.x;
		y -= arPoint.y;
		return *this;
	}

	NiPoint2& operator*=(const NiPoint2& arPoint) {
		x *= arPoint.x;
		y *= arPoint.y;
		return *this;
	}

	NiPoint2& operator/=(const NiPoint2& arPoint) {
		x /= arPoint.x;
		y /= arPoint.y;
		return *this;
	}

	NiPoint2 operator+(float afScalar) const { return NiPoint2(x + afScalar, y + afScalar); }

	NiPoint2 operator-(float afScalar) const { return NiPoint2(x - afScalar, y - afScalar); }

	NiPoint2 operator*(float afScalar) const { return NiPoint2(x * afScalar, y * afScalar); }

	NiPoint2 operator/(float afScalar) const { return NiPoint2(x / afScalar, y / afScalar); }

	NiPoint2& operator+=(float afScalar) {
		x += afScalar;
		y += afScalar;
		return *this;
	}

	NiPoint2& operator-=(float afScalar) {
		x -= afScalar;
		y -= afScalar;
		return *this;
	}

	NiPoint2& operator*=(float afScalar) {
		x *= afScalar;
		y *= afScalar;
		return *this;
	}

	NiPoint2& operator/=(float afScalar) {
		x /= afScalar;
		y /= afScalar;
		return *this;
	}

	float Length() const {
		return sqrt(x * x + y * y);
	}

	float SqrLength() const {
		return x * x + y * y;
	}

	float Unitize() {
		float fLength = Length();

		if (fLength > 1e-06f) {
			float fRecip = 1.0f / fLength;
			x *= fRecip;
			y *= fRecip;
		}
		else
		{
			x = 0.0f;
			y = 0.0f;
			fLength = 0.0f;
		}
		return fLength;
	}
};