#pragma once
#include "vertex-array-object.h"

class OpenGLVertexArrayObject : public VertexArrayObject {
protected:
	std::vector<const VertexBuffer*> vertexBuffers;
	const IndexBuffer* indexBuffer;
	unsigned int vertexBufferIndex;

public:
	OpenGLVertexArrayObject();
	~OpenGLVertexArrayObject();

	void Bind() const override;
	void Unbind() const override;

	void AddVertexBuffer(const VertexBuffer* vb) override;
	void SetIndexBuffer(const IndexBuffer* ib) override;

	const std::vector<const VertexBuffer*>& GetVertexBuffers() const override { return vertexBuffers; }
	const IndexBuffer* GetIndexBuffer() const override { return indexBuffer; }
};
