#include "Tile.hpp"

// GAME - 0xA01350
void Tile::SetValue(uint32_t auiTrait, const char* apString, bool abPropagate) {
	ThisCall(0xA01350, this, auiTrait, apString, abPropagate);
}

// GAME - 0xA012D0
void Tile::SetValue(uint32_t auiTrait, float afValue, bool abPropagate) {
	ThisCall(0xA012D0, this, auiTrait, afValue, abPropagate);
}

// GAME - 0xA03DA0
Tile* Tile::GetChildByName(const char* apName) const {
	return ThisCall<Tile*>(0xA03DA0, this, apName);
}
