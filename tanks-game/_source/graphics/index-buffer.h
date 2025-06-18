#pragma once

class IndexBuffer {
public:
	//same as VertexBuffer
	enum class RenderingHints {
		stream_draw, //"The data store contents will be modified once and used at most a few times."
		static_draw, //"The data store contents will be modified once and used many times."
		dynamic_draw //"The data store contents will be modified repeatedly and used many times."
	};

protected:
	unsigned int rendererID;
	unsigned int count;

public:
	virtual void modifyData(const void* data, int offset, unsigned int size) = 0;
	inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	unsigned int getRendererID() const { return rendererID; }
	unsigned int getCount() const { return count; }

	static IndexBuffer* MakeIndexBuffer(const unsigned int* data, unsigned int count, RenderingHints hint);
	static inline IndexBuffer* MakeIndexBuffer(const unsigned int* data, unsigned int count) {
		return MakeIndexBuffer(data, count, RenderingHints::stream_draw);
	}
	virtual ~IndexBuffer() { return; }
};
