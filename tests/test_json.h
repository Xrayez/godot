/*************************************************************************/
/*  test_json.h                                                          */
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

#ifndef TEST_JSON_H
#define TEST_JSON_H

#include "core/io/json.h"

#include "thirdparty/doctest/doctest.h"

namespace TestJSON {

TEST_SUITE("[IO][JSON] JSON issues") {
	// https://github.com/godotengine/godot/issues/40794
	TEST_CASE("Issue #40676") {
		// Correct one: "{ \"a\":12345,\"b\":12345 }";
		String json = "\"a\":12345,\"b\":12345 }"; // Missing start bracket.
		Variant parsed;
		String err;
		int line;

		JSON::parse(json, parsed, err, line);

		const Variant::Type parsed_type = parsed.get_type();

		CHECK_MESSAGE(parsed_type != Variant::STRING, "Should not prematurely parse as string.");
		REQUIRE_MESSAGE(parsed_type == Variant::DICTIONARY, "Parsed JSON should represent a dictionary");

		Dictionary result = parsed;

		const bool has_a = result.has("a");
		CHECK(has_a);

		const bool has_b = result.has("b");
		CHECK(has_b);
	}
}

} // namespace TestJSON

#endif // TEST_JSON_H
