/*
 * xmlstream.h --
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

#ifndef	__MD_XMLSTREAM_H_85783458234547857824567835678
#define	__MD_XMLSTREAM_H_85783458234547857824567835678

#include	<stack>
#include	<string>
#include	<sstream>

namespace phlib {

class XmlStream {
public:

	typedef	std::string	string_type;
	typedef	std::ostream	ostream_type;
	typedef	std::ostringstream	sstream_type;

	// XML version constants
	enum {versionMajor = 1, versionMinor = 0};

	// Internal helper class
	struct Controller {
		typedef	enum {whatProlog, whatTag, whatTagEnd, whatAttribute, whatCharData}	what_type;

		typedef	std::string	string_type;

		what_type	what;
		string_type str;

		inline Controller(const Controller& c) : what(c.what), str(c.str) {}
		inline Controller(const what_type _what) : what(_what){}
		
		// use template constructor because string field <str> may be initialized 
		// from different sources: char*, std::string etc
		template<class t>
		inline Controller(const what_type _what, const t& _str) : what(_what), str(_str) {}
	};

	// XmlStream refers ostream object to perform actual output operations
	XmlStream(ostream_type&	_s);
	XmlStream(ostream_type&	_s, const string_type& encoding);

	// Before destroying check whether all the open tags are closed
	~XmlStream();

	// default behaviour - delegate object output to ostream
	template<class t>
	XmlStream& operator<<(const t& value) {
		if (stateTagName == state)
			tagName << value;
		s << value;
		return *this;
	}

	// overload ostream::write function
	template<class t>
	XmlStream& write(const t* str, int n) {
		if (stateTagName == state)
			tagName.write(str, n);
		s.write(str, n);
		return *this;
	}
	
	// this is the main working horse
	XmlStream& operator<<(const Controller& controller);

	static int allocIndex() {
		if (xallocIndex < 0)
			xallocIndex = std::ios_base::xalloc();
		return xallocIndex;
	}

private:
	// state of the stream 
	typedef	enum {stateNone, stateTag, stateAttribute, stateTagName}	state_type;

	// tag name stack
	typedef std::stack<string_type>	tag_stack_type;

	static int	xallocIndex;
	tag_stack_type	tags;
	state_type	state;
	ostream_type&	s;
	bool	prologWritten;
	sstream_type	tagName;
	string_type	encoding;

	// clear tagName field
	void clearTagName();

	// Close current tag
	void closeTagStart(bool self_closed = false);

	// Close tag (may be with closing all of its children)
	void endTag(const string_type& tag);

	inline void startTagging() {
		s.iword(xallocIndex) = 0;
	}

	inline void endTagging() {
		s.iword(xallocIndex) = 1;
	}
};	//	class XmlStream

inline const XmlStream::Controller prolog() {
	return XmlStream::Controller(XmlStream::Controller::whatProlog);
}

inline const XmlStream::Controller tag() {
	return XmlStream::Controller(XmlStream::Controller::whatTag);
}

inline const XmlStream::Controller tag(const char* const tag_name) {
	return XmlStream::Controller(XmlStream::Controller::whatTag, tag_name);
}

inline const XmlStream::Controller tag(const std::string& tag_name) {
	return XmlStream::Controller(XmlStream::Controller::whatTag, tag_name);
}

inline const XmlStream::Controller endtag(const char* const tag_name) {
	return XmlStream::Controller(XmlStream::Controller::whatTagEnd, tag_name);
}

inline const XmlStream::Controller endtag(const std::string& tag_name) {
	return XmlStream::Controller(XmlStream::Controller::whatTagEnd, tag_name);
}

inline const XmlStream::Controller attr(const char* const attr_name) {
	return XmlStream::Controller(XmlStream::Controller::whatAttribute, attr_name);
}

inline const XmlStream::Controller attr(const std::string& attr_name) {
	return XmlStream::Controller(XmlStream::Controller::whatAttribute, attr_name);
}

inline const XmlStream::Controller endtag() {
	return XmlStream::Controller(XmlStream::Controller::whatTagEnd);
}

inline const XmlStream::Controller chardata() {
	return XmlStream::Controller(XmlStream::Controller::whatCharData);
}


}

#endif	//	__MD_XMLSTREAM_H_85783458234547857824567835678
