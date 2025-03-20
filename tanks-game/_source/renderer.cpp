#include "renderer.h"

#include "constants.h"
#include <algorithm> //std::fill
#include <iostream>
#include <gtc/matrix_transform.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "graphics/opengl-rendering-context.h"
#include "graphics/software-rendering-context.h"
#include "graphics/null-rendering-context.h"

#include "diagnostics.h"
#include "keypress-manager.h"
#include "game-manager.h" //for isDebugDrawingEnabled()
#include "game-scene-manager.h" //to refresh the graphics when the window size changes

//std::mutex Renderer::drawingDataLock;
//std::thread Renderer::graphicsThread;
//std::atomic_bool Renderer::thread_keepRunning;
//std::atomic_bool Renderer::thread_workExists;

GLFWwindow* Renderer::glfw_window = nullptr;
bool Renderer::currently_fullscreen = false;
int Renderer::old_window_width;
int Renderer::old_window_height;
int Renderer::old_window_xpos;
int Renderer::old_window_ypos;

glm::mat4 Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
glm::mat4 Renderer::getProj() { return proj; }
int Renderer::window_width;
int Renderer::window_height;
int Renderer::gamewindow_width;
int Renderer::gamewindow_height;
RenderingContext* Renderer::renderingMethod = nullptr;
AvailableRenderingContexts Renderer::renderingMethodType;

float Renderer::current_cameraX = 0, Renderer::current_cameraY = 0, Renderer::current_cameraZ = -1;
float Renderer::current_targetX = 0, Renderer::current_targetY = 0, Renderer::current_targetZ = 0;
bool Renderer::viewMatBound = false, Renderer::projectionMatBound = false;
Shader* Renderer::boundShader = nullptr;

VertexArray* Renderer::batched_va;
VertexBuffer* Renderer::batched_vb;
IndexBuffer* Renderer::batched_ib;
bool Renderer::initialized_GPU = false;

std::unordered_map<std::string, std::vector<std::pair<std::vector<float>, std::vector<unsigned int>>>> Renderer::sceneData;
std::vector<std::string> Renderer::sceneList;
std::string Renderer::currentSceneName = "";
const int Renderer::maxVerticesDataLength = (1 << 24) / sizeof(float);
const int Renderer::maxIndicesDataLength = (1 << 24) / sizeof(unsigned int); //TODO: size (fills up faster)

void Renderer::windowResizeFunc(GLFWwindow*, int w, int h) {
	//note: this function also gets called when moving the window between monitors (when the scale is different)
	Renderer::window_width = w;
	Renderer::window_height = h;

	// Define x-axis and y-axis range
	const float appXmin = 0.0;
	const float appXmax = GAME_WIDTH;
	const float appYmin = 0.0;
	const float appYmax = GAME_HEIGHT;

	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);

	if ((appXmax - appXmin) / w < (appYmax - appYmin) / h) { //too wide
		float scale = ((appYmax - appYmin) / h) / ((appXmax - appXmin) / w);
		Renderer::proj = glm::ortho(float(GAME_WIDTH/2) - float(GAME_WIDTH/2)*scale, float(GAME_WIDTH/2) + float(GAME_WIDTH/2)*scale, 0.0f, (float)GAME_HEIGHT);
		Renderer::gamewindow_width = Renderer::window_height * (GAME_WIDTH/GAME_HEIGHT);
		Renderer::gamewindow_height = Renderer::window_height;
	} else { //too tall
		float scale = ((appXmax - appXmin) / w) / ((appYmax - appYmin) / h);
		Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, float(GAME_HEIGHT/2) - float(GAME_HEIGHT/2)*scale, float(GAME_HEIGHT/2) + float(GAME_HEIGHT/2)*scale);
		Renderer::gamewindow_width = Renderer::window_width;
		Renderer::gamewindow_height = Renderer::window_width * (GAME_HEIGHT/GAME_WIDTH);
	}

	Diagnostics::pushGraphTime("tick", 0); //HACK: since tick will never happen when the window is being resized, add it here; will not double-add when the game is over because that still does a tick
	GameSceneManager::DrawScenes_WindowResize();
}

