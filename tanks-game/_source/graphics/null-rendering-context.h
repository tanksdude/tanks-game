#pragma once
#include "rendering-context.h"

//draws nothing
class NullRenderingContext : public RenderingContext {
	void Initialize() override {}
	void Clear() const override {}
	void Flush() const override {}
	void PrintRendererInfo() const override {}
	void PrintRenderingError() const override {}

	void DrawWithoutIndices(int vertexCount) const override {}
	void DrawUsingIndices(int indexCount) const override {}
	void DrawInstanced(int indexCount, int drawCount) const override {}
};
