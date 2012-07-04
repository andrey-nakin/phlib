/*
 * cloneable_vector.hpp --
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

#ifndef CLONEABLE_VECTOR_HPP_
#define CLONEABLE_VECTOR_HPP_

#include <vector>
#include <iterator>

namespace phlib {

	template <class Type, class Cloner>
	class CloneableVector : public std::vector<Type> {

		class my_back_insert_iterator : public std::back_insert_iterator<CloneableVector> {

			Cloner cloner;

		public:

			explicit my_back_insert_iterator(CloneableVector& container, Cloner& cloner) :
				std::back_insert_iterator<CloneableVector>(container),
				cloner(cloner)
			{}

			my_back_insert_iterator& operator=(typename CloneableVector::const_reference value) {
				std::back_insert_iterator<CloneableVector>::operator=(cloner(value));
				return *this;
			}

		};

		CloneableVector& operator=(const CloneableVector&);
		CloneableVector(const CloneableVector& src);

	public:

		CloneableVector()
		{}

		CloneableVector(const CloneableVector& src, Cloner cloner) {
			std::copy(src.begin(), src.end(), *(new my_back_insert_iterator(*this, cloner)));
		}
	};

}

#endif /* CLONEABLE_VECTOR_HPP_ */
