/*************************************************************************/
/*  test_context.cpp                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "tests/test_context.h"
#include "core/variant.h" // For vformat().

Map<String, Tests::Context> *Tests::contexts = nullptr;
char **Tests::args = nullptr; // From command line.
int Tests::arg_count = 0;

void Tests::handle_cmdline_args(int argc, char *argv[]) {
	List<String> valid_arguments;

	// Clean arguments of "--test" from the args.
	for (int i = 0; i < argc; i++) {
		if (strncmp(argv[i], "--test", 6) != 0) {
			valid_arguments.push_back(String(argv[i]));
			arg_count++;
		}
	}
	// Convert Godot command line arguments back to standard arguments.
	args = new char *[arg_count];
	for (int i = 0; i < arg_count; ++i) {
		// Operation to convert Godot string to non wchar string.
		CharString cs = valid_arguments[i].utf8();
		const char *str = cs.get_data();
		// Allocate the string copy.
		args[i] = new char[strlen(str) + 1];
		// Copy this into memory.
		std::memcpy(args[i], str, strlen(str) + 1);
	}
}

void Tests::cleanup() {
	for (int i = 0; i < arg_count; ++i) {
		delete[] args[i];
	}
	delete[] args;
}

void Tests::add_context(const Context &p_context, String p_filter) {
	if (!contexts) {
		contexts = new Map<String, Context>;
	}
	(*contexts)[p_filter] = p_context;
}

void Tests::clear_contexts() {
	if (!contexts) {
		return;
	}
	contexts->clear();
}

int Tests::register_context_function(TestFunc p_function, FuncType p_type, String p_filter) {
	if (!contexts) {
		contexts = new Map<String, Context>;
	}
	// Contexts are created automatically per filter.
	if (!contexts->has(p_filter)) {
		Context ct;
		ct.filter = p_filter;
		(*contexts)[p_filter] = ct;
	}
	switch (p_type) {
		case FuncType::TYPE_SETUP: {
			(*contexts)[p_filter].setup.function = p_function;
			(*contexts)[p_filter].setup.type = FuncType::TYPE_SETUP;
		} break;
		case FuncType::TYPE_CLEANUP: {
			(*contexts)[p_filter].cleanup.function = p_function;
			(*contexts)[p_filter].cleanup.type = FuncType::TYPE_CLEANUP;
		} break;
	}
	return 0;
}

int Tests::run() {
	int result = 0;

	// STATE 1: Run tests without a context filter first (core).
	Vector<Context> has_filter;
	for (Map<String, Context>::Element *E = contexts->front(); E; E = E->next()) {
		if (!E->get().filter.empty()) {
			has_filter.push_back(E->get());
		}
	}
	String exclude;
	for (int i = 0; i < has_filter.size(); ++i) {
		exclude += vformat("*%s*", has_filter[i].filter);
		if (i < has_filter.size() - 1) {
			exclude += ",";
		}
	}
	doctest::Context doctest_core;
	doctest_core.addFilter("test-case-exclude", exclude.utf8().get_data());
	// Setup default options.
	doctest_core.setOption("order-by", "name");
	doctest_core.setOption("abort-after", 5);
	doctest_core.setOption("no-breaks", true);
	// Always allow to override all of the above.
	doctest_core.applyCommandLine(arg_count, args);
	// Run test cases.
	result |= doctest_core.run();

	// STAGE 2: Run tests with a context filter (integration tests etc.)
	for (Map<String, Context>::Element *E = contexts->front(); E; E = E->next()) {
		Context &ct = E->get();
		if (ct.filter.empty()) {
			continue;
		}
		// Setup.
		if (ct.setup.function) {
			ct.setup.function();
		}
		doctest::Context doctest;
		// Set scope of test cases using the filter.
		String test_cases = vformat("*%s*", E->key());
		doctest.addFilter("test-case", test_cases.utf8().get_data());
		// Setup default options.
		doctest.setOption("order-by", "name");
		doctest.setOption("abort-after", 5);
		doctest.setOption("no-breaks", true);
		// Always allow to override all of the above.
		doctest.applyCommandLine(arg_count, args);
		// Run test cases.
		result |= doctest.run();

		// Cleanup.
		if (ct.cleanup.function) {
			ct.cleanup.function();
		}
	}
	return result;
}
