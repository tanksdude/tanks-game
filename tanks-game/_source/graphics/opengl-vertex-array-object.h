#pragma once
#include "vertex-array-object.h"

class OpenGLVertexArrayObject final : public VertexArrayObject {
protected:
	std::vector<const VertexBuffer*> vertexBuffers;
	const IndexBuffer* indexBuffer;
	unsigned int vertexBufferIndex;

public:
	void AddVertexBuffer(const VertexBuffer* vb) override;
	void SetIndexBuffer(const IndexBuffer* ib) override;

	void Bind() const override;
	void Unbind() const override;

	const std::vector<const VertexBuffer*>& GetVertexBuffers() const override { return vertexBuffers; }
	const IndexBuffer* GetIndexBuffer() const override { return indexBuffer; }

	OpenGLVertexArrayObject();
	~OpenGLVertexArrayObject();
};
