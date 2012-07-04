/*
 * cmdline.h --
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

#ifndef __CMDLINE_H_43874553575757758735763457234
#define __CMDLINE_H_43874553575757758735763457234

#include  <vector>
#include  <string>
#include  <sstream>
#include  <exception>
#include  <complex>
#include  <string.h>

namespace phlib {

namespace options {
	namespace priorities {
		extern const char* low;
		extern const char* normal;
		extern const char* high;
	}
}


class CommandLine {

  typedef std::vector<const char*>  pchar_vector;

  pchar_vector  params, savedParams;
  pchar_vector::size_type valueIndex;
  bool  required;
  bool  optionFound;

	// check if parameter is an option name
	static inline bool isOption(const char* const s) {
		return '-' == s[0] && '\0' != s[1];
	}

	// find desired option in the <param> list
	// return its index in list
	pchar_vector::size_type findOption(const char* const option_name) const {
		for (pchar_vector::size_type i = 0; i < params.size(); i++)
			if (0 == option_name) {
				if (!isOption(params[i]))
					return i;
			}
			else {
				if (isOption(params[i]) && 0 == ::strcmp(option_name, params[i] + 1))
					return i;
			}

		return	params.size();
	}

	template<class t>
	inline CommandLine& simpleValue(t& dest) {
		if (hasData(1))
	    dest = value();
		return *this;
	}

public:

  // common command line error
	class Error : std::exception {
	public:
		inline Error(const CommandLine& /* not used yet */) {}
		virtual ~Error() throw() {}

		virtual const char* what() const throw() {
			return "Error in command line";
		}
	};

	// unknown option found
	class UnknownOption : Error {
		std::string	message;

	public:
		inline UnknownOption(const CommandLine& cmd, const char* const option_name)
			: Error(cmd), message("Unexpected option: ") {
			message += option_name;
		}
		virtual ~UnknownOption() throw() {}

		virtual const char* what() const throw() {
			return message.c_str();
		}
	};

  CommandLine(int argc, const char* const* argv) : required(false), optionFound(false) {
    for (int i = 1; i < argc; i++)
      params.push_back(argv[i]);

		savedParams = params;
    valueIndex = params.size();
  }

	// reset class to initial state - ready for next use
	inline void rewind() {
		params = savedParams;
	}

	// check if there are unhandled options
	void checkUnknownOptions() const {
		for (pchar_vector::const_iterator i = params.begin(); i != params.end(); i++)
			if (isOption(*i))
				throw UnknownOption(*this, *i);
	}

	// check if command line has desired option
  inline bool hasOption(const char* const option_name) const {
    return findOption(option_name) < params.size();
  }

	// check if there are parameter to read
	// <param_num> - required number of values
  inline bool hasData(unsigned param_num) const {
		if (valueIndex + param_num <= params.size())
			return true;
		else if (required)
      throw Error(*this);
		else
			return false;
  }

	// retrieve one value
  const char* value() {
		// we must check parameter existance via hasData() call
		if (valueIndex >= params.size())
			throw Error(*this);

		const char* res = params[valueIndex];
		params.erase(params.begin() + valueIndex);

    return res;
  }

  inline CommandLine& operator<<(const bool rq) {
    required = rq;
    return *this;
  }

  CommandLine& operator<<(const char* const option_name) {
		optionFound = false;
		if (option_name && *option_name) {
			// look for named option
			if ((valueIndex = findOption(option_name)) < params.size()) {
				params.erase(params.begin() + valueIndex);
				optionFound = true;
				required = true;
			}
			else
				required = false;
		}
		else
			// reset value index
			// we are looking for unnamed parameter
			valueIndex = findOption(0);

    return *this;
  }

	// generic processing of <some type> value
	template<class t>
  CommandLine& operator>>(t& dest) {
		if (hasData(1)) {
			std::istringstream	s(value());
			s >> dest;
			if (s.fail())
				throw Error(*this);
		}
		return *this;
	}

	// special processing of <bool> options
	// boolean options have no values
  inline CommandLine& operator>>(bool& dest) {
    dest = optionFound;
    return *this;
  }

	// simplest processing of string values
  inline CommandLine& operator>>(const char*& dest) {return simpleValue(dest);}
  inline CommandLine& operator>>(std::string& dest) {return simpleValue(dest);}

};

}

#endif  //  __CMDLINE_H_43874553575757758735763457234
