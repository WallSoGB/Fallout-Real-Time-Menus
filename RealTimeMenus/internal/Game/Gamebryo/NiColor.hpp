#pragma once

#include "Bethesda/BSMemObject.hpp"
#include "NiPoint3.hpp"

class NiColor {
public:
	float r;
	float g;
	float b;

	inline float operator[] (uint32_t i) const { return ((float*)&r)[i]; };

	operator float* () const { return (float*)this; };

	NiColor() : r(0.f), g(0.f), b(0.f) {};
	NiColor(float r, float g, float b) : r(r), g(g), b(b) {};
	NiColor(float r, float g, float b, float a) : r(r), g(g), b(b) {};
	NiColor(float f) : r(f), g(f), b(f) {};

	NiColor operator*(const float f) const {
		return NiColor(r * f, g * f, b * f);
	};

	NiColor operator*(const NiColor& color) const {
		return NiColor(r * color.r, g * color.g, b * color.b);
	};

	NiColor operator/(const float f) const {
		return NiColor(r / f, g / f, b / f);
	};

	NiColor operator/(const NiColor& color) const {
		return NiColor(r / color.r, g / color.g, b / color.b);
	};

	bool operator==(const NiColor& color) const {
		return r == color.r && g == color.g && b == color.b;
	};

	bool operator==(const float f) const {
		return r == f && g == f && b == f;
	};

	bool operator!=(const NiColor& color) const {
		return r != color.r || g != color.g || b != color.b;
	};

	bool operator!=(const float f) const {
		return r != f && g != f && b != f;
	};

	bool operator>(const float f) const {
		return r > f && g > f && b > f;
	}

	bool operator<(const float f) const {
		return r < f && g < f && b < f;
	}

	NiColor operator*=(const float f) {
		r *= f;
		g *= f;
		b *= f;
		return *this;
	};

	NiColor operator=(const NiPoint3& color) {
		r = color.x;
		g = color.y;
		b = color.z;
		return *this;
	};

	operator bool() const {
		return r != 0.0f || g != 0.0f || b != 0.0f;
	};
};

ASSERT_SIZE(NiColor, 0xC);