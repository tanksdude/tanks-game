#pragma once
#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

class VertexArray {
private:
	unsigned int rendererID;

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual unsigned int getRendererID() const = 0;

	static VertexArray* MakeVertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
	virtual ~VertexArray() { return; }
};
