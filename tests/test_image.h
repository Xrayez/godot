/*************************************************************************/
/*  test_image.h                                                         */
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

#ifndef TEST_IMAGE_H
#define TEST_IMAGE_H

#include "core/io/image.h"

#include "tests/test_macros.h"

namespace TestImage {

TEST_CASE("[Stress][Image] Image fill by pixel 2048") {
	Ref<Image> image;
	image.instance();
	image->create(2048, 2048, false, Image::FORMAT_RGBA8);
	for (int y = 0; y < image->get_height(); ++y) {
		for (int x = 0; x < image->get_width(); ++x) {
			image->set_pixel(Point2(x, y), Color(1, 1, 1));
		}
	}
}

TEST_CASE("[Stress][Image] Image fill by pixel 4096") {
	Ref<Image> image;
	image.instance();
	image->create(4096, 4096, false, Image::FORMAT_RGBA8);
	for (int y = 0; y < image->get_height(); ++y) {
		for (int x = 0; x < image->get_width(); ++x) {
			image->set_pixel(Point2(x, y), Color(1, 1, 1));
		}
	}
}

} // namespace TestImage

#endif // TEST_IMAGE_H
