#pragma once
#include "ggl.h"
class FrameBufferObject 
{
public:
	GLuint mFrameBufferObject;
	GLint mPrevFrameBuffer;
	std::map<std::string, GLuint> mBuffers;
	std::vector<GLenum> mDrawBuffers;
public:
	FrameBufferObject();
	void AttachColorBuffer(const char* bufferName, GLenum attachment, int width, int height, int texIndex = 0);
	void AttachDepthBuffer(const char* bufferName, int width, int height, int texIndex = 0);
	void Finish();
	void Bind();
	void Unbind();
	GLuint GetBuffer(const char* bufferName);
};