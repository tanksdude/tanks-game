#include "background-rect.h"
#include "renderer.h"
#include "constants.h"

ColorValueHolder BackgroundRect::backColor = ColorValueHolder(0xDD/255.0, 0xDD/255.0, 0xDD/255.0);
//ColorValueHolder BackgroundRect::backColor = ColorValueHolder(0.875f, 0.875f, 0.875f);

ColorValueHolder BackgroundRect::getBackColor() {
	return backColor;
}

VertexArray* BackgroundRect::va;
VertexBuffer* BackgroundRect::vb;
IndexBuffer* BackgroundRect::ib;
bool BackgroundRect::initialized_GPU = false;

bool BackgroundRect::initializeGPU() {
	if (initialized_GPU) {
		return false;
	}

	float background_positions[] = {
		0, 0,   0xDD/255.0, 0xDD/255.0, 0xDD/255.0, 1.0,
		1, 0,   0xDD/255.0, 0xDD/255.0, 0xDD/255.0, 1.0,
		1, 1,   0xDD/255.0, 0xDD/255.0, 0xDD/255.0, 1.0,
		0, 1,   0xDD/255.0, 0xDD/255.0, 0xDD/255.0, 1.0
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	vb = VertexBuffer::MakeVertexBuffer(background_positions, sizeof(background_positions), RenderingHints::static_draw);
	VertexBufferLayout layout = {
		{ ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	};
	vb->SetLayout(layout);

	ib = IndexBuffer::MakeIndexBuffer(background_indices, 6);

	va = VertexArray::MakeVertexArray();
	va->AddVertexBuffer(vb);
	va->SetIndexBuffer(ib);

	initialized_GPU = true;
	return true;
}

bool BackgroundRect::uninitializeGPU() {
	if (!initialized_GPU) {
		return false;
	}

	delete vb;
	delete va;
	delete ib;

	initialized_GPU = false;
	return true;
}

/*
void BackgroundRect::drawCPU() {
	glColor3f(backColor.getRf(), backColor.getGf(), backColor.getBf());
	glBegin(GL_POLYGON);
	glVertex3f(0, 0, 0);
	glVertex3f(GAME_WIDTH, 0, 0);
	glVertex3f(GAME_WIDTH, GAME_HEIGHT, 0);
	glVertex3f(0, GAME_HEIGHT, 0);
	glEnd();
}
*/

void BackgroundRect::draw() {
	Shader* shader = Renderer::getShader("main");
	va->Bind();
	shader->setUniformMat4f("u_ModelMatrix", Renderer::GenerateModelMatrix(GAME_WIDTH, GAME_HEIGHT, 0, 0, 0));

	Renderer::Draw(*va, *ib, *shader);

	/*
	ColorValueHolder color = BackgroundRect::getBackColor();

	float background_positions[] = {
		0,          0,             color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		GAME_WIDTH, 0,             color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		GAME_WIDTH, GAME_HEIGHT,   color.getRf(), color.getGf(), color.getBf(), color.getAf(),
		0,          GAME_HEIGHT,   color.getRf(), color.getGf(), color.getBf(), color.getAf()
	};
	unsigned int background_indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	Renderer::SubmitBatchedDraw(background_positions, 4 * (2+4), background_indices, 2 * 3);
	*/
}
