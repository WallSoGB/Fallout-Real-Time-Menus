#include "Calendar.hpp"

Calendar* Calendar::GetSingleton() {
    return reinterpret_cast<Calendar*>(0x11DE7B8);
}

// GAME - 0x867DA0
float Calendar::GetHour() const {
    return ThisCall<float>(0x867DA0, this);
}

// GAME - 0x867970
BSString Calendar::GetDateString() const {
    BSString strDate;
    ThisCall(0x867970, this, &strDate);
	return strDate;
}