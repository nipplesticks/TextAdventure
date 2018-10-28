#pragma once
#include <Windows.h>
#include "../Entities/Drawable.h"
#include "Camera.h"
class Render
{
private:
	Quad m_viewPort;
	Vec m_size;
	Drawable * m_renderTarget;
public:
	Render();
	void Init(Quad viewport);
	void Resize(Quad viewPort);
	void Flush(const Camera & cam, bool forceRedraw = false);
	void Clear();
private:
	void _cleanup();
	void _alloc();
	void _setTarget(const int & x, const int & y);
	void _cls();
};