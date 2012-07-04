/*
 * cloneable.hpp --
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

#ifndef CLONEABLE_HPP_
#define CLONEABLE_HPP_

#include "polymorphic.hpp"

namespace phlib {

	class Cloneable : Polymorphic  {
	protected:

		Cloneable() {}

		virtual Cloneable* doClone() const = 0;

	public:

		Cloneable* clone() const {
			return doClone();
		}
	};

}

#endif /* CLONEABLE_HPP_ */
