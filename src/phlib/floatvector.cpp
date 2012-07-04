/*
 * floatvector.cpp --
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

#include	"floatvector.h"
#include	"memory.h"
#include	<iterator>
#include	<math.h>

namespace phlib {

float_vector::float_vector(const float_vector& v, float_vector::size_type excluded_index)
{
	for (size_type i = 0; i < v.size(); i++)
		if (i != excluded_index)
			push_back(v[i]);
}

float_vector& float_vector::operator=(const float_vector& src)
{
	register size_type	sz = size();
	if (sz != src.size())
		this->resize(sz = src.size());
	::memcpy(&(*(this->begin())), &(*(src.begin())), sz * sizeof(value_type));
	return *this;
}

float_vector& float_vector::operator+=(const float_vector& v)
{
	register iterator i;
	register const_iterator v_i;
	register const_iterator v_last;

	if (size() < v.size())
		resize(v.size(), 0.0);

	for (i = begin(), v_i = v.begin(), v_last = v.end(); v_i < v_last; )
		*i++ += *v_i++;

	return *this;
}

float_vector& float_vector::operator-=(const float_vector& v)
{
	register iterator i;
	register const_iterator v_i;
	register const_iterator v_last;

	if (size() < v.size())
		resize(v.size(), 0.0);

	for (i = begin(), v_i = v.begin(), v_last = v.end(); v_i < v_last; )
		*i++ -= *v_i++;

	return *this;
}

float_vector& float_vector::operator*=(element_type v)
{
	register iterator i;
	register const_iterator last;

	for (i = begin(), last = end(); i < last; )
		*i++ *= v;

	return *this;
}

float_vector& float_vector::operator/=(element_type v)
{
	if (v != 0.0)
		*this *= 1.0 / v;
	return *this;
}

void float_vector::addMul(const float_vector& v, element_type mult)
{
	register iterator i;
	register const_iterator v_i;
	register const_iterator v_last;

	if (size() < v.size())
		resize(v.size(), 0.0);

	for (i = begin(), v_i = v.begin(), v_last = v.end(); v_i < v_last; )
		*i++ += *v_i++ * mult;
}

void float_vector::subDiv(const float_vector& v, element_type divisor)
{
	register iterator i;
	register const_iterator v_i;
	register const_iterator v_last;

	if (size() < v.size())
		resize(v.size(), 0.0);

	if (divisor == 0.0)
		divisor = 1.0;

	for (i = begin(), v_i = v.begin(), v_last = v.end(); v_i < v_last; ) {
		*i -= *v_i++;
		*i++ /= divisor;
	}
}

void float_vector::addSquared(const float_vector& v)
{
	register iterator i;
	register const_iterator v_i;
	register const_iterator v_last;

	if (size() < v.size())
		resize(v.size(), 0.0);

	for (i = begin(), v_i = v.begin(), v_last = v.end(); v_i < v_last; ) {
		const double v = *v_i++;
		*i++ += v * v;
	}
}

float_vector::element_type float_vector::getSumm() const
{
	element_type res = 0.0;

	for (register const_iterator i = begin(); i != end(); i++)
		res += *i;

	return res;
}

float_vector::element_type float_vector::getMax() const
{
	register const_iterator src;
	element_type res = size() ? front() : 0.0;

	for (src = begin() + 1; src != end(); src++)
		if (res < *src)
			res = *src;

	return res;
}

float_vector::element_type float_vector::getMin() const
{
	register const_iterator src;
	element_type res = size() ? front() : 0.0;

	for (src = begin() + 1; src != end(); src++)
		if (res > *src)
			res = *src;

	return res;
}

void float_vector::setMin(const float_vector& v)
{
	register const_iterator src;
	register iterator dest;

	for (src = v.begin(), dest = begin(); src != v.end(); src++)
		if (end() == dest) {
			push_back(*src);
			dest = end();
		}
		else {
			if (*src < *dest)
				*dest = *src;
			dest++;
		}
}

void float_vector::setMax(const float_vector& v)
{
	register const_iterator src;
	register iterator dest;

	for (src = v.begin(), dest = begin(); src != v.end(); src++)
		if (end() == dest) {
			push_back(*src);
			dest = end();
		}
		else {
			if (*src > *dest)
				*dest = *src;
			dest++;
		}
}

void float_vector::setPikes(const float_vector& v, element_type zero_value)
{
	register const_iterator src;
	register iterator dest;

	for (src = v.begin(), dest = begin(); src != v.end(); src++)
		if (end() == dest) {
			push_back(*src);
			dest = end();
		}
		else {
			if (::fabs(*src - zero_value) > ::fabs(*dest - zero_value))
				*dest = *src;
			dest++;
		}
}

void float_vector::setLowerBound(const element_type f)
{
	for (register iterator dest = begin(); dest != end(); dest++)
		if (*dest < f)
			*dest = f;
}

void float_vector::setUpperBound(const element_type f)
{
	for (register iterator dest = begin(); dest != end(); dest++)
		if (*dest > f)
			*dest = f;
}

void float_vector::invert()
{
	for (register iterator i = begin(); i != end(); i++)
		if (*i != 0.0)
			*i = 1.0 / *i;
}

void float_vector::normalize(element_type a0, element_type b0, element_type a1, element_type b1)
{
	element_type div = b0 - a0;
	if (div == 0.0)
		return;	// bad initial range

	element_type mul = (b1 - a1) / div;
	register iterator src;

	for (src = begin(); src != end(); src++)
		*src = (*src - a0) * mul + a1;
}

bool float_vector::isZero() const
{
  for (register const_iterator i = begin(), e = end(); i != e; i++)
    if (*i != 0.0)
      return false;
  return true;
}

std::ostream& float_vector::write(std::ostream& s, const float_vector& first, const float_vector& second)
{
	float_vector::const_iterator first_i = first.begin(), first_last = first.end();
	float_vector::const_iterator second_i = second.begin(), second_last = second.end();

	for (; first_i != first_last || second_i != second_last; ) {
		if (first_i != first_last)
			s << *first_i++ << "\t\t";
		else 
			s << "\t\t";

		if (second_i != second_last)
			s << *second_i++;

		s << '\n';
	}

	return	s;
}

std::ostream& operator<<(std::ostream& s, const float_vector& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<float_vector::element_type>(s, "\n"));
	return s;
}

}
