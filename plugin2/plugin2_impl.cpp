#include <string>
#include <iomanip>
#include <iostream>

#include "../PluginX/plugin_ifc.h"
#include <boost/dll/alias.hpp>

namespace my_namespace {

	class plugin_prod : public plugin_ifc {
	public:
		plugin_prod(std::string loader_name) : plugin_ifc(loader_name) 
		{
			std::cout << "Constructing plugin_prod\n";
		}
		std::string name() const
		{
			return "prod plugin. Loaded by \"" + loader_name + "\"";
		}
		float calculate(float x, float y) 
		{
			return x * y;
		}
		~plugin_prod() {
			std::cout << "Destructing plugin_prod\n";
		}
		// Factory method
		static plugin_ifc* create(std::string loader_name) {
			return new plugin_prod(loader_name);
		}
	};
} // namespace my_namespace

BOOST_DLL_ALIAS(my_namespace::plugin_prod::create, create_plugin)