void Renderer::windowCoordsToGameCoords(double inputX, double inputY, int& actualX, int& actualY) {
	//basically subtract out the black bars (just left/top), convert to a percent of the window, scale to game size
	//when the window is too wide, Renderer::window_height = Renderer::gamewindow_height; same when too tall for width
	//therefore this can handle both cases
	actualX =      (inputX - (Renderer::window_width  - Renderer::gamewindow_width)/2)  / Renderer::gamewindow_width   * GAME_WIDTH;
	actualY = (1 - (inputY - (Renderer::window_height - Renderer::gamewindow_height)/2) / Renderer::gamewindow_height) * GAME_HEIGHT;
}

//from: https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw#31526753
GLFWmonitor* Renderer::get_current_monitor(GLFWwindow* window) {
	int nmonitors, i;
	int wx, wy, ww, wh;
	int mx, my, mw, mh;
	int overlap, bestoverlap;
	GLFWmonitor* bestmonitor;
	GLFWmonitor** monitors;
	const GLFWvidmode* mode;

	bestoverlap = 0;
	bestmonitor = NULL;

	glfwGetWindowPos(window, &wx, &wy);
	glfwGetWindowSize(window, &ww, &wh);
	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++) {
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		overlap =
			std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
			std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

		if (bestoverlap < overlap) {
			bestoverlap = overlap;
			bestmonitor = monitors[i];
		}
	}

	return bestmonitor;
}

//actual renderer code:

std::unordered_map<std::string, Shader*> Renderer::shaderCache;
unsigned int Renderer::currentShader = -1;
unsigned int Renderer::currentVertexArray = -1;
unsigned int Renderer::currentIndexBuffer = -1;

void Renderer::BeginningStuff() {
	//while (thread_workExists.load()) {
	//	//spin
	//	std::cout << "thread1 waiting\n";
	//}
	//drawingDataLock.lock();

	if (KeypressManager::getKeyState("F11")) [[unlikely]] {
		if (currently_fullscreen) {
			//it's possible for the number of monitors to change, so these positions could be wrong, but whatever

			glfwSetWindowAttrib(Renderer::glfw_window, GLFW_DECORATED, GLFW_TRUE);
			glfwSetWindowPos(Renderer::glfw_window, Renderer::old_window_xpos, Renderer::old_window_ypos);
			glfwSetWindowSize(Renderer::glfw_window, Renderer::old_window_width, Renderer::old_window_height);

			currently_fullscreen = false;
		} else {
			GLFWmonitor* monitor = get_current_monitor(Renderer::glfw_window);
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			//doesn't need glfwGetMonitorContentScale to work properly

			int monitorX, monitorY;
			glfwGetMonitorPos(monitor, &monitorX, &monitorY);
			glfwGetWindowPos(Renderer::glfw_window, &Renderer::old_window_xpos, &Renderer::old_window_ypos);
			glfwGetWindowSize(Renderer::glfw_window, &Renderer::old_window_width, &Renderer::old_window_height);

			glfwSetWindowAttrib(Renderer::glfw_window, GLFW_DECORATED, GLFW_FALSE); //this is not stated to be required for proper fullscreen
			glfwSetWindowPos(Renderer::glfw_window, monitorX, monitorY);
			glfwSetWindowSize(Renderer::glfw_window, mode->width, mode->height);

			currently_fullscreen = true;
		}

		KeypressManager::unsetKeyState("F11");
	}
}

void Renderer::SetContext(AvailableRenderingContexts API) {
	if (renderingMethod != nullptr) {
		throw std::logic_error("ERROR: Cannot change rendering context!");
	}

	switch (API) {
		default:
			std::cerr << "Rendering context unknown! Defaulting to OpenGL..." << std::endl;
			[[fallthrough]];
		case AvailableRenderingContexts::OpenGL:
			renderingMethod = new OpenGLRenderingContext();
			renderingMethodType = AvailableRenderingContexts::OpenGL;
			break;
		case AvailableRenderingContexts::software:
			renderingMethod = new SoftwareRenderingContext();
			renderingMethodType = AvailableRenderingContexts::software;
			break;
		case AvailableRenderingContexts::null_rendering:
			renderingMethod = new NullRenderingContext();
			renderingMethodType = AvailableRenderingContexts::null_rendering;
			break;
	}
}

