#pragma once
#include "ggl.h"
class FrameBufferObject
{
public:
	unsigned int FBO;
	unsigned int RBO;
	unsigned int ColorBuffer;
	unsigned int DepthBuffer;
public:
	void Init(int width, int height, bool b_clr, bool b_dep);
	void Active(int width, int height);
	void UnActive();
private:
	void InitColorBuffer(int width, int height);
	void InitDepthBuffer(int width, int height);
};

