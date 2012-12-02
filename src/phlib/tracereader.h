/*
 * tracereader.h --
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

#ifndef  __MD_TRACEREADER_H_743743657364573465743657
#define __MD_TRACEREADER_H_743743657364573465743657

#include  <vector>
#include  <string>
#include	<iostream>
#include  "floatvector.h"
#include  "floatmatrix.h"

namespace phlib {

class TraceReader {
protected:
	enum {BufferSize = 1024 * 1024 * 10};

	typedef	std::vector<std::string>	TitleVector;

  bool  needDataLine;
	std::ios_base*	controlledStream;
	std::streamsize	maxPrecision;
	int	lineCounter;  // text line counter
  int dataLineCounter;  // data line counter (when needDataLine == true)
  TitleVector  dataTitles;
  float_vector  dataLine;
	bool newLine, firstLine;
  std::vector<std::string>  filenames;

  inline TraceReader() : needDataLine(false), controlledStream(0), maxPrecision(-1),
		  lineCounter(0), dataLineCounter(0), dataLine(0), newLine(false), firstLine(false) {}
  virtual ~TraceReader(){}

	virtual bool isZip(const char* filename);

public:
  void addName(const char*);

	void read(int index = -1);
	void read(int index_begin, int index_end);

	// trace thread in range [index_begin, index_end)
	void read(std::istream& src, int index_begin, int index_end);
	void read(const char* filename);
	void read(const char* filename, int index);
	void read(const char* filename, int index_begin, int index_end);
  void read(const std::vector<const char*>& filenames, int index);
  void read(const std::vector<std::string>& filenames, int index);
	void read(const std::vector<std::string>& filenames, int index_begin, int index_end);

  virtual void handle(int index, double value) {};
  virtual void handle(float_vector&) {};

  int getNameCount() const {
    return filenames.size();
  }

private:
	void addTitle(const char* title, int index);
};

class MatrixReader : public TraceReader {
	float_matrix&	matrix;

public:
	MatrixReader(float_matrix& dest) : matrix(dest) {
		needDataLine = true;
	};

protected:
  virtual void handle(float_vector&);
};

}

#endif  //  __MD_TRACEREADER_H_743743657364573465743657
