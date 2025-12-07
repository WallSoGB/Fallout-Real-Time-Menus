#include "ScreenSplatter.hpp"

// GAME - 0x5BAC20
bool ScreenSplatter::IsActiveSplatter() {
    return CdeclCall<bool>(0x5BAC20);
}
