#pragma once

class IndexBuffer {
private:
	unsigned int m_rendererID;
	unsigned int m_count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();

	inline unsigned int getCount() { return m_count; }
};