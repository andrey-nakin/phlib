/*
 * floatvector.h --
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

#ifndef	__MD_FLOATVECTOR_H_895897358634785645126457657
#define	__MD_FLOATVECTOR_H_895897358634785645126457657

#include	<vector>
#include	<iostream>

namespace phlib {

class float_vector : public std::vector<double> {
public:
	typedef value_type	element_type;

	inline float_vector() {}
	inline float_vector(size_type n) : std::vector<double>(n) {}
	inline float_vector(size_type n, const value_type& t) : std::vector<double>(n, t) {}
	inline float_vector(const float_vector& v) {
		*this = v;
	}
	float_vector(const float_vector& v, size_type excluded_index);

	float_vector& operator=(const float_vector&);
	float_vector& operator+=(const float_vector&);
	float_vector& operator-=(const float_vector&);
	float_vector& operator*=(element_type v);
	float_vector& operator/=(element_type v);

  void addMul(const float_vector&, element_type mult);
	void subDiv(const float_vector&, element_type divisor);

	void addSquared(const float_vector&);

	element_type getSumm() const;
	element_type getMax() const;
	element_type getMin() const;

	void setMin(const float_vector&);
	void setMax(const float_vector&);
	void setPikes(const float_vector&, element_type zero_value);

	void setLowerBound(const element_type);
	void setUpperBound(const element_type);

	void invert();	// vector[i] = 1.0 / vector[i] (except for zeros)

	void normalize(element_type a0, element_type b0, element_type a1, element_type b1);

  bool isZero() const;

	static std::ostream& write(std::ostream&, const float_vector& first, const float_vector& second);
};

class float_vector_stream {
	float_vector& vector;
	float_vector::iterator	current;

public:
	float_vector_stream(float_vector& v) : vector(v) {
		reset();
	}

	inline void reset() {
		current = vector.begin();
	}

	float_vector_stream& operator<<(float_vector::element_type x) {
		if (current != vector.end())
			*current++ = x;
		return *this;
	}
};

std::ostream& operator<<(std::ostream&, const float_vector&);

}

#endif	//	__MD_FLOATVECTOR_H_895897358634785645126457657
