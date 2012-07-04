/*
 * floatmatrix.cpp --
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

#include <math.h>
#include "floatmatrix.h"

namespace phlib {

float_matrix::float_matrix(size_type rows, size_type cols)
{
  float_vector  v(cols, 0.0);
  while (rows--)
    push_back(v);
}

float_matrix::float_matrix(const float_matrix& m, float_matrix::size_type excluded_row, float_matrix::size_type excluded_column)
{
	for (float_matrix::size_type i = 0; i < m.rows(); i++)
		if (i != excluded_row) {
			float_vector v(m[i], excluded_column);
			push_back(v);
		}
}

float_matrix& float_matrix::operator=(const float_matrix& m)
{
  iterator  i;
  const_iterator  j;

  if (this->rows() != m.rows())
    this->rows(m.rows());
  if (this->columns() != m.columns())
    this->columns(m.columns());

  for (i = begin(), j = m.begin(); i != end(); i++, j++)
    *i = *j;

  return *this;
}

void float_matrix::rows(size_type r)
{
  while (size() > r)
    erase(end() - 1);

  if (size() < r) {
    float_vector  v(this->columns(), 0.0);
    while (size() < r)
      push_back(v);
  }
}

void float_matrix::columns(size_type cols)
{
  for (iterator i = begin(); i != end(); i++)
    i->resize(cols, 0.0);
}

void float_matrix::setColumn(float_matrix::size_type column_index, const float_vector& column)
{
	if (rows() == column.size()) {
		for (float_matrix::size_type i = 0; i < column.size(); i++)
			at(i, column_index) = column[i];
	}
}

float_matrix::element_type float_matrix::getMax() const
{
	register const_iterator src;
	element_type res = size() ? front().getMax() : 0.0, f;

	for (src = begin() + 1; src != end(); src++) {
		f = src->getMax();
		if (res < f)
			res = f;
	}

	return res;
}

float_matrix::element_type float_matrix::getMin() const
{
	register const_iterator src;
	element_type res = size() ? front().getMin() : 0.0, f;

	for (src = begin() + 1; src != end(); src++) {
		f = src->getMin();
		if (res > f)
			res = f;
	}

	return res;
}

float_matrix::element_type float_matrix::D() const
{
	if (rows() != columns() || !rows())
		return 0.0f;
	else if (1 == rows())
		return front().front();
	else if (2 == rows())
		return at(0, 0) * at(1, 1) - at(0, 1) * at(1, 0);
	else {
		float_matrix	temp(*this);
		return temp.calcD();
	}
}

float_matrix::element_type float_matrix::D(int column_index, const float_vector& column) const
{
	if (rows() != columns() || !rows() || rows() != column.size())
		return 0.0f;
	float_matrix temp(*this);
	temp.setColumn(column_index, column);
	return temp.calcD();
}

void float_matrix::setLowerBound(const element_type f)
{
	for (register iterator dest = begin(); dest != end(); dest++)
		dest->setLowerBound(f);
}

void float_matrix::setUpperBound(const element_type f)
{
	for (register iterator dest = begin(); dest != end(); dest++)
		dest->setUpperBound(f);
}

void float_matrix::normalize(element_type a0, element_type b0, element_type a1, element_type b1)
{
	register iterator src;

	for (src = begin(); src != end(); src++)
		src->normalize(a0, b0, a1, b1);
}

void float_matrix::interchangeRows(int row1, int row2)
{
	for (register int i = 0, c = columns(); i < c; i++)
		std::swap(at(row1, i), at(row2, i));
}

bool float_matrix::decompose(double& d)
{
	size_type	i, j, k, imax = 0;
	double	sum, aamax, dum;
	float_vector	v(rows());
	std::vector<size_type>	indx(rows(), 0);

	d = 1.0f;

	// loop over rows to get the implicit scalling information
	for (i = 0; i < rows(); i++) {
		aamax = 0.0f;

		for (j = 0; j < columns(); j++) {
			if (::fabs(at(i, j)) > aamax)
				aamax = at(i, j);
		}

		if (aamax == 0.0)
			return false;	//	singular matrix
							//	no nonzero largest element

		v[i] = 1.0f / aamax;	//	save the scaling
	}

	for (j = 0; j < columns(); j++) {	//	this is the loop over columns of Crout's method
		for (i = 0; i < j; i++) {	//	this is equation 2.3.12 except for "i = j"
			sum = at(i, j);

			for (k = 0; k < i; k++)
				sum -= at(i, k) * at(k, j);

			at(i, j) = sum;
		}

		aamax = 0.0;	//	initialize for the search for largest pivot element

		for (i = j; i < rows(); i++) {	//	this is "i = j" of equation 2.3.12
										//	and "i = j + 1 ... N" of equation 2.3.13
			sum = at(i, j);

			for (k = 0; k < j; k++)
				sum -= at(i, k) * at(k, j);

			at(i, j) = sum;

			dum = v[i] * ::fabs(sum);	//	figure of merit for the pivot
			if (dum >= aamax) {	//	is it better than the best so far?
				imax = i;
				aamax = dum;
			}
		}

		if (j != imax) {	//	do we need to interchange rows?
			interchangeRows(j, imax);	//	yes, do so...
			d = -d;	//	and change the parity of D
			v[imax] = v[j];	//	also interchange the scale factor
		}

		indx[j] = imax;

		if (0.0 == at(j, j))
			return false;	// matrix is singular

		if (j != columns() - 1) {	//	now, finally divide by the pivot element
			dum = 1.0f / at(j, j);	//	if the pivot element is zero the matrix is singular
									//	at least to the precision of the algorithm).
			for (i = j + 1; i < rows(); i++)
				at(i, j) *= dum;
		}
	}	//	go back to the next column in the reduction

	return true;
}

// matrix is damaged after calculation
float_matrix::element_type float_matrix::calcD()
{
	element_type	summ;

	if (decompose(summ)) {
		for (register float_matrix::size_type i = 0; i < rows(); i++)
			summ *= at(i, i);
	}
	else
		summ = 0.0f;	//	matrix is singular

	return summ;
}

}
