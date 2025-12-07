#include "DialogMenu.hpp"

DialogMenu* DialogMenu::GetSingleton() {
	return *reinterpret_cast<DialogMenu**>(0x11D9510);
}
