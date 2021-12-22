#include "opengl-vertex-array.h"
#include <GL/glew.h>
#include <stdexcept>

OpenGLVertexArray::OpenGLVertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	glGenVertexArrays(1, &rendererID);

	Bind();
	vb.Bind();

	const auto& element = layout.getElement();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, element.count, element.type, element.normalized, layout.getStride(), (const void*)0);
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
