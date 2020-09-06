#pragma once
#include "vertexbuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
protected:
	unsigned int rendererID;
public:
	void modifyData(const void* data, int offset, unsigned int size);
	inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	void Bind() const;
	void Unbind() const;

	OpenGLVertexBuffer(const void* data, unsigned int size);
	OpenGLVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	~OpenGLVertexBuffer();
};
