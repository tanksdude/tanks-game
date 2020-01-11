#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int getSize(unsigned int type) {
		switch (type) {
			case GL_FLOAT: return sizeof(float);
			case GL_UNSIGNED_INT: return sizeof(unsigned int);
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char);
		}
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout() {
		stride = 0;
	}

	void Push_f(unsigned int count) { //float
		elements.push_back({ GL_FLOAT, count, GL_FALSE });
		stride += count * sizeof(float);
	}

	void Push_ui(unsigned int count) { //unsigned int
		elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		stride += count * sizeof(unsigned int);
	}

	void Push_uc(unsigned int count) { //unsigned char (byte)
		elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		stride += count * sizeof(unsigned char);
	}

	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};