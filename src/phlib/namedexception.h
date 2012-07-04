/*
 * namedexception.h --
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

#ifndef	__MD_NAMEDEXCEPTION_H_7357835378567346
#define	__MD_NAMEDEXCEPTION_H_7357835378567346

#include	<string>
#include	<exception>

namespace phlib {

class NamedException : public std::exception {

	std::string	message;

public:

	NamedException(const char* msg) : message(msg ? msg : "")
	{}

	NamedException(const NamedException& ne) : message(ne.message)
	{}

	virtual ~NamedException() throw()
	{}

	virtual const char* what() const throw() {
		return message.c_str();
	}
};

}

#endif	//	__MD_NAMEDEXCEPTION_H_7357835378567346
