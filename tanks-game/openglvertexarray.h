#pragma once
#include "vertexarray.h"

class OpenGLVertexArray : public VertexArray {
protected:
	unsigned int rendererID;
public:
	OpenGLVertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
	~OpenGLVertexArray();

	void Bind() const;
	void Unbind() const;

	unsigned int getRendererID() const { return rendererID; }
};