void Renderer::SetContext(std::string API) {
	AvailableRenderingContexts context;
	if (API == "OpenGL") {
		context = AvailableRenderingContexts::OpenGL;
	} else if (API == "software") {
		context = AvailableRenderingContexts::software;
	} else if (API == "null" || API == "NULL") {
		context = AvailableRenderingContexts::null_rendering;
	} else {
		std::cerr << "Rendering context unknown! Defaulting to OpenGL..." << std::endl;
		context = AvailableRenderingContexts::OpenGL;
	}

	SetContext(context);
}

void Renderer::PreInitialize(int* argc, char** argv, std::string windowName) {
	Renderer::PreInitialize(argc, argv, windowName, 120, 120);
}

void Renderer::PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY) {
	Renderer::PreInitialize(argc, argv, windowName, startX, startY, 2.5);
}

void Renderer::PreInitialize(int* argc, char** argv, std::string windowName, int startX, int startY, double sizeMultiplier) {
	// Initialize GLFW
	if (!glfwInit()) {
		throw "glfw failed";
	}

	// Setup window position, size, and title
	GLFWwindow* window;
	Renderer::window_width = GAME_WIDTH*sizeMultiplier; Renderer::window_height = GAME_HEIGHT*sizeMultiplier;
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	//glfwWindowHint(GLFW_SAMPLES, 64); //TODO: doesn't seem to work
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); //unnecessary
	window = glfwCreateWindow(Renderer::window_width, Renderer::window_height, windowName.c_str(), NULL, NULL);
	glfwSetWindowPos(window, startX, startY); //TODO: this sets the inner window content part (framebuffer?)'s position, I think
	Renderer::glfw_window = window;
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_MULTISAMPLE);

	glfwGetWindowPos(Renderer::glfw_window, &Renderer::old_window_xpos, &Renderer::old_window_ypos);
	glfwGetWindowSize(Renderer::glfw_window, &Renderer::old_window_width, &Renderer::old_window_height); //required for monitor content scaling
	Renderer::window_width = Renderer::old_window_width; Renderer::window_height = Renderer::old_window_height;
	Renderer::gamewindow_width = Renderer::window_width; Renderer::gamewindow_height = Renderer::window_height;
	//glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);

	glfwMakeContextCurrent(window);

	//thanks to https://community.khronos.org/t/wglmakecurrent-issues/62656/3 for solving why a draw call would take ~15ms for no reason (it's just the V-sync time)
	//glfwSwapInterval(0);

	//glDisable(GL_DEPTH_TEST);
	//transparency:
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//initialize glew
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		throw "glew failed";
	}
}

void Renderer::Initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Shader* shader = new Shader("res/shaders/main.vert", "res/shaders/main.frag");
	shaderCache.insert({ "main", shader });
	//bindShader(shader); //the main shader will be used most often so it gets binded at start

	//shader = new Shader("res/shaders/default.vert", "res/shaders/default.frag");
	//shaderCache.insert({ "default", shader });

	initializeGPU();

	//thread_keepRunning.store(true);
	//thread_workExists.store(false);
	//graphicsThread = std::thread(thread_func);
}

void Renderer::Uninitialize() {
	//thread_keepRunning.store(false);
	//thread_workExists.store(true);
	//graphicsThread.join();
	////uninitializeGPU();
	glfwTerminate();
}

