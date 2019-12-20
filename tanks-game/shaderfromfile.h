#pragma once
#include <string>

class ShaderFromFile {
public:
	//it's up to the user to keep track of whether the shaders are vertex or fragment
	static std::string parse(const std::string& filepath);
};