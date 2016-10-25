#include <iostream>
#include <iomanip>
#include <string>
#include <boost\dll\import.hpp>
#include "plugin_ifc.h"
#include <boost/function.hpp>
#include <vector>

namespace dll = boost::dll;
namespace fs = boost::filesystem;

std::vector<fs::directory_entry>
list_files_in_dir(fs::path dir_path)
{
	std::vector<fs::directory_entry> entries;
	std::copy(fs::directory_iterator(dir_path), fs::directory_iterator{},
		std::back_inserter(entries));
	return entries;
}

void filter_dll(std::vector<fs::directory_entry>& entries)
{
	auto first_wrong = std::partition(begin(entries), end(entries),
		[](fs::directory_entry const& entry) {
		return (entry.path().extension() == fs::path{".dll"});
});
	entries.erase(first_wrong, end(entries));
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
	
	auto entries = list_files_in_dir(dir_path);
	filter_dll(entries);

	std::vector<dll::shared_library> libs; // each lib contains a plugin
	for (auto& entry : entries)
	{
		libs.emplace_back(entry.path());
		if ( !libs.back().is_loaded() ) {
			std::cerr << "can't load shared lib " << libs.back().location() << '\n';
		}
	}
	
	using plugin_ptr = std::unique_ptr<plugin_ifc>;
	using factory = plugin_ifc* (std::string);

	std::vector<plugin_ptr> plugins;
	for (auto& lib : libs)
	{
		factory* creator = lib.get_alias<factory>("create_plugin"s);
		plugin_ptr plugin(creator(app_name));
		if (!plugin)
			std::cerr << "unable to create an instance of plugin from " << lib.location() << '\n';
		plugins.push_back(std::move(plugin));
	}

	for (auto& plugin : plugins)
		std::cout << "name: " << plugin->name() << " : " << plugin->calculate(2.0, 3.0) << '\n';
}
catch (const fs::filesystem_error& er)
{
	// здесь должен находиться код анализа ошибки 
	// и принятия решения о дальнейших действиях
	std::cerr << "filesystem error occurred: " << er.what() << "\n exiting.";
	std::exit(1);
}