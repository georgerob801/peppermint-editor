#include <editor/serialisation/ComponentParameterRegister.h>

using namespace editor::gui;

void ComponentParameterRegister::clearParameters() {
	this->parameters.clear();
}

std::vector<ComponentParameterRegister::ParameterData>::iterator ComponentParameterRegister::registerParameter(std::string name, void* location, ParameterTypes type, bool useMapping, bool clamp) {
	this->parameters.push_back(ParameterData(name, location, type, useMapping, clamp));
	return this->parameters.end() - 1;
}