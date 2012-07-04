/*
 * floatmatrix.h --
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

#ifndef	__MD_FLOATMATRIX_H_647357326573656432756347564375643
#define	__MD_FLOATMATRIX_H_647357326573656432756347564375643

#include  "floatvector.h"

namespace phlib {

class float_matrix : public std::vector<float_vector> {
public:
	typedef value_type::value_type	element_type;

	inline float_matrix() {}
  float_matrix(size_type rows, size_type cols);
  inline float_matrix(const float_matrix& m) {
    *this = m;
  }
  float_matrix(const float_matrix& m, size_type excluded_row, size_type excluded_column);

  float_matrix& operator=(const float_matrix&);

	inline void add(float_vector& v) {
		push_back(v);
	}

  inline size_type rows() const {
    return size();
  }
  void rows(size_type r);
  inline size_type columns() const {
	  return empty() ? 0 : front().size();
  }
  void columns(size_type cols);

  inline element_type& at(size_type row, size_type col) {
    return  (*this)[row][col];
  }
  inline const element_type& at(size_type row, size_type col) const {
    return  (*this)[row][col];
  }

	void setColumn(size_type column_index, const float_vector& column);

	element_type getMax() const;
	element_type getMin() const;

	element_type D() const;
	element_type D(int column_index, const float_vector& column) const;

	void setLowerBound(const element_type);
	void setUpperBound(const element_type);

	void normalize(element_type a0, element_type b0, element_type a1, element_type b1);
	void interchangeRows(int row1, int row2);

protected:
	bool decompose(double& d);
	element_type calcD();
};

class float_matrix_stream {
	float_matrix& matrix;
	float_matrix::iterator current_row;
	float_vector::iterator current_column;

public:
	float_matrix_stream(float_matrix& m) : matrix(m) {
		reset();
	}

	inline void reset() {
		if ((current_row = matrix.begin()) != matrix.end())
			current_column = current_row->begin();
	}

	float_matrix_stream& operator<<(float_matrix::element_type x)
	{
		if (current_row == matrix.end())
			return *this;

		if (current_column == current_row->end()) {
			if (++current_row == matrix.end())
				return *this;
			current_column = current_row->begin();
		}
		
		*current_column++ = x;

		return *this;
	}
};

}

#endif  //  __MD_FLOATMATRIX_H_647357326573656432756347564375643
