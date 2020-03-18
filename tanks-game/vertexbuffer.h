#pragma once

#include <GL/glew.h>

class VertexBuffer {
private:
	unsigned int rendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, GLenum hint);
	~VertexBuffer();

	void modifyData(const void* data, int offset, unsigned int size);
	inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	void Bind() const;
	void Unbind() const;
};