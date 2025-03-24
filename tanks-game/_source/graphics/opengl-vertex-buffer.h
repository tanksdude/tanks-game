#pragma once
#include "vertex-buffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
protected:
	unsigned int rendererID;
	VertexBufferLayout layout;

public:
	void modifyData(const void* data, int offset, unsigned int size) override;

	void Bind() const override;
	void Unbind() const override;

	const VertexBufferLayout& GetLayout() const override { return layout; }
	void SetLayout(const VertexBufferLayout& l) override { layout = l; }

	static GLenum ShaderDataTypeToGLenum(ShaderDataType);

	OpenGLVertexBuffer(const void* data, unsigned int size);
	OpenGLVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	~OpenGLVertexBuffer();
};
