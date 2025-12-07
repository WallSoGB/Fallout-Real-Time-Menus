#include "VATS.hpp"

VATS* VATS::GetSingleton() {
	return reinterpret_cast<VATS*>(0x11F2250);
}
