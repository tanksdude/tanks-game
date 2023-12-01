#pragma once
#include "index-buffer.h"

class OpenGLIndexBuffer : public IndexBuffer {
protected:
	unsigned int rendererID;
	unsigned int count;
public:
	void modifyData(const void* data, int offset, unsigned int size) override;

	void Bind() const override;
	void Unbind() const override;

	unsigned int getCount() const override { return count; }
	unsigned int getRendererID() const override { return rendererID; }

	OpenGLIndexBuffer(const unsigned int* data, unsigned int count);
	~OpenGLIndexBuffer();
};
