#pragma once
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <glm.hpp>

#include "graphics/rendering-context.h"
#include "graphics/vertex-array.h"
#include "graphics/vertex-buffer.h"
#include "graphics/index-buffer.h"
#include "graphics/shader.h"

//#include <thread>
//#include <atomic>
//#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class AvailableRenderingContexts {
	OpenGL,
	software,
	null_rendering
};

/*
enum class RenderingDrawTypes {
	triangles, //main
	line_loop,
	line_strip,
	lines,
	points, //unused
};
*/

class Renderer {
	friend class DeveloperManager;

public:
	static GLFWwindow* glfw_window;
	static void windowResizeFunc(GLFWwindow* window, int w, int h);
	static void windowCoordsToGameCoords(double inputX, double inputY, int& actualX, int& actualY);
	static GLFWmonitor* get_current_monitor(GLFWwindow* window);

	//static void thread_func();
	//static std::mutex drawingDataLock;
	//static std::atomic_bool thread_workExists;

private:
	static glm::mat4 proj;
	static glm::mat4 getProj();

	static float current_cameraX, current_cameraY, current_cameraZ, current_targetX, current_targetY, current_targetZ; //view matrix
	static bool viewMatBound;
	static bool projectionMatBound;

	static int window_width;
	static int window_height;
	static int gamewindow_width; //width of game inside window
	static int gamewindow_height;

	static bool currently_fullscreen;
	static int old_window_width; //for GLFW toggling fullscreen
	static int old_window_height;
	static int old_window_xpos;
	static int old_window_ypos;

	static AvailableRenderingContexts renderingMethodType;
	static RenderingContext* renderingMethod;

	//static std::thread graphicsThread;
	//static std::atomic_bool thread_keepRunning;

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
	static Shader* getShader(std::string);

	static void Unbind(const VertexArray&);
	static void Unbind(const IndexBuffer&);
	static void Unbind(const Shader&);
	static void UnbindAll();
	static void Cleanup();

	static std::string getErrorString(GLenum err);

private:
	static std::unordered_map<std::string, std::vector<std::pair<std::vector<float>, std::vector<unsigned int>>>> sceneData; //<vertices, indices>
	static std::vector<std::string> sceneList;
	static std::string currentSceneName;
	static std::vector<float>* currentVerticesData;
	static std::vector<unsigned int>* currentIndicesData;
	static const int maxVerticesDataLength;
	static const int maxIndicesDataLength;
	static inline bool enoughRoomForMoreVertices(int pushLength);
	static inline bool enoughRoomForMoreIndices(int pushLength);
	static inline void pushAnotherDataList();
	static void ActuallyFlush();
	static void BatchedFlush(std::vector<float>& vertices, std::vector<unsigned int>& indices);

	static VertexArray* batched_va;
	static VertexBuffer* batched_vb;
	static IndexBuffer* batched_ib;
	static bool initialized_GPU;

	static bool initializeGPU();
	static bool uninitializeGPU();

public:
	static void SetContext(AvailableRenderingContexts);
	static void SetContext(std::string);
	static AvailableRenderingContexts GetContext() { return renderingMethodType; }

	static void PreInitialize(int* argc, char** argv, std::string windowName); //initialize GLFW and GLEW
	static void PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY);
	static void PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, double sizeMultiplier);
	static void SetWindowIcon(const char* image_path); //called during PreInitialize; 32x32 seems to be the size OSes like the most
	static void Initialize();
	static void Uninitialize();

	static void BeginningStuff();
	static void Clear();
	static void BeginScene(std::string name); //name of scene, to push timing to diagnostics
	static void SubmitBatchedDraw(const float* posAndColor, int posAndColorLength, const unsigned int* indices, int indicesLength);
	static void EndScene();
	static void Flush();
	static bool isDebugDrawingEnabled(std::string name); //should this really be here?

	static glm::mat4 GenerateModelMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY);
	static void SetViewMatrix(float cameraX, float cameraY, float cameraZ, float targetX, float targetY, float targetZ);
	//static void SetProjectionMatrix(float left, float right, float bottom, float top/*, float near, float far*/); //orthographic
	static void SetProjectionMatrix(); //should this be public?

	static void printGLError();

private:
	Renderer() = delete;
	Renderer(const Renderer&) = delete;
};
