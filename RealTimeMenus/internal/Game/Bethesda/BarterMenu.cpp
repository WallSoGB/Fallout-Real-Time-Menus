#include "BarterMenu.hpp"

BarterMenu* BarterMenu::GetSingleton() {
    return *reinterpret_cast<BarterMenu**>(0x11D8FA4);
}
