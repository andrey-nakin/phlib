/*
 * minmax.hpp --
 *
 * This file is part of phlib library.
 *
 * Copyright (c) 2012 Andrey V. Nakin <andrey.nakin@gmail.com>
 * All rights reserved.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#ifndef MINMAX_HPP_
#define MINMAX_HPP_

#include <algorithm>

namespace phlib {

	template <class T>
	const T& min(const T& a, const T& b, const T& c) {
		return std::min(a, std::min(b, c));
	}

	template <class T>
	const T& max(const T& a, const T& b, const T& c) {
		return std::max(a, std::max(b, c));
	}
}

#endif /* MINMAX_HPP_ */
