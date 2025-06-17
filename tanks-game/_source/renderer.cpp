#include "renderer.h"

#include "constants.h"
#include <algorithm> //std::fill
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include "bullet.h" //to avoid passing in the bullet's vertices every time

#include "graphics/opengl-rendering-context.h"
#include "graphics/software-rendering-context.h"
#include "graphics/null-rendering-context.h"

#include "diagnostics.h"
#include "game-manager.h" //for isDebugDrawingEnabled()

//std::mutex Renderer::drawingDataLock;
//std::thread Renderer::graphicsThread;
//std::atomic_bool Renderer::thread_keepRunning;
//std::atomic_bool Renderer::thread_workExists;

glm::mat4 Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
glm::mat4 Renderer::getProj() { return proj; }
RenderingContext* Renderer::renderingMethod = nullptr;
AvailableRenderingContexts Renderer::renderingMethodType;

float Renderer::current_cameraX = 0, Renderer::current_cameraY = 0, Renderer::current_cameraZ = -1;
float Renderer::current_targetX = 0, Renderer::current_targetY = 0, Renderer::current_targetZ = 0;
bool Renderer::viewMatBound = false, Renderer::projectionMatBound = false;
Shader* Renderer::boundShader = nullptr;

VertexArray* Renderer::batched_va;
VertexBuffer* Renderer::batched_vb;
IndexBuffer* Renderer::batched_ib;
VertexArray* Renderer::instanced_va;
VertexBuffer* Renderer::instanced_vb_pos;
VertexBuffer* Renderer::instanced_vb_color;
VertexBuffer* Renderer::instanced_vb_life;
VertexBuffer* Renderer::instanced_vb_mat;
IndexBuffer* Renderer::instanced_ib;
bool Renderer::initialized_GPU = false;

std::unordered_map<std::string, std::vector<Renderer::VertexDrawingData*>> Renderer::sceneData;
std::vector<std::string> Renderer::sceneList;
std::string Renderer::currentSceneName = "";
//std::vector<float>* Renderer::currentVerticesData;
//std::vector<unsigned int>* Renderer::currentIndicesData;
const unsigned int Renderer::maxVerticesDataLength = (1 << 16) / sizeof(float); //2^16 - 2^18 have basically identical performance; lower results in problems, higher results in stuttering/spikes with worse performance
const unsigned int Renderer::maxIndicesDataLength = (1 << 16) / sizeof(unsigned int); //TODO: size (fills up faster)

std::unordered_map<std::string, Shader*> Renderer::shaderStorage;
unsigned int Renderer::currentShader = -1;
unsigned int Renderer::currentVertexArray = -1;
unsigned int Renderer::currentIndexBuffer = -1;

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

void Renderer::Initialize() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Shader* shader = new Shader("res/shaders/main.vert", "res/shaders/main.frag");
	shaderStorage.insert({ "main", shader });

	//shader = new Shader("res/shaders/default.vert", "res/shaders/default.frag");
	//shaderStorage.insert({ "default", shader });

	shader = new Shader("res/shaders/bullet.vert", "res/shaders/bullet.frag");
	shaderStorage.insert({ "bullet", shader });

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

bool Renderer::MainBatched_VertexData::enoughRoomForMoreVertices(unsigned int pushLength) {
	return (m_vertices.size() + pushLength <= Renderer::maxVerticesDataLength);
}
bool Renderer::MainBatched_VertexData::enoughRoomForMoreIndices(unsigned int pushLength) {
	return (m_indices.size() + pushLength <= Renderer::maxIndicesDataLength);
}

bool Renderer::Bullet_VertexData::enoughRoomForMoreColors(unsigned int pushLength) {
	//TODO: maxVerticesDataLength
	return (m_colors.size() + pushLength <= Renderer::maxVerticesDataLength);
}
bool Renderer::Bullet_VertexData::enoughRoomForMoreLifeValues(unsigned int pushLength) {
	//TODO: maxVerticesDataLength
	return (m_lifeValues.size() + pushLength <= Renderer::maxVerticesDataLength);
}
bool Renderer::Bullet_VertexData::enoughRoomForMoreMatrices(unsigned int pushLength) {
	//TODO: maxVerticesDataLength
	return (m_modelMatrices.size() + pushLength <= Renderer::maxVerticesDataLength);
}

Renderer::MainBatched_VertexData::MainBatched_VertexData() {
	m_shaderName = "main";
	#if _DEBUG
	//performance is awful //TODO: check again
	#else
	m_vertices.reserve(maxVerticesDataLength);
	m_indices.reserve(maxIndicesDataLength);
	#endif
}

