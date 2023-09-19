#pragma once
#include "vertex-buffer.h"
#include "index-buffer.h"

class VertexArray {
private:
	unsigned int rendererID;

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual unsigned int getRendererID() const = 0;

	virtual void AddVertexBuffer(const VertexBuffer* vb) = 0;
	virtual void SetIndexBuffer(const IndexBuffer* ib) = 0;

	virtual const std::vector<const VertexBuffer*>& GetVertexBuffers() const = 0;
	virtual const IndexBuffer* GetIndexBuffer() const = 0;

	static VertexArray* MakeVertexArray();
	virtual ~VertexArray() { return; }
};
