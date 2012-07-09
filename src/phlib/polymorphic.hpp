/*
 * polymorphic.hpp --
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

#ifndef POLYMORPHIC_H_
#define POLYMORPHIC_H_

namespace phlib {

	class Polymorphic {

		Polymorphic& operator=(const Polymorphic&);

	protected:

		Polymorphic() {}
		Polymorphic(const Polymorphic&) {}

	public:

		virtual ~Polymorphic()
		{}

	};

}

#endif /* POLYMORPHIC_H_ */
