#include "NiDX9Renderer.hpp"

NiDX9Renderer* NiDX9Renderer::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<NiDX9Renderer**>(0x11C73B4);
#else
	return *reinterpret_cast<NiDX9Renderer**>(0xED6C14);
#endif
}