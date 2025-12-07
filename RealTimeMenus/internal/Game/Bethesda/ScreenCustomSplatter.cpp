#include "ScreenCustomSplatter.hpp"

// GAME - 0x871260
bool ScreenCustomSplatter::IsActiveSplatter() {
    return CdeclCall<bool>(0x871260);
}
