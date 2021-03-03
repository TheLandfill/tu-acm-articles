#ifndef MDXX_PLUGIN_LOADER_H
#define MDXX_PLUGIN_LOADER_H
#include "expansion.h"
#include "variable_map_definition.h"
#include "dll_info.h"
#include "mdxx_manager.h"
#include <memory>
#include <vector>
#include <string>

namespace mdxx {

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <Windows.h>
	#define MDXX_SHARED_HANDLE_TYPE HINSTANCE
#else
	#include <dlfcn.h>
	#define MDXX_SHARED_HANDLE_TYPE void *
#endif

class Plugin_Loader {
public:
	Plugin_Loader();
	// Should have a shared library with functions/contexts and should use either
	// 		template<typename T>
	// 		void MDXX_Manager::add_new_context(const std::string name)
	// or
	// 		template<typename T>
	// 		void MDXX_Manager::add_new_context(const std::string name, variable_map variables)
	// to add a new context and
	// 		void MDXX_Manager::add_function(const std::string name, gen_func function)
	// to add a new function to a shared dictionary (i.e. unordered_map) that
	// all MDXX_Managers can use. Use the first one if you want to create an
	// empty context and add variables later or the second one if you already
	// have the variables ready to use.
	void load_plugin(MDXX_Manager * mdxx, const char * shared_libary_name);
	void set_plugin_dir(const std::string& pd);
	std::string get_plugin_dir();
	variable_map * get_variable_map(void * id);
	void free_variable_map(void * id);
	~Plugin_Loader();
public:
	std::string plugin_loader_obj_id;
private:
	std::string plugin_dir;
	std::vector<MDXX_SHARED_HANDLE_TYPE> plugins;
	std::unordered_map<void *, variable_map*> plugin_variable_maps;
};

template<>
const char * Expansion<Plugin_Loader*>::to_string();

extern "C" DLL_IMPORT_EXPORT char * MDXX_set_plugin_dir(Expansion_Base** args, size_t argc);
extern "C" DLL_IMPORT_EXPORT char * MDXX_load_plugins(Expansion_Base** args, size_t argc);

}
#endif