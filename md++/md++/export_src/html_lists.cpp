#include "html_lists.h"
#include "html_manager.h"

namespace mdxx {

HTML_List::HTML_List(const char * n) :
	name(n),
	list_first_element(true)
{}

void HTML_List::open(HTML_Manager& html, const char * args) {
	(void)args;
	std::string opening = std::string("<") + name + ">";
	html.add(opening);
	html.push();
	list_first_element = true;
}

void HTML_List::process(HTML_Manager& html, const char * line_ptr, size_t num_lines) {
	if (list_first_element) {
		html.add_no_nl("<li>");
		html.add_pre(line_ptr);
		html.push();
		list_first_element = false;
		return;
	}
	if (num_lines > 1) {
		html.pop();
		html.add("</li>");
		html.add_no_nl("<li>");
		html.add_pre(line_ptr);
		html.push();
	} else {
		html.add(line_ptr);
	}
}

void HTML_List::close(HTML_Manager& html) {
	html.pop();
	html.add("</li>");
	html.pop();
	html.add_no_nl("</");
	html.write(name);
	html.add_pre(">");
}

HTML_List::~HTML_List() {}

MDXX_CONTEXT_COMMON_FUNCTIONALITY_DEFINITION(HTML_List)

}