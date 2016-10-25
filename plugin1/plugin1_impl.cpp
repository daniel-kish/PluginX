#include <string>
#include <iostream>

#include "../PluginX/plugin_ifc.h"
#include <boost/dll/alias.hpp>

namespace my_namespace {

	class my_plugin_sum : public plugin_ifc {
	public:
		my_plugin_sum(std::string loader_name) : plugin_ifc(loader_name) 
		{
			std::cout << "Constructing my_plugin_sum\n";
		}
		std::string name() const
		{
			return "sum plugin. Loaded by " + loader_name;
		}
		float calculate(float x, float y) 
		{
			return x + y;
		}
		~my_plugin_sum() {
			std::cout << "Destructing my_plugin_sum\n";
		}
		// Factory method
		static plugin_ifc* create(std::string loader_name) {
			return new my_plugin_sum(loader_name);
		}
	};
} // namespace my_namespace

BOOST_DLL_ALIAS(my_namespace::my_plugin_sum::create, create_plugin)