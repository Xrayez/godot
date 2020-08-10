/*************************************************************************/
/*  test_context.h                                                       */
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

#ifndef TEST_CONTEXT_H
#define TEST_CONTEXT_H

#include "core/hash_map.h"
#include "core/map.h"
#include "core/ustring.h"

#include "thirdparty/doctest/doctest.h"

typedef void (*TestFunc)();

class Tests {
public:
	enum FuncType {
		TYPE_SETUP,
		TYPE_CLEANUP,
	};
	struct FuncData {
		TestFunc function = nullptr;
		FuncType type;
		String filter;
	};
	struct Context {
		FuncData setup;
		String filter;
		FuncData cleanup;
	};

private:
	static Map<String, Context> *contexts; // Key: test case filter.
	static char **args; // From command line.
	static int arg_count;

public:
	static void handle_cmdline_args(int argc, char *argv[]);

	// Context.
	static void add_context(const Context &p_context, String p_filter = "");
	static Map<String, Context> *get_contexts() { return contexts; }
	static void clear_contexts();

	// Needed by test macros for dynamic initialization
	// of setup and cleanup functions for the test runner.
	static int register_context_function(TestFunc p_function, FuncType p_type, String p_filter);

	// Run all tests.
	static int run();

	static void cleanup();
};

#endif // TEST_CONTEXT_H
