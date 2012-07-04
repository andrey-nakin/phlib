/*
 * xmlstream.cpp --
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

#include	"xmlstream.h"

namespace phlib {

int	XmlStream::xallocIndex = -1;

XmlStream::XmlStream(ostream_type&	_s) : state(stateNone), s(_s), prologWritten(false)
{
	allocIndex();
	endTagging();
}

XmlStream::XmlStream(ostream_type&	_s, const string_type& enc) : state(stateNone), s(_s), prologWritten(false), encoding(enc)
{
	allocIndex();
	endTagging();
}

// Before destroying check whether all the open tags are closed
XmlStream::~XmlStream()
{
	if (stateTagName == state) {
		s << "/>";
		state = stateNone;
	}
	while (tags.size())
		endTag(tags.top());
}

// this is the main working horse
// and it's long a little
XmlStream& XmlStream::operator<<(const Controller& controller)
{

	switch (controller.what) {
	case Controller::whatProlog:
		if (!prologWritten && stateNone == state) {
			startTagging();
			s << "<?xml version=\"" << versionMajor << '.' << versionMinor;
			if (!encoding.empty())
				s << "\" encoding=\"" << encoding;
			s << "\"?>\n";
			endTagging();
			prologWritten = true;
		}
		break;	//	Controller::whatProlog

	case Controller::whatTag:
		closeTagStart();
		startTagging();
		s << '<';
		if (controller.str.empty()) {
			clearTagName();
			state = stateTagName;
		}
		else {
			s << controller.str;
			tags.push(controller.str);
			state = stateTag;
		}
		endTagging();
		break;	//	Controller::whatTag

	case Controller::whatTagEnd:
		endTag(controller.str);
		break;	//	Controller::whatTagEnd

	case Controller::whatAttribute:
		switch (state) {
		case stateTagName:
			tags.push(tagName.str());
			break;

		case stateAttribute:
			startTagging();
			s << '\"';
			endTagging();

		default:
			break;
		}

		if (stateNone != state) {
			startTagging();
			s << ' ' << controller.str << "=\"";
			endTagging();
			state = stateAttribute;
		}
		// else throw some error - unexpected attribute (out of any tag)

		break;	//	Controller::whatAttribute

	case Controller::whatCharData:
		closeTagStart();
		state = stateNone;
		break;	//	Controller::whatCharData

	default:
		break;
	}

	return	*this;
}

// I don't know any way easier (legal) to clear stringstream...
void XmlStream::clearTagName()
{
	static const	string_type	empty_str;
	tagName.rdbuf()->str(empty_str);
}

// Close current tag
void XmlStream::closeTagStart(bool self_closed)
{
	if (stateTagName == state)
		tags.push(tagName.str());

	// note: absence of 'break's is not an error
	switch (state) {
	case stateAttribute:
		startTagging();
		s << '\"';
		endTagging();

	case stateTagName:
	case stateTag:
		startTagging();
		if (self_closed)
			s << '/';
		s << '>';
		endTagging();

	default:
		break;
	}
}

// Close tag (may be with closing all of its children)
void XmlStream::endTag(const string_type& tag)
{
	bool	brk = false;

	while (tags.size() > 0 && !brk) {
		if (stateNone == state) {
			startTagging();
			s << "</" << tags.top() << '>';
			endTagging();
		}
		else {
			closeTagStart(true);
			state = stateNone;
		}
		brk = tag.empty() || tag == tags.top();
		tags.pop();
	}
}

}
