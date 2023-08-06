#pragma once
#include <string>
#include <unordered_map>
#include "rendering-context.h"
#include "vertex-array.h"
#include "vertex-buffer.h"
#include "index-buffer.h"
#include "shader.h"
#include "drawable-thing.h"
#include "color-value-holder.h"
#include <glm.hpp>
#include <GL/glew.h>

enum class AvailableRenderingContexts {
	OpenGL,
	software,
	null_rendering
};

enum class RenderingDrawTypes {
	triangles, //main
	line_loop,
	line_strip,
	lines,
	points //unused
};

class Renderer {
	friend class DeveloperManager;

public:
	//public until I rewrite rendering
	static glm::mat4 proj;
	static glm::mat4 getProj();

	static float current_cameraX, current_cameraY, current_cameraZ, current_targetX, current_targetY, current_targetZ; //view matrix
	static bool viewMatBound;
	static bool projectionMatBound;

	static int window_width;
	static int window_height;
	static int gamewindow_width; //width of game inside window
	static int gamewindow_height;
	static void windowResizeFunc(int w, int h);

	static AvailableRenderingContexts renderingMethodType;
	static RenderingContext* renderingMethod;

	static void Draw(RenderingDrawTypes, VertexArray*, IndexBuffer*, glm::mat4 matrix, ColorValueHolder) { return; } //main
	static void Draw(RenderingDrawTypes, VertexArray*, int count, glm::mat4 matrix, ColorValueHolder) { return; } //cooldowns and stuff
	static void Draw(RenderingDrawTypes, VertexArray*, IndexBuffer*, int count, glm::mat4 matrix, ColorValueHolder) { return; } //cooldowns and stuff

private:
	static std::unordered_map<std::string, Shader*> shaderCache;
	static unsigned int currentShader;
	static unsigned int currentVertexArray;
	static unsigned int currentIndexBuffer;
	static Shader* boundShader;
	static inline void bindShader(Shader*);
	static inline void bindShader(const Shader&);
	static inline void bindVertexArray(const VertexArray&);
	static inline void bindIndexBuffer(const IndexBuffer&);

	static std::string getErrorString(GLenum err);

public:
	static Shader* getShader(std::string);
	static void BeginningStuff();
	static void Clear();
	static void Clear(int bits);
	static void Flush();
	static void SetContext(AvailableRenderingContexts);
	static void SetContext(std::string);
	static AvailableRenderingContexts GetContext() { return renderingMethodType; }
	static void PreInitialize(int* argc, char** argv, std::string windowName); //initialize freeglut and GLEW
	static void PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY);
	static void PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, int sizeX, int sizeY);
	static void Initialize();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, unsigned int count);
	static void Draw(const VertexArray& va, const Shader& shader, GLenum type, GLint first, GLsizei count);
	static void Draw(GLenum type, GLint first, GLsizei count);
	static glm::mat4 GenerateModelMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY);
	static void SetViewMatrix(float cameraX, float cameraY, float cameraZ, float targetX, float targetY, float targetZ);
	//static void SetProjectionMatrix(float left, float right, float bottom, float top/*, float near, float far*/); //orthographic
	static void SetProjectionMatrix();
	static void Unbind(const VertexArray&);
	static void Unbind(const IndexBuffer&);
	static void Unbind(const Shader&);
	static void UnbindAll();
	static void Cleanup();

	static void printGLError();

private:
	Renderer() {}
	Renderer(const Renderer&) {}
};
