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

HashMap<String, Tests::Context> Tests::contexts = HashMap<String, Context>();
HashMap<String, TestFunc> Tests::setup_functions = HashMap<String, TestFunc>();
HashMap<String, TestFunc> Tests::cleanup_functions = HashMap<String, TestFunc>();

int Tests::register_function(TestFunc p_function, FuncType p_type, String p_filter) {
	// Contexts are created automatically based on filter.
	if (!contexts.has(p_filter)) {
		Context ct;
		ct.runner = new doctest::Context;
		contexts[p_filter] = ct;
	}

	switch (p_type) {
		case FuncType::TYPE_SETUP: {
			// setup_functions[p_filter] = p_function;
			contexts[p_filter].setup.function = p_function;
			contexts[p_filter].setup.type = FuncType::TYPE_SETUP;
		} break;
		case FuncType::TYPE_CLEANUP: {
			// cleanup_functions[p_filter] = p_function;
			contexts[p_filter].cleanup.function = p_function;
			contexts[p_filter].cleanup.type = FuncType::TYPE_CLEANUP;
		} break;
	}
	return 0;
}

int Tests::run() {
	int result = 0;

	const String *k = nullptr;

	while ((k = contexts.next(k))) {
		Context &ct = contexts[*k];
		// Setup.
		if (ct.setup.function) {
			ct.setup.function();
		}

		// Run, set scope of test cases using the filter.
		ct.runner->clearFilters();
		String test_cases = vformat("*%s*", *k);
		ct.runner->addFilter("test-case", test_cases.utf8().get_data());
		result |= ct.runner->run();

		// Cleanup.
		if (ct.cleanup.function) {
			ct.cleanup.function();
		}
	}
	return result;
}
