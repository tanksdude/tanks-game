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

glm::mat4 Renderer::proj = glm::ortho(0.0f, (float)GAME_WIDTH, 0.0f, (float)GAME_HEIGHT);
RenderingContext* Renderer::renderingMethod = nullptr;
AvailableRenderingContexts Renderer::renderingMethodType;

std::unordered_map<std::string, Shader*> Renderer::shaderStorage;
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
const unsigned int Renderer::MainBatched_VertexData::maxVerticesDataLength = (1 << 16) / sizeof(float); //2^16 - 2^18 have basically identical performance; lower results in problems, higher results in stuttering/spikes with worse performance
const unsigned int Renderer::MainBatched_VertexData::maxIndicesDataLength = (1 << 16) / sizeof(unsigned int); //TODO: size (fills up faster)
const unsigned int Renderer::Bullet_VertexData::maxDataLength = (1 << 16) / sizeof(float);

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

void Renderer::SetContext(const std::string& API) {
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
}

void Renderer::Uninitialize() {
	//uninitializeGPU();
}

bool Renderer::MainBatched_VertexData::enoughRoomForMoreVertices(unsigned int pushLength) {
	return (m_vertices.size() + pushLength <= Renderer::MainBatched_VertexData::maxVerticesDataLength);
}
bool Renderer::MainBatched_VertexData::enoughRoomForMoreIndices(unsigned int pushLength) {
	return (m_indices.size() + pushLength <= Renderer::MainBatched_VertexData::maxIndicesDataLength);
}

bool Renderer::Bullet_VertexData::enoughRoomForMoreColors(unsigned int pushLength) {
	return (m_colors.size() + pushLength <= Renderer::Bullet_VertexData::maxDataLength);
}
bool Renderer::Bullet_VertexData::enoughRoomForMoreLifeValues(unsigned int pushLength) {
	return (m_lifeValues.size() + pushLength <= Renderer::Bullet_VertexData::maxDataLength);
}
bool Renderer::Bullet_VertexData::enoughRoomForMoreMatrices(unsigned int pushLength) {
	return (m_modelMatrices.size() + pushLength <= Renderer::Bullet_VertexData::maxDataLength);
}

Renderer::MainBatched_VertexData::MainBatched_VertexData() {
	m_shaderName = "main";
	#if _DEBUG
	//performance is awful //TODO: check again
	#else
	m_vertices.reserve(MainBatched_VertexData::maxVerticesDataLength);
	m_indices.reserve(MainBatched_VertexData::maxIndicesDataLength);
	#endif
}

Renderer::Bullet_VertexData::Bullet_VertexData() {
	m_shaderName = "bullet";
	#if _DEBUG
	//performance is awful //TODO: check again
	#else
	m_colors.reserve(Bullet_VertexData::maxDataLength);
	m_lifeValues.reserve(Bullet_VertexData::maxDataLength);
	m_modelMatrices.reserve(Bullet_VertexData::maxDataLength);
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
	boundShader->setUniformMat4f("u_ViewMatrix", glm::lookAt(glm::vec3(cameraX, cameraY, cameraZ), glm::vec3(targetX, targetY, targetZ), glm::vec3(0, 1, 0)));
	//boundShader->setUniformMat4f("u_ViewMatrix", glm::mat4(1.0f));
}

void Renderer::SetProjectionMatrix() {
	boundShader->setUniformMat4f("u_ProjectionMatrix", Renderer::proj);
}

inline void Renderer::bindShader(Shader* shader) {
	shader->Bind();
	boundShader = shader;
	SetViewMatrix(0, 0, 1, 0, 0, 0);
	SetProjectionMatrix();
}

inline void Renderer::bindVertexArray(const VertexArray* va) {
	va->Bind();
}

inline void Renderer::bindIndexBuffer(const IndexBuffer* ib) {
	ib->Bind();
}

Shader* Renderer::getShader(const std::string& s) {
	auto get = shaderStorage.find(s);
	if (get != shaderStorage.end()) {
		return get->second;
	}
	//else shader wasn't found
	std::cerr << "Could not find '" << s << "' shader!" << std::endl;

	//return the magenta shader, just so there's something
	get = shaderStorage.find("default");
	if (get != shaderStorage.end()) {
		return get->second;
	}
	//else big uh-oh
	std::cerr << "Could not find the default shader!" << std::endl;

	return nullptr; //default magenta shader is missing
}

void Renderer::UnbindAll() {
	glBindVertexArray(0); //vertex array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //index buffer
	glUseProgram(0); //shader

	boundShader = nullptr;
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
		if (err == GL_NO_ERROR)
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
					bindVertexArray(batched_va);
					//bindIndexBuffer(batched_ib); //TODO: is this necessary?
				} else if (sceneDrawCalls[j]->m_shaderName == "bullet") {
					instanced_vb_pos->modifyData(Bullet::instanced_vertices, sizeof(Bullet::instanced_vertices));
					instanced_ib->modifyData(Bullet::instanced_indices, sizeof(Bullet::instanced_indices));

					bindShader(Renderer::getShader("bullet"));
					bindVertexArray(instanced_va);
					//bindIndexBuffer(instanced_ib); //TODO: is this necessary?
				} else {
					//oh no
				}
				currentShaderName = sceneDrawCalls[j]->m_shaderName;
			}

			if (currentShaderName == "main") {
				BatchedFlush(static_cast<MainBatched_VertexData*>(sceneDrawCalls[j]));
				delete static_cast<MainBatched_VertexData*>(sceneDrawCalls[j]);
			} else if (currentShaderName == "bullet") {
				BulletFlush(static_cast<Bullet_VertexData*>(sceneDrawCalls[j]));
				delete static_cast<Bullet_VertexData*>(sceneDrawCalls[j]);
			} else {
				//oh no
				delete sceneDrawCalls[j]; //TODO
			}
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
}

