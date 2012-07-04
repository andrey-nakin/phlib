/*
 * point3d.h --
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

#ifndef POINT3D_H_
#define POINT3D_H_

namespace phlib {

	class Point3d {
	public:
		double x, y, z;

		Point3d() : x(0.0), y(0.0), z(0.0) {}
		Point3d(const double x, const double y, const double z) : x(x), y(y), z(z) {}
		Point3d(const Point3d& src) : x(src.x), y(src.y), z(src.z) {}

		Point3d& operator=(const Point3d& src) {
			this->x = src.x;
			this->y = src.y;
			this->z = src.z;
			return *this;
		}

	};

}

#endif /* POINT3D_H_ */
