/*
 * scoped_guard.hpp --
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

#ifndef SCOPED_GUARD_HPP_
#define SCOPED_GUARD_HPP_

namespace phlib {

template <typename T>
class scoped_guard {

	const T savedValue;
	T& variable;

	scoped_guard(const scoped_guard&);
	scoped_guard& operator=(const scoped_guard&);

public:

	scoped_guard(T& variable, const T newValue) :
		savedValue(variable),
		variable(variable) {
		variable = newValue;
	}

	~scoped_guard() {
		variable = savedValue;
	}

};

}

#endif /* SCOPED_GUARD_HPP_ */
