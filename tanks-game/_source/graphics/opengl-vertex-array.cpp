#include "opengl-vertex-array.h"

//#include <stdexcept>

#include <GL/glew.h>
#include "opengl-vertex-buffer.h"
#include "opengl-index-buffer.h"

OpenGLVertexArray::OpenGLVertexArray() {
	glGenVertexArrays(1, &rendererID);
	indexBuffer = nullptr;
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(1, &rendererID);
}

void OpenGLVertexArray::Bind() const {
	glBindVertexArray(rendererID);
}

void OpenGLVertexArray::Unbind() const {
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const VertexBuffer* vb) {
	glBindVertexArray(rendererID);
	vb->Bind();
	vertexBuffers.push_back(vb);

	const VertexBufferLayout& layout = vb->GetLayout();
	for (int i = 0; i < layout.getElements().size(); i++) {
		const VertexBufferElement& element = layout.getElements()[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,
			element.getComponentCount(element.type),
			OpenGLVertexBuffer::ShaderDataTypeToGLenum(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.getStride(),
			(const void*) element.offset);
	}
}

void OpenGLVertexArray::SetIndexBuffer(const IndexBuffer* ib) {
	glBindVertexArray(rendererID);
	ib->Bind();
	if (indexBuffer != nullptr) [[likely]] {
		delete indexBuffer;
	}
	indexBuffer = ib;
}
