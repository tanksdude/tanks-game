#include "shader.h"

#include <stdexcept>

#include "../renderer.h"
#include "opengl-shader.h"

Shader* Shader::MakeShader(const std::string& vert_filepath, const std::string& frag_filepath) {
	AvailableRenderingContexts API = Renderer::GetContext();
	switch (API) {
		case AvailableRenderingContexts::OpenGL:
			return new OpenGLShader(vert_filepath, frag_filepath);
		case AvailableRenderingContexts::software:
			//return new SoftwareShader(vert_filepath, frag_filepath);
		case AvailableRenderingContexts::null_rendering:
			//return new NullShader(vert_filepath, frag_filepath);
		default:
			throw std::invalid_argument("ERROR: Unknown rendering API!");
	}
}
