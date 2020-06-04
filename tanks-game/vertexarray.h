#pragma once
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

class VertexArray {
private:
	unsigned int rendererID;
public:
	VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	unsigned int getRendererID() const { return rendererID; }
};
