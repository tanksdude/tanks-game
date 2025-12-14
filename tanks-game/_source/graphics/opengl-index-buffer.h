#pragma once
#include "index-buffer.h"

class OpenGLIndexBuffer final : public IndexBuffer {
public:
	void modifyData(const void* data, int offset, unsigned int size) override;

	void Bind() const override;
	void Unbind() const override;

	OpenGLIndexBuffer(const unsigned int* data, unsigned int count, RenderingHints hint);
	~OpenGLIndexBuffer();
};
