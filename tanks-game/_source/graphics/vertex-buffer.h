#pragma once
#include "vertex-buffer-layout.h"

class VertexBuffer {
public:
	//same as IndexBuffer
	enum class RenderingHints {
		stream_draw, //"The data store contents will be modified once and used at most a few times."
		static_draw, //"The data store contents will be modified once and used many times."
		dynamic_draw //"The data store contents will be modified repeatedly and used many times."
	};

protected:
	unsigned int rendererID;
	VertexBufferLayout layout;

public:
	virtual void modifyData(const void* data, int offset, unsigned int size) = 0;
	inline void modifyData(const void* data, unsigned int size) { modifyData(data, 0, size); }

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	unsigned int getRendererID() const { return rendererID; }
	const VertexBufferLayout& GetLayout() const { return layout; }
	void SetLayout(const VertexBufferLayout& l) { layout = l; }

	static VertexBuffer* MakeVertexBuffer(const void* data, unsigned int size, RenderingHints hint);
	static inline VertexBuffer* MakeVertexBuffer(const void* data, unsigned int size) {
		return MakeVertexBuffer(data, size, RenderingHints::stream_draw);
	}
	virtual ~VertexBuffer() { return; }
};
