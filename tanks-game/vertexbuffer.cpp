#pragma once

#include "vertexbuffer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : VertexBuffer(data, size, GL_DYNAMIC_DRAW){
	//nothing
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum hint) {
	glGenBuffers(1, &rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, hint);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::modifyData(const void* data, int offset, unsigned int size) {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
