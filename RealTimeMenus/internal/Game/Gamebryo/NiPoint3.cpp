#include "NiPoint3.hpp"

NiPoint3 const NiPoint3::ZERO = NiPoint3(0, 0, 0);

// GAME - 0x457990
// GECK - 0x40B3D0
float NiPoint3::Length() const {
	return std::sqrt(x * x + y * y + z * z);
}

// GAME - 0x4B6190
float NiPoint3::Dot(const NiPoint3& arOther) const {
	return x * arOther.x + y * arOther.y + z * arOther.z;
}

float NiPoint3::Distance(const NiPoint3& arOther) const {
	return (*this - arOther).Length();
}