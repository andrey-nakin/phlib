/*
 * pointer_iterator.hpp --
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

#ifndef POINTER_ITERATOR_HPP_
#define POINTER_ITERATOR_HPP_

#include <iterator>

namespace phlib {

	/**
	 * Class is used to iterate through containers of pointers
	 */
	template <class ValueType, class PointerType, class IteratorType>
	class PointerIterator : public std::iterator<
			std::bidirectional_iterator_tag,
			ValueType,
			ptrdiff_t,
			PointerType,
			typename IteratorType::reference> {

		IteratorType source;

	public:

		typedef typename IteratorType::difference_type difference_type;

		PointerIterator(IteratorType source) :
			source(source)
		{}

		PointerIterator(const PointerIterator& source) :
			source(source.source)
		{}

		inline PointerIterator& operator=(const PointerIterator i) {
			source = i.source;
			return *this;
		}

		inline PointerIterator& operator++() {
			++source;
			return *this;
		}

		inline PointerIterator operator++(int) {
			PointerIterator tmp(*this);
			source++;
			return tmp;
		}

		inline PointerIterator& operator--() {
			--source;
			return *this;
		}

		inline PointerIterator operator--(int) {
			PointerIterator tmp(*this);
			source--;
			return tmp;
		}

		inline ValueType& operator*() {
			return **source;
		}

		inline ValueType& operator*() const {
			return **source;
		}

		inline PointerType operator->() {
			return *source;
		}

		inline PointerType operator->() const {
			return *source;
		}

		inline bool operator==(const PointerIterator i) const {
			return source == i.source;
		}

		inline bool operator!=(const PointerIterator i) const {
			return source != i.source;
		}

		template <class Distance>
		inline PointerIterator operator+(const Distance& d) const {
			return PointerIterator(source + d);
		}

		template <class Distance>
		inline PointerIterator operator-(const Distance& d) const {
			return PointerIterator(source - d);
		}

		inline PointerIterator operator+(const difference_type& n) const {
			return PointerIterator(source + n);
		}

		inline PointerIterator operator-(const difference_type& n) const {
			return PointerIterator(source - n);
		}
	};
}

#endif /* POINTER_ITERATOR_HPP_ */
