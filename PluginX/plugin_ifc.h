#pragma once

#include <string>

class plugin_ifc {
public:
	virtual std::string name() const = 0;
	virtual float calculate(float x, float y) = 0;

	virtual ~plugin_ifc() {}
};