Renderer::Bullet_VertexData::Bullet_VertexData() {
	m_shaderName = "bullet";
	#if _DEBUG
	//performance is awful //TODO: check again
	#else
	m_colors.reserve(maxVerticesDataLength);
	m_lifeValues.reserve(maxVerticesDataLength);
	m_modelMatrices.reserve(maxVerticesDataLength);
	#endif
}

glm::mat4 Renderer::GenerateModelMatrix(float scaleX, float scaleY, float rotateAngle, float transX, float transY) {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, transY, 0.0f));
	glm::mat4 rot = glm::rotate(trans, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::scale(rot, glm::vec3(scaleX, scaleY, 0));
}

glm::mat4 Renderer::GenerateModelMatrix_NoRotate(float scaleX, float scaleY, float transX, float transY) {
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, transY, 0.0f));
	return glm::scale(trans, glm::vec3(scaleX, scaleY, 0));
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

	auto get = shaderStorage.find(s);
	if (get != shaderStorage.end()) {
		Shader* shader = shaderStorage[s];
		bindShader(shader);
		return shader;
	}
	//else shader wasn't found
	std::cerr << "Could not find '" << s << "' shader!" << std::endl;

	//return the magenta shader, just so there's something
	get = shaderStorage.find("default");
	if (get != shaderStorage.end()) {
		Shader* shader = shaderStorage[s];
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
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ActuallyFlush() {
	auto start = Diagnostics::getTime();
	std::string currentShaderName = "";
	for (int i = 0; i < sceneList.size(); i++) {
		const std::string& sceneName = sceneList[i];
		std::vector<VertexDrawingData*>& sceneDrawCalls = sceneData[sceneName];
		for (int j = 0; j < sceneDrawCalls.size(); j++) {
			if (sceneDrawCalls[j]->m_shaderName != currentShaderName) {
				if (sceneDrawCalls[j]->m_shaderName == "main") {
					bindShader(Renderer::getShader("main"));
					Renderer::getShader("main")->setUniformMat4f("u_ModelMatrix", glm::mat4(1.0f));
					bindVertexArray(*batched_va);
					bindIndexBuffer(*batched_ib);
					SetProjectionMatrix();
				} else if (sceneDrawCalls[j]->m_shaderName == "bullet") {
					instanced_vb_pos->modifyData(Bullet::instanced_vertices, sizeof(Bullet::instanced_vertices));
					instanced_ib->modifyData(Bullet::instanced_indices, sizeof(Bullet::instanced_indices));

					Shader* shader = Renderer::getShader("bullet");
					bindShader(shader);
					bindVertexArray(*instanced_va);
					bindIndexBuffer(*instanced_ib);
					SetProjectionMatrix();
				} else {
					//oh no
				}
				currentShaderName = sceneDrawCalls[j]->m_shaderName;
			}

			if (currentShaderName == "main") {
				BatchedFlush_New(static_cast<MainBatched_VertexData*>(sceneDrawCalls[j]));
				//TODO: move the delete here
			} else if (currentShaderName == "bullet") {
				BulletFlush(static_cast<Bullet_VertexData*>(sceneDrawCalls[j]));
				//TODO: move the delete here
			} else {
				//oh no
			}

			delete sceneDrawCalls[j]; //TODO
		}
		sceneDrawCalls.clear();
	}
	sceneList.clear(); //TODO: this is a surprising area for CPU time (_Tidy from reallocating) (or is it the line above?)
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

	#if 0
	auto start = Diagnostics::getTime();
	for (int i = 0; i < sceneList.size(); i++) {
		const std::string& name = sceneList[i];
		std::vector<std::pair<std::vector<float>, std::vector<unsigned int>>>& sceneDrawCalls = sceneData[name];
		for (int j = 0; j < sceneDrawCalls.size(); j++) {
			BatchedFlush(sceneDrawCalls[j].first, sceneDrawCalls[j].second);
		}
		sceneDrawCalls.clear();
	}
	sceneList.clear(); //TODO: this is a surprising area for CPU time (_Tidy from reallocating) (or is it the line above?)
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
	//glfwSwapBuffers(WindowInitializer::glfw_window);

	UnbindAll();
	#endif
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

	float* positions_instanced = new float[maxVerticesDataLength];
	std::fill(positions_instanced, positions_instanced + maxVerticesDataLength, 0);

	unsigned int* indices_instanced = new unsigned int[maxIndicesDataLength];
	std::fill(indices_instanced, indices_instanced + maxIndicesDataLength, 0);

	instanced_vb_pos = VertexBuffer::MakeVertexBuffer(positions_instanced, maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_pos = {
		{ ShaderDataType::Float2, "a_Position" }
	};
	instanced_vb_pos->SetLayout(layout_instanced_pos);

	instanced_vb_color = VertexBuffer::MakeVertexBuffer(positions_instanced, maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_color = {
		{ ShaderDataType::Float4, "a_ColorInstanced", false, true }
	};
	instanced_vb_color->SetLayout(layout_instanced_color);

	instanced_vb_life = VertexBuffer::MakeVertexBuffer(positions_instanced, maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_life = {
		{ ShaderDataType::Float, "a_BulletLifeInstanced", false, true }
	};
	instanced_vb_life->SetLayout(layout_instanced_life);

	instanced_vb_mat = VertexBuffer::MakeVertexBuffer(positions_instanced, maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_mat = {
		{ ShaderDataType::Mat4, "a_ModelMatrixInstanced" }
	};
	instanced_vb_mat->SetLayout(layout_instanced_mat);

	instanced_ib = IndexBuffer::MakeIndexBuffer(indices_instanced, maxIndicesDataLength);

	instanced_va = VertexArray::MakeVertexArray();
	instanced_va->AddVertexBuffer(instanced_vb_pos);
	instanced_va->AddVertexBuffer(instanced_vb_color);
	instanced_va->AddVertexBuffer(instanced_vb_life);
	instanced_va->AddVertexBuffer(instanced_vb_mat);
	instanced_va->SetIndexBuffer(instanced_ib);

	delete[] positions_instanced;
	delete[] indices_instanced;

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
	delete instanced_va;
	delete instanced_vb_pos;
	delete instanced_vb_color;
	delete instanced_vb_life;
	delete instanced_vb_mat;
	delete instanced_ib;

	initialized_GPU = false;
	return true;
}

#if 0
inline bool Renderer::enoughRoomForMoreVertices(int pushLength) {
	//const std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
	return (currentVerticesData->size() + pushLength <= maxVerticesDataLength);
}
inline bool Renderer::enoughRoomForMoreIndices(int pushLength) {
	//const std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
	return (currentIndicesData->size() + pushLength <= maxIndicesDataLength);
}

inline void Renderer::pushAnotherDataList() {
	sceneData[currentSceneName].push_back({});

	#if _DEBUG
	//performance is awful
	#else
	sceneData[currentSceneName][sceneData[currentSceneName].size()-1].first.reserve(maxVerticesDataLength);
	sceneData[currentSceneName][sceneData[currentSceneName].size()-1].second.reserve(maxIndicesDataLength);
	#endif
	currentVerticesData = &sceneData[currentSceneName][sceneData[currentSceneName].size()-1].first;
	currentIndicesData  = &sceneData[currentSceneName][sceneData[currentSceneName].size()-1].second;
}
#endif

void Renderer::SubmitBatchedDraw(const float* posAndColor, unsigned int posAndColorLength, const unsigned int* indices, unsigned int indicesLength) {
	if (currentSceneName == "") [[unlikely]] {
		//only happens for Diagnostics
		MainBatched_VertexData* tempVertexDataGroup = new MainBatched_VertexData();
		tempVertexDataGroup->m_vertices.insert(tempVertexDataGroup->m_vertices.end(), posAndColor, posAndColor + posAndColorLength);
		tempVertexDataGroup->m_indices.insert(tempVertexDataGroup->m_indices.end(), indices, indices + indicesLength);
		BatchedFlush_New(tempVertexDataGroup);
		delete tempVertexDataGroup;
	} else {
		MainBatched_VertexData* currentVertexDataGroup;
		std::vector<VertexDrawingData*>& sceneDrawGroup = sceneData[currentSceneName];
		if (sceneDrawGroup.empty() || sceneDrawGroup[sceneDrawGroup.size()-1]->m_shaderName != "main") {
			currentVertexDataGroup = new MainBatched_VertexData();
			sceneDrawGroup.push_back(static_cast<VertexDrawingData*>(currentVertexDataGroup));
		} else {
			currentVertexDataGroup = static_cast<MainBatched_VertexData*>(sceneDrawGroup[sceneDrawGroup.size()-1]);
			if (!currentVertexDataGroup->enoughRoomForMoreVertices(posAndColorLength) || !currentVertexDataGroup->enoughRoomForMoreIndices(indicesLength)) {
				currentVertexDataGroup = new MainBatched_VertexData();
				sceneDrawGroup.push_back(static_cast<VertexDrawingData*>(currentVertexDataGroup));

				if (posAndColorLength > maxVerticesDataLength || indicesLength > maxIndicesDataLength) [[unlikely]] {
					std::cerr << "GPU buffer overrun!" << std::endl;
				}
			}
		}

		std::vector<float>& verticesData = currentVertexDataGroup->m_vertices;
		std::vector<unsigned int>& indicesData = currentVertexDataGroup->m_indices;

		unsigned int currVerticesLength = verticesData.size();
		verticesData.insert(verticesData.end(), posAndColor, posAndColor + posAndColorLength);

		unsigned int currIndicesLength = indicesData.size();
		indicesData.insert(indicesData.end(), indices, indices + indicesLength);
		for (unsigned int i = currIndicesLength; i < indicesData.size(); i++) {
			indicesData[i] += currVerticesLength/(2+4);
			//could glDrawElementsBaseVertex() be usable to avoid fixing indices?
		}
	}

	#if 0
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

		//std::pair<std::vector<float>, std::vector<unsigned int>>& currentSceneData = sceneData[currentSceneName][sceneData[currentSceneName].size()-1];
		std::vector<float>& verticesData = *currentVerticesData;
		std::vector<unsigned int>& indicesData = *currentIndicesData;

		unsigned int currVerticesLength = verticesData.size();
		verticesData.insert(verticesData.end(), posAndColor, posAndColor + posAndColorLength);

		unsigned int currIndicesLength = indicesData.size();
		indicesData.insert(indicesData.end(), indices, indices + indicesLength);
		for (unsigned int i = currIndicesLength; i < indicesData.size(); i++) {
			indicesData[i] += currVerticesLength/(2+4);
		}
	}
	#endif
}

void Renderer::SubmitBulletDrawCall(float bulletLife, const ColorValueHolder& color, const glm::mat4& modelMatrix) {
	if (currentSceneName == "") [[unlikely]] {
		//TODO
	} else {
		Bullet_VertexData* currentVertexDataGroup;
		std::vector<VertexDrawingData*>& sceneDrawGroup = sceneData[currentSceneName];
		if (sceneDrawGroup.empty() || sceneDrawGroup[sceneDrawGroup.size()-1]->m_shaderName != "bullet") {
			currentVertexDataGroup = new Bullet_VertexData();
			sceneDrawGroup.push_back(static_cast<VertexDrawingData*>(currentVertexDataGroup));
		} else {
			currentVertexDataGroup = static_cast<Bullet_VertexData*>(sceneDrawGroup[sceneDrawGroup.size()-1]);
			if (!currentVertexDataGroup->enoughRoomForMoreColors(4) || !currentVertexDataGroup->enoughRoomForMoreLifeValues(1) || !currentVertexDataGroup->enoughRoomForMoreMatrices(4*4)) {
				currentVertexDataGroup = new Bullet_VertexData();
				sceneDrawGroup.push_back(static_cast<VertexDrawingData*>(currentVertexDataGroup));
			}
		}

		std::vector<float>& colorsData = currentVertexDataGroup->m_colors;
		std::vector<float>& lifeValuesData = currentVertexDataGroup->m_lifeValues;
		std::vector<float>& modelMatricesData = currentVertexDataGroup->m_modelMatrices;

		//colorsData.insert(colorsData.end(), { color.getRf(), color.getGf(), color.getBf(), color.getAf() });
		colorsData.insert(colorsData.end(), reinterpret_cast<const float*>(&color), reinterpret_cast<const float*>(&color) + 4);
		lifeValuesData.push_back(bulletLife);
		modelMatricesData.insert(modelMatricesData.end(), &modelMatrix[0][0], &modelMatrix[0][0] + 4*4);
	}
}

#if 0
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
#endif

void Renderer::BatchedFlush_New(MainBatched_VertexData* drawData) {
	//TODO: check if there's nothing?

	batched_vb->modifyData(drawData->m_vertices.data(), drawData->m_vertices.size() * sizeof(float));
	batched_ib->modifyData(drawData->m_indices.data(), drawData->m_indices.size() * sizeof(unsigned int));

	glDrawElements(GL_TRIANGLES, batched_va->GetIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	//glDrawElements(GL_TRIANGLES, drawData->m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::BulletFlush(Bullet_VertexData* drawData) {
	//TODO: check if there's nothing?

	instanced_vb_color->modifyData(drawData->m_colors.data(), drawData->m_colors.size() * sizeof(float));
	instanced_vb_life->modifyData(drawData->m_lifeValues.data(), drawData->m_lifeValues.size() * sizeof(float));
	instanced_vb_mat->modifyData(drawData->m_modelMatrices.data(), drawData->m_modelMatrices.size() * sizeof(float));

	const unsigned int bulletCount = drawData->m_lifeValues.size();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(Bullet::instanced_indices)/sizeof(*Bullet::instanced_indices), GL_UNSIGNED_INT, nullptr, bulletCount);
}

void Renderer::BeginScene(std::string name) {
	currentSceneName = name;
	sceneList.push_back(name);
	//pushAnotherDataList();
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
