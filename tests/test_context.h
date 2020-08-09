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
		doctest::Context *runner = nullptr;
		FuncData cleanup;
	};

private:
	static HashMap<String, TestFunc> setup_functions; // key: test case filter.
	static HashMap<String, TestFunc> cleanup_functions; // key: test case filter.
	static HashMap<String, Context> contexts; // key: test case filter.

public:
	// Context.
	static void add_context(Context p_context, String p_filter) {
		contexts[p_filter] = p_context;
	}
	static HashMap<String, Context> &get_contexts() { return contexts; }
	static void clear_contexts() { contexts.clear(); }

	// Needed by test macros for dynamic initialization.
	static int register_function(TestFunc p_function, FuncType p_type, String p_filter);

	// Run all tests.
	static int run();
};

#endif // TEST_CONTEXT_H
