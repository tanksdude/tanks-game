#pragma once
#include "vertex-array.h"

class OpenGLVertexArray : public VertexArray {
protected:
	unsigned int rendererID;
	std::vector<const VertexBuffer*> vertexBuffers;
	const IndexBuffer* indexBuffer;

public:
	OpenGLVertexArray();
	~OpenGLVertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const VertexBuffer* vb) override;
	void SetIndexBuffer(const IndexBuffer* ib) override;

	const std::vector<const VertexBuffer*>& GetVertexBuffers() const override { return vertexBuffers; }
	const IndexBuffer* GetIndexBuffer() const override { return indexBuffer; }

	unsigned int getRendererID() const { return rendererID; }
};
