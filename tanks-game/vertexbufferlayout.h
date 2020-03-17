#pragma once

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
	VertexBufferElement elements;
	unsigned int stride;

public:
	VertexBufferLayout() {
		stride = 0;
	}

	VertexBufferLayout(unsigned int floatCount) {
		elements = { GL_FLOAT, floatCount, GL_FALSE };
		stride = floatCount * sizeof(float);
	}

	void Push_2f() { //2D floats
		elements = { GL_FLOAT, 2, GL_FALSE };
		stride = 2 * sizeof(float);
	}

	void Push_3f() { //3D floats
		elements = { GL_FLOAT, 3, GL_FALSE };
		stride = 3 * sizeof(float);
	}

	inline const VertexBufferElement& getElement() const { return elements; }
	inline unsigned int getStride() const { return stride; }
};