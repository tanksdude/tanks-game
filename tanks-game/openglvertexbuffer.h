#pragma once
#include "vertexbuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
protected:
	unsigned int rendererID;
public:
	void modifyData(const void* data, int offset, unsigned int size) override;
	inline void modifyData(const void* data, unsigned int size) override { modifyData(data, 0, size); }

	void Bind() const override;
	void Unbind() const override;

	OpenGLVertexBuffer(const void* data, unsigned int size);
	OpenGLVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	~OpenGLVertexBuffer();
};