void Renderer::Flush() {
	//TODO: this function is separate in case graphics will be put on a separate thread
	//(which wouldn't be particularly useful, as rendering is typically <1ms in worst case scenarios)

	ActuallyFlush();
}

void Renderer::Cleanup() {
	//TODO?
}

bool Renderer::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float* positions = new float[MainBatched_VertexData::maxVerticesDataLength];
	std::fill(positions, positions + MainBatched_VertexData::maxVerticesDataLength, 0);

	unsigned int* indices = new unsigned int[MainBatched_VertexData::maxIndicesDataLength];
	std::fill(indices, indices + MainBatched_VertexData::maxIndicesDataLength, 0);

	batched_vb = VertexBuffer::MakeVertexBuffer(positions, MainBatched_VertexData::maxVerticesDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	batched_vb->SetLayout(layout);

	batched_ib = IndexBuffer::MakeIndexBuffer(indices, MainBatched_VertexData::maxIndicesDataLength);

	batched_va = VertexArray::MakeVertexArray();
	batched_va->AddVertexBuffer(batched_vb);
	batched_va->SetIndexBuffer(batched_ib);

	delete[] positions;
	delete[] indices;

	float* positions_instanced = new float[Bullet_VertexData::maxDataLength];
	std::fill(positions_instanced, positions_instanced + Bullet_VertexData::maxDataLength, 0);

	unsigned int* indices_instanced = new unsigned int[MainBatched_VertexData::maxIndicesDataLength]; //TODO: might want a better size
	std::fill(indices_instanced, indices_instanced + MainBatched_VertexData::maxIndicesDataLength, 0);

	instanced_vb_pos = VertexBuffer::MakeVertexBuffer(positions_instanced, Bullet_VertexData::maxDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_pos = {
		{ ShaderDataType::Float2, "a_Position" }
	};
	instanced_vb_pos->SetLayout(layout_instanced_pos);

	instanced_vb_color = VertexBuffer::MakeVertexBuffer(positions_instanced, Bullet_VertexData::maxDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_color = {
		{ ShaderDataType::Float4, "a_ColorInstanced", false, true }
	};
	instanced_vb_color->SetLayout(layout_instanced_color);

	instanced_vb_life = VertexBuffer::MakeVertexBuffer(positions_instanced, Bullet_VertexData::maxDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_life = {
		{ ShaderDataType::Float, "a_BulletLifeInstanced", false, true }
	};
	instanced_vb_life->SetLayout(layout_instanced_life);

	instanced_vb_mat = VertexBuffer::MakeVertexBuffer(positions_instanced, Bullet_VertexData::maxDataLength * sizeof(float), RenderingHints::stream_draw);
	VertexBufferLayout layout_instanced_mat = {
		{ ShaderDataType::Mat4, "a_ModelMatrixInstanced" }
	};
	instanced_vb_mat->SetLayout(layout_instanced_mat);

	instanced_ib = IndexBuffer::MakeIndexBuffer(indices_instanced, MainBatched_VertexData::maxIndicesDataLength);

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

void Renderer::SubmitBatchedDraw(const float* posAndColor, unsigned int posAndColorLength, const unsigned int* indices, unsigned int indicesLength) {
	if (currentSceneName == "") [[unlikely]] {
		//only happens for Diagnostics
		MainBatched_VertexData* tempVertexDataGroup = new MainBatched_VertexData();
		tempVertexDataGroup->m_vertices.insert(tempVertexDataGroup->m_vertices.end(), posAndColor, posAndColor + posAndColorLength);
		tempVertexDataGroup->m_indices.insert(tempVertexDataGroup->m_indices.end(), indices, indices + indicesLength);
		BatchedFlush(tempVertexDataGroup);
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

				if (posAndColorLength > MainBatched_VertexData::maxVerticesDataLength || indicesLength > MainBatched_VertexData::maxIndicesDataLength) [[unlikely]] {
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

void Renderer::BatchedFlush(const MainBatched_VertexData* drawData) {
	//TODO: check if there's nothing?

	batched_vb->modifyData(drawData->m_vertices.data(), drawData->m_vertices.size() * sizeof(float));
	batched_ib->modifyData(drawData->m_indices.data(), drawData->m_indices.size() * sizeof(unsigned int));

	glDrawElements(GL_TRIANGLES, batched_va->GetIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	//glDrawElements(GL_TRIANGLES, drawData->m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::BulletFlush(const Bullet_VertexData* drawData) {
	//TODO: check if there's nothing?

	instanced_vb_color->modifyData(drawData->m_colors.data(), drawData->m_colors.size() * sizeof(float));
	instanced_vb_life->modifyData(drawData->m_lifeValues.data(), drawData->m_lifeValues.size() * sizeof(float));
	instanced_vb_mat->modifyData(drawData->m_modelMatrices.data(), drawData->m_modelMatrices.size() * sizeof(float));

	const unsigned int bulletCount = drawData->m_lifeValues.size();
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(Bullet::instanced_indices)/sizeof(*Bullet::instanced_indices), GL_UNSIGNED_INT, nullptr, bulletCount);
}

void Renderer::BeginScene(const std::string& name) {
	currentSceneName = name;
	sceneList.push_back(name);
}

void Renderer::EndScene() {
	currentSceneName = "";
	//not sure what to put...
}

bool Renderer::isDebugDrawingEnabled(const std::string& name) {
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
