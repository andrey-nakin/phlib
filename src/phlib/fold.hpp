/*
 * fold.hpp --
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

/*
 * fold.hpp
 *
 * Folding operations on STL containers
 */

#ifndef FOLD_HPP_
#define FOLD_HPP_

#include <numeric>
#include <algorithm>

namespace phlib {

	template <class Result, class ForwardIterator, class Folder>
	Result foldl(ForwardIterator begin, const ForwardIterator end, Folder folder, const Result initial) {
		return std::accumulate(begin, end, initial, folder);
	}

	template <class Result, class Container, class Folder>
	Result foldl(const Container& c, Folder folder, const Result initial) {
		return std::accumulate(c.begin(), c.end(), initial, folder);
	}

	template <class Result, class ReverseIterator, class Folder>
	Result foldr(ReverseIterator begin, const ReverseIterator end, Folder folder, const Result initial) {
		return std::accumulate(begin, end, initial, folder);
	}

	template <class Result, class Container, class Folder>
	Result foldr(const Container& c, Folder folder, const Result initial) {
		return std::accumulate(c.rbegin(), c.rend(), initial, folder);
	}

}

#endif /* FOLD_HPP_ */
