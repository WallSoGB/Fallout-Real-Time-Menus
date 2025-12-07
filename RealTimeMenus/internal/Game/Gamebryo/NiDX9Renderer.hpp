#pragma once

#include "NiRenderer.hpp"

NiSmartPointer(NiDX9Renderer);

class NiDX9Renderer : public NiRenderer {
public:
	NiDX9Renderer();
	virtual ~NiDX9Renderer();

	static NiDX9Renderer* GetSingleton();
};