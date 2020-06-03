#pragma once

#include "vertexarray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	glGenVertexArrays(1, &rendererID);

	Bind();
	vb.Bind();
	
	const auto& element = layout.getElement();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, element.count, element.type, element.normalized, layout.getStride(), (const void*)0);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::Bind() const {
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}
