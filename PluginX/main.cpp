#include <iostream>
#include <iomanip>
#include <string>
#include <boost/dll/import.hpp>
#include "plugin_ifc.h"
#include <boost/function.hpp>
#include <vector>

namespace dll = boost::dll;
namespace fs = boost::filesystem;

std::vector<fs::path> list_files_in_dir(fs::path dir_path)
{
	std::vector<fs::path> entries;
	std::copy(fs::directory_iterator(dir_path), fs::directory_iterator{},
		std::back_inserter(entries));
	return entries;
}

bool is_dll_pred (fs::path const& p) 
{
	static const fs::path windows_dll_ext(".dll"); // for windows only
	return (p.extension() == windows_dll_ext);
};

void filter_dll(std::vector<fs::path>& entries)
{
	auto mid = std::partition(entries.begin(), entries.end(), is_dll_pred); // ret - first 'not_dll'
	entries.erase(mid, entries.end()); // erase from first 'not_dll' to the end
}


int main(int argc, char** argv)
try {
	using namespace std::string_literals;
	using std::begin;
	using std::end;

	const std::string app_name("MainApp");

	if (argc != 2) {
		std::cerr << "usage: " << app_name << ": app_name <path_to_plugins>\n";
		std::exit(1);
	}

	fs::path dir_path(argv[1]);
	if (!fs::exists(dir_path)) {
		std::cerr << "error: " << app_name << ": given path \n"
			<< dir_path << "\ndoes not exist!\n";
		std::exit(2);
	}
	if (!fs::is_directory(dir_path)) {
		std::cerr << "error: " << app_name << ": given path \n"
			<< dir_path << "\ndoes not point to a directory!\n";
		std::exit(3);
	}

	std::vector<fs::path> paths = list_files_in_dir(dir_path);
	filter_dll(paths);
	
	std::vector<dll::shared_library> libs; // each lib contains a plugin
	for (auto& path : paths)
	{
		dll::shared_library lib(path);
		if (!lib.is_loaded())
			std::cerr << "can't load shared lib " << libs.back().location() << '\n';
		libs.push_back(lib);
	}

	// function signature renaming (since C++11): for simplicity 
	using factory_fun = plugin_ifc* (std::string); 

	std::vector<plugin_ifc*> plugins;
	for (auto& lib : libs)
	{
		factory_fun* create_ptr = lib.get_alias<factory_fun>("create_plugin");
		plugin_ifc* plugin = create_ptr(app_name);
		if (!plugin)
			std::cerr << "unable to create an instance of plugin from " << lib.location() << '\n';
		plugins.push_back(plugin);
	}

	// using plugins: obtaining their names and using them to perform "calculations"
	for (plugin_ifc* plugin : plugins)
		std::cout << "name: " << plugin->name() << " : " << plugin->calculate(2.0, 3.0) << '\n';

	//cleanup
	for (auto plugin : plugins)
		delete plugin;
}
catch (const fs::filesystem_error& er)
{
	std::cerr << "filesystem error occurred: " << er.what() << "\n exiting.";
	std::exit(1);
}