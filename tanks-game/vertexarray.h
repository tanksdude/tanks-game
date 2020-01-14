#pragma once

#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

class VertexArray {
private:
	unsigned int rendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	unsigned int getRendererID() const { return rendererID; }
};