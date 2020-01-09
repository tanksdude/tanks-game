#pragma once
#include "vertexbuffer.h"
#include <GL/glew.h>

#include "renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_rendererID);
}

void VertexBuffer::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

