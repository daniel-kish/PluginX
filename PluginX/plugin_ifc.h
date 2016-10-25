#pragma once

#include <string>

class plugin_ifc {
protected:
	std::string loader_name;
public:
	plugin_ifc(std::string name) : loader_name(name)
	{}
	virtual std::string name() const = 0;
	virtual float calculate(float x, float y) = 0;

	virtual ~plugin_ifc() {}
};