/*
void Renderer::thread_func() {
	while (thread_keepRunning.load()) {
		while (!thread_workExists.load()) {
			//spin
			//std::cout << "thread2 waiting\n";
		}
		drawingDataLock.lock();

		if (!thread_keepRunning.load()) {
			drawingDataLock.unlock();
			break;
		}

		Renderer::ActuallyFlush();
		//std::cout << "thread2 did work\n";
		thread_workExists.store(false);
		drawingDataLock.unlock();
	}
}
*/

glm::mat4 Renderer::GenerateModelMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY) {
	//glm::mat4 trans = glm::translate(proj, glm::vec3(transX, transY, 0.0f));
	glm::mat4 trans = glm::translate(glm::mat4(), glm::vec3(transX, transY, 0.0f));
	//if (rotateAngle == 0) {
	//	return glm::scale(trans, glm::vec3(scaleX, scaleY, 0));
	//}
	glm::mat4 rot = glm::rotate(trans, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::scale(rot, glm::vec3(scaleX, scaleY, 0));
}

void Renderer::SetViewMatrix(float cameraX, float cameraY, float cameraZ, float targetX, float targetY, float targetZ) {
	/*
	if ((!viewMatBound) ||
	    ((cameraX != current_cameraX || cameraY != current_cameraY || cameraZ != current_cameraZ) &&
	    (targetX != current_targetX || targetY != current_targetY || targetZ != current_targetZ))) {
			current_cameraX = cameraX;
			current_cameraY = cameraY;
			current_cameraZ = cameraZ;
			current_targetX = targetX;
			current_targetY = targetY;
			current_targetZ = targetZ;
			boundShader->setUniformMat4f("u_ViewMatrix", glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(targetX, targetY, targetZ), glm::vec3(0, 1, 0)));
			viewMatBound = true;
	}
	*/
	if (!viewMatBound) {
		boundShader->setUniformMat4f("u_ViewMatrix", glm::mat4(1.0f));
		//note: glm::mat4() is *not* an identity matrix
		viewMatBound = true;
	}
}

void Renderer::SetProjectionMatrix() {
	if (!projectionMatBound) {
		boundShader->setUniformMat4f("u_ProjectionMatrix", Renderer::proj);
		projectionMatBound = true;
	}
}

inline void Renderer::bindShader(Shader* shader) {
	if (currentShader != shader->getRendererID()) {
		shader->Bind();
		currentShader = shader->getRendererID();
		boundShader = shader;

		viewMatBound = false;
		projectionMatBound = false;

		Renderer::SetViewMatrix(0, 0, -1, 0, 0, 0);
		Renderer::SetProjectionMatrix();
	}
}

inline void Renderer::bindShader(const Shader& shader) {
	if (currentShader != shader.getRendererID()) {
		shader.Bind();
		currentShader = shader.getRendererID();
	}
}

inline void Renderer::bindVertexArray(const VertexArray& va) {
	if (currentVertexArray != va.getRendererID()) {
		va.Bind();
		currentVertexArray = va.getRendererID();
	}
}

inline void Renderer::bindIndexBuffer(const IndexBuffer& ib) {
	if (currentIndexBuffer != ib.getRendererID()) {
		ib.Bind();
		currentIndexBuffer = ib.getRendererID();
	}
}

Shader* Renderer::getShader(std::string s) {
	//return shaderCache[s];

	auto get = shaderCache.find(s);
	if (get != shaderCache.end()) {
		Shader* shader = shaderCache[s];
		//if (shader != boundShader) {
			bindShader(shader);
		//}
		return shader;
	}
	//else shader wasn't found
	std::cerr << "Could not find '" << s << "' shader!" << std::endl;

	//return the magenta shader, just so there's something
	get = shaderCache.find("default");
	if (get != shaderCache.end()) {
		Shader* shader = shaderCache[s];
		bindShader(shader);
		return shader;
	}
	//else big uh-oh
	std::cerr << "Could not find the default shader!" << std::endl;

	return nullptr; //default magenta shader is missing
}

void Renderer::Unbind(const VertexArray& va) {
	if (currentVertexArray == va.getRendererID()) {
		va.Unbind();
		currentVertexArray = -1;
	}
}

void Renderer::Unbind(const IndexBuffer& ib) {
	//if (currentVertexArray == ib.getRendererID()) {
	if (currentIndexBuffer == ib.getRendererID()) {
		ib.Unbind();
		currentIndexBuffer = -1;
	}
}

void Renderer::Unbind(const Shader& s) {
	//if (currentVertexArray == s.getRendererID()) {
	if (currentShader == s.getRendererID()) {
		s.Unbind();
		currentShader = -1;
	}
}

void Renderer::UnbindAll() {
	glBindVertexArray(0); //vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //index buffer
	glUseProgram(0); //shader

	currentVertexArray = -1;
	currentIndexBuffer = -1;
	currentShader = -1;
}

std::string Renderer::getErrorString(GLenum err) {
	//gotten from https://codeyarns.com/2015/09/14/how-to-check-error-in-opengl/
	switch (err) {
		case GL_NO_ERROR:          return "No error";
		case GL_INVALID_ENUM:      return "Invalid enum";
		case GL_INVALID_VALUE:     return "Invalid value";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_STACK_OVERFLOW:    return "Stack overflow";
		case GL_STACK_UNDERFLOW:   return "Stack underflow";
		case GL_OUT_OF_MEMORY:     return "Out of memory";
		default:                   return "Unknown error";
	}
}

void Renderer::printGLError() {
	bool error = false;
	while (true) {
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << getErrorString(err) << std::endl;
		error = true; //set breakpoint to here when debugging!
	}
	if (!error) {
		std::cout << "no error" << std::endl;
	}
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ActuallyFlush() {
	auto start = Diagnostics::getTime();
	for (int i = 0; i < sceneList.size(); i++) {
		const std::string& name = sceneList[i];
		std::vector<std::pair<std::vector<float>, std::vector<unsigned int>>>& sceneDrawCalls = sceneData[name];
		for (int j = 0; j < sceneDrawCalls.size(); j++) {
			BatchedFlush(sceneDrawCalls[j].first, sceneDrawCalls[j].second);
		}
		sceneDrawCalls.clear();
	}
	sceneList.clear();
	auto end = Diagnostics::getTime();

	Diagnostics::pushGraphTime("draw", Diagnostics::getDiff(start, end));
	Diagnostics::pushGraphSumTime("all");

	const GameSettings& game_settings = GameManager::get_settings();
	if (game_settings.PerformanceGraphEnable) {
		Diagnostics::drawGraphTimes();
	}

	//for single framebuffer, use glFlush; for double framebuffer, swap the buffers
	//swapping buffers is limited to monitor refresh rate, so I use glFlush
	glFlush();
	//glfwSwapBuffers(glfw_window);

	UnbindAll();
}

void Renderer::Flush() {
	//thread_workExists.store(true);

	ActuallyFlush();
	//drawingDataLock.unlock();
}

void Renderer::Cleanup() {
	//glDisableVertexAttribArray(0); //disable vertex attribute to avoid issues
}

bool Renderer::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float* positions = new float[maxVerticesDataLength];
	std::fill(positions, positions + maxVerticesDataLength, 0);

	unsigned int* indices = new unsigned int[maxIndicesDataLength];
	std::fill(indices, indices + maxIndicesDataLength, 0);

	batched_vb = VertexBuffer::MakeVertexBuffer(positions, maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	batched_vb->SetLayout(layout);

	batched_ib = IndexBuffer::MakeIndexBuffer(indices, maxIndicesDataLength);

	batched_va = VertexArray::MakeVertexArray();
	batched_va->AddVertexBuffer(batched_vb);
	batched_va->SetIndexBuffer(batched_ib);

	delete[] positions;
	delete[] indices;
	initialized_GPU = true;
	return true;
}

bool Renderer::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete batched_va;
	delete batched_vb;
	delete batched_ib;

	initialized_GPU = false;
	return true;
}

inline bool Renderer::enoughRoomForMoreVertices(int pushLength) {
	const std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
	return (currentSceneData.first.size() + pushLength <= maxVerticesDataLength);
}
inline bool Renderer::enoughRoomForMoreIndices(int pushLength) {
	const std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
	return (currentSceneData.second.size() + pushLength <= maxIndicesDataLength);
}

inline void Renderer::pushAnotherDataList() {
	sceneData[currentSceneName].push_back({});

	#if _DEBUG
	//performance is awful
	#else
	sceneData[currentSceneName][sceneData[currentSceneName].size()-1].first.reserve(maxVerticesDataLength);
	sceneData[currentSceneName][sceneData[currentSceneName].size()-1].second.reserve(maxIndicesDataLength);
	#endif
}

void Renderer::SubmitBatchedDraw(const float* posAndColor, int posAndColorLength, const unsigned int* indices, int indicesLength) {
	//idea for multithreading vertex upload: add an int threadID parameter, turn the single list of vertices into a list of lists
	//would require modifying draw(), so hold off for now

	if (currentSceneName == "") [[unlikely]] {
		//only happens for Diagnostics
		std::vector<float> verticesData = std::vector<float>(posAndColor, posAndColor + posAndColorLength);
		std::vector<unsigned int> indicesData = std::vector<unsigned int>(indices, indices + indicesLength);
		BatchedFlush(verticesData, indicesData);
	} else {
		if (!enoughRoomForMoreVertices(posAndColorLength) || !enoughRoomForMoreIndices(indicesLength)) {
			//std::cout << enoughRoomForMoreVertices(posAndColorLength) << enoughRoomForMoreIndices(indicesLength);
			pushAnotherDataList();
			if (posAndColorLength > maxVerticesDataLength || indicesLength > maxIndicesDataLength) [[unlikely]] {
				std::cerr << "GPU buffer overrun!" << std::endl;
			}
		}

		std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
		std::vector<float>& verticesData = currentSceneData.first;
		std::vector<unsigned int>& indicesData = currentSceneData.second;

		unsigned int currVerticesLength = verticesData.size();
		verticesData.insert(verticesData.end(), posAndColor, posAndColor + posAndColorLength);

		unsigned int currIndicesLength = indicesData.size();
		indicesData.insert(indicesData.end(), indices, indices + indicesLength);
		for (unsigned int i = currIndicesLength; i < indicesData.size(); i++) {
			indicesData[i] += currVerticesLength/(2+4);
		}
	}
}

void Renderer::BatchedFlush(std::vector<float>& verticesData, std::vector<unsigned int>& indicesData) {
	if (verticesData.empty()) [[unlikely]] {
		return;
	}

	batched_vb->modifyData(verticesData.data(), verticesData.size() * sizeof(float));
	batched_ib->modifyData(indicesData.data(), indicesData.size() * sizeof(unsigned int));

	bindShader(Renderer::getShader("main"));
	Renderer::getShader("main")->setUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));
	bindVertexArray(*batched_va);
	bindIndexBuffer(*batched_ib);

	glDrawElements(GL_TRIANGLES, batched_va->GetIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	//glDrawElements(GL_TRIANGLES, indicesData.size(), GL_UNSIGNED_INT, nullptr);

	verticesData.clear();
	indicesData.clear();
}

void Renderer::BeginScene(std::string name) {
	currentSceneName = name;
	sceneList.push_back(name);
	pushAnotherDataList();
}

void Renderer::EndScene() {
	currentSceneName = "";
	//not sure what to put...
}

bool Renderer::isDebugDrawingEnabled(std::string name) {
	const BasicINIParser::BasicINIData& ini_data = GameManager::get_INI();

	bool drawingEnabled = false;
	if (ini_data.exists("DEBUG", "EnableDebugDrawingObjects")) {
		int length = ini_data.length("DEBUG", "EnableDebugDrawingObjects");
		for (int i = 0; i < length; i++) {
			if (ini_data.get("DEBUG", "EnableDebugDrawingObjects", i) == name) {
				drawingEnabled = true;
				break;
			}
		}
	}
	return drawingEnabled;
}
