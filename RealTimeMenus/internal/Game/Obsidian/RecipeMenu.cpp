#include "RecipeMenu.hpp"

RecipeMenu* RecipeMenu::GetSingleton() {
	return *reinterpret_cast<RecipeMenu**>(0x11D8E90);
}
