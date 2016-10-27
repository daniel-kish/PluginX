#include <string>
#include <iostream>

#include "../PluginX/plugin_ifc.h"
#include <boost/dll/alias.hpp>

namespace my_namespace {

	class plugin_sum : public plugin_ifc {
	public:
		plugin_sum(std::string loader_name) : plugin_ifc(loader_name) 
		{
			std::cout << "Constructing plugin_sum\n";
		}
		std::string name() const
		{
			return "sum plugin. Loaded by *" + loader_name + "*";
		}
		float calculate(float x, float y) 
		{
			return x + y;
		}
		~plugin_sum() {
			std::cout << "Destructing plugin_sum\n";
		}
		// Factory method
		static plugin_ifc* create(std::string loader_name) {
			return new plugin_sum(loader_name);
		}
	};
} // namespace my_namespace

BOOST_DLL_ALIAS(my_namespace::plugin_sum::create, create_plugin)