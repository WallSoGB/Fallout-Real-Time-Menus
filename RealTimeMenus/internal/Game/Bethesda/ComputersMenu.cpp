#include "ComputersMenu.hpp"

ComputersMenu* ComputersMenu::GetSingleton() {
    return *reinterpret_cast<ComputersMenu**>(0x11D9334);
}
