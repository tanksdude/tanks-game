#pragma once
#include "vertex-buffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
public:
	void modifyData(const void* data, int offset, unsigned int size) override;

	void Bind() const override;
	void Unbind() const override;

	OpenGLVertexBuffer(const void* data, unsigned int size);
	OpenGLVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	~OpenGLVertexBuffer();

	static GLenum ShaderDataTypeToGLenum(ShaderDataType);
};
