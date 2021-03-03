#ifndef MDXX_MDXX_MANAGER_H
#define MDXX_MDXX_MANAGER_H
#include "expansion.h"
#include "context.h"
#include "line_data.h"
#include <unordered_map>
#include <memory>
#include <fstream>

namespace mdxx {

typedef std::shared_ptr<std::unordered_map<std::string, std::unique_ptr<Context>>> context_dict_type;
typedef std::shared_ptr<std::unordered_map<std::string, gen_func>> imported_function_dict_type;

class MDXX_Manager {
public:
	MDXX_Manager(std::ifstream& i);
	MDXX_Manager(std::ifstream&& i);
	MDXX_Manager(std::string filename);
	void init_dictionaries();
	void print_expansion_flip();
	const std::vector<std::string>& context_stack() const;
	std::unique_ptr<Context>& cur_context();
	Expansion_Base* get_var(std::string variable);
	std::string next_line();
	std::string next_line_no_nl();
	std::string print_line();
	void handle_context(HTML_Manager& html);
	void find_next_content_line();
	std::string find_and_return_next_content_line();
	std::string expand_line(std::string & line);
	void set_context(std::vector<std::string> new_context);
	bool at_end_of_file();
	template<typename T>
	void add_variable_to_context(const char * context, const char * variable_name, T value);
	template<typename T>
	void add_variable_to_context(const char * context, const char * variable_name, T* value);
	void add_variable_to_context(const char * context, const char * variable_name, gen_func value);
	void add_variable_to_context(const char * context, const char * variable_name, const char * value);
	template<typename T>
	void add_new_context(const char * name);
	void add_raw_context(const char * name, std::unique_ptr<Context> & context);
	std::string list_all_vars();
	std::string list_context_stack();
	std::string list_valid_contexts();
	std::string list_imported_functions();
	~MDXX_Manager();
	static char * open_context(Expansion_Base** args, size_t argc);
	static char * close_context(Expansion_Base** args, size_t argc);
	void destroy_contexts();
	context_dict_type get_context_dict();
	void set_context_dict(context_dict_type other_context_dict);
	imported_function_dict_type get_imported_function_dict();
	void set_imported_function_dict(imported_function_dict_type other_imported_function_dict);
public:
	std::string mdxx_object_id;
private:
	void variable_definition(std::string& line);
	//void function_import(std::string& line);
	void immediate_substitution(std::string& line);
	void check_variable_dependency(const Context& c);

	std::string find_context_with_variable(const std::string& var);
	void throw_exception_if_context_not_found(const std::string& context);
	void check_if_imported_function_found(const std::string& function);
	void handle_curly_braces(std::string& line);
	long convert_string_to_long(const std::string& str);
	void check_if_index_in_range(long index, size_t size);
	void handle_range_substitutions(std::string& line, const std::vector<std::string>& num_args);
private:
	std::ifstream* in_if_need_to_allocate = nullptr;
	std::ifstream& in;
	std::vector<std::string> context = { "default" };
	size_t line_number = 0;
	size_t cur_line_count = 0;
	bool print_expansion = false;
	std::shared_ptr<std::unordered_map<std::string, std::unique_ptr<Context>>> context_dict;
	std::shared_ptr<std::unordered_map<std::string, gen_func>> imported_function_dict;
	std::string line_stack;
	Line_Data line_data;
	bool finished_reading = false;
};

template<typename T>
void MDXX_Manager::add_variable_to_context(const char * context, const char * variable_name, T value) {
	throw_exception_if_context_not_found(std::string(context));
	(*context_dict)[context]->add_variable(variable_name, new Expansion<T>(value));
}

template<typename T>
void MDXX_Manager::add_variable_to_context(const char * context, const char * variable_name, T* value) {
	throw_exception_if_context_not_found(std::string(context));
	(*context_dict)[std::string(context)]->add_variable(variable_name, new Expansion<T*>(value));
}

template<typename T>
void MDXX_Manager::add_new_context(const char * name) {
	(*context_dict)[std::string(name)] = std::make_unique<T>(name);
}
template<>
const char * Expansion<MDXX_Manager*>::to_string();

}

extern "C" DLL_IMPORT_EXPORT void MDXX_add_new_context(mdxx::MDXX_Manager * mdxx, const char * name, mdxx::Context * context);
#endif