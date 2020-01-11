#pragma once

#include "vertexarray.h"
#include <GL/glew.h>

VertexArray::VertexArray() {
	glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSize(element.type);
	}
}

void VertexArray::Bind() const {
	glBindVertexArray(rendererID);
}

void VertexArray::Unbind() const {
	glBindVertexArray(0);
}