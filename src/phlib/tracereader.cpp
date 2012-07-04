/*
 * tracereader.cpp --
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

#include  "tracereader.h"
#include  <fstream>
#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#ifndef	_MSC_VER
#include  <ext/stdio_filebuf.h>
#endif
#include  <errno.h>
#include <memory>

namespace phlib {

#define	STDIN_FILENAME	((const char*) "-")

///////////////////////////////////////////
//
// TraceReader members
//
///////////////////////////////////////////

bool TraceReader::isZip(const char* filename)
{
	const char	*p = strrchr(filename, '.');
	return 0 != p && (0 == strcmp(p, ".zip") || 0 == strcmp(p, ".ZIP"));
}

void TraceReader::addName(const char* filename)
{
  filenames.push_back(filename);
}

void TraceReader::read(int index)
{
  read(filenames, index);
}

void TraceReader::read(int index_begin, int index_end)
{
  read(filenames, index_begin, index_end);
}

void TraceReader::read(std::istream& src, int index_begin, int index_end) {
	static const char	sep[] = "\t";
	std::auto_ptr<char>	buf(new char[BufferSize]);
	char	*buf_start, *p, *p1;
	int	index;
	double	f;
	bool  notitles = false, is_comment;

	lineCounter = dataLineCounter = 0;
	if (controlledStream)
		maxPrecision = controlledStream->precision();

	firstLine = true;
	dataTitles.clear();

	while (!src.eof()) {
		src.getline(buf.get(), BufferSize);

		// skip starting spaces
		for (buf_start = buf.get(); *buf_start && ::isspace(*buf_start); buf_start++);
		if (!*buf_start)
			continue;

		is_comment = false;
		// check where line is a comment
		if ('#' == *buf_start) {
			// skip remaining spaces
			for (buf_start++; *buf_start && ::isspace(*buf_start); buf_start++);
			if (!*buf_start)
				continue;
			is_comment = true;
		}

		newLine = true;
		for (p = ::strtok(buf_start, sep), index = 0; p; p = ::strtok(0, sep), index++) {
			if (is_comment) {
				if (!notitles)
					addTitle(p, index);
				continue;
			}

			errno = 0;
			f = ::strtod(p, &p1);

			if (p1 > p) {
				// string is a number

				if (controlledStream) {
					// don't lose data precision!
					register std::streamsize prec = p1 - p;
					if (prec > maxPrecision)
						controlledStream->precision(maxPrecision = prec);
				}

				if (!needDataLine) {
				  if ((index >= index_begin || index_begin < 0) && (index < index_end || index_end < 0)) {
	  					  handle(index, f);
								newLine = false;
							}
				}
				else {
							if (newLine) {
								if (dataLine.size() > 0) {
									dataLine.reserve(dataLine.size());
								}
								dataLine.clear();
								newLine = false;
							}
				  dataLine.push_back(f);
				}
			       notitles = true;
			}
			else if (!notitles) {
				// string is not a number
				addTitle(p, index);
			}
			else if (needDataLine)
				dataLine.push_back(0.0);
		}

		if (needDataLine && !newLine && dataLine.size()) {
		  handle(dataLine);
		  dataLineCounter++;
		}

		lineCounter++;
		if (!newLine)
			firstLine = false;
	}
}

void TraceReader::read(const char* filename)
{
	read(filename, -1, -1);
}

void TraceReader::read(const char* filename, int index)
{
	read(filename, index, index >= 0 ? index + 1 : -1);
}

void TraceReader::read(const char* filename, int index_begin, int index_end)
{
	if (!isZip(filename)) {
		if (0 == ::strcmp(STDIN_FILENAME, filename))
			read(std::cin, index_begin, index_end);
		else {
			std::ifstream	src(filename);
			if (!src.is_open())
				return;

			read(src, index_begin, index_end);
		}
	}
	else {
		std::string command("unzip -p ");
		command += filename;

		FILE*	p = 
#ifdef	_MSC_VER
			_popen(command.c_str(), "rt");
#else	//	_MSC_VER
			popen(command.c_str(), "r");
#endif	//	_MSC_VER

		if (0 != p) {

#ifdef	_MSC_VER
			std::filebuf	buffer(p);
#else	//	_MSC_VER
			__gnu_cxx::stdio_filebuf<char>	buffer(p, std::ios_base::in);
#endif	//	_MSC_VER

			std::istream	src(&buffer);
			read(src, index_begin, index_end);

#ifdef	_MSC_VER
			_pclose(p);
#else	//	_MSC_VER
			pclose(p);
#endif	//	_MSC_VER
		}
	}
}

void TraceReader::read(const std::vector<const char*>& filenames, int index)
{
  for (std::vector<const char*>::const_iterator i = filenames.begin(); i != filenames.end(); i++)
    read(*i, index);
}

void TraceReader::read(const std::vector<std::string>& filenames, int index)
{
	read(filenames, index, index >= 0 ? index + 1 : -1);
}

void TraceReader::read(const std::vector<std::string>& filenames, int index_begin, int index_end)
{
	if (0 == filenames.size() || (1 == filenames.size() && 0 == ::strcmp(STDIN_FILENAME, filenames.front().c_str()))) {
		  read(std::cin, index_begin, index_end);
	}
	else {
	  for (std::vector<std::string>::const_iterator i = filenames.begin(); i != filenames.end(); i++)
		  read(i->c_str(), index_begin, index_end);
	}
}

void TraceReader::addTitle(const char* title, int index)
{
	if (*title) {
		while (static_cast<int>(dataTitles.size()) <= index)
			dataTitles.push_back("");
		if (dataTitles[index].size() > 0)
			dataTitles[index] += " ";
		dataTitles[index] += title;
	}
}

///////////////////////////////////////////
//
// MatrixReader members
//
///////////////////////////////////////////

void MatrixReader::handle(float_vector& v)
{
	matrix.add(v);
}

}
