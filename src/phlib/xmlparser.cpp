/*
 * xmlparser.cpp --
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

#include	"xmlparser.h"

namespace phlib {

XmlParser::Parser::Parser(XmlParser* owner)
{
	parser = expat::XML_ParserCreate(NULL);
	expat::XML_SetUserData(parser, owner);
	expat::XML_SetElementHandler(parser, XmlParser::_start, XmlParser::_end);
	expat::XML_SetCharacterDataHandler(parser, XmlParser::_char_data);
}

XmlParser::Parser::~Parser()
{
	expat::XML_ParserFree(parser);
}

XmlParser::AttributeMap::AttributeMap(const char **attr)
{
	while (attr && *attr) {
		XmlParser::TagName	name(*attr++);
		XmlParser::AttrValue	value(*attr++);
		(*this)[name] = value;
	}
}

XmlParser::ParseError::ParseError() : NamedException("XML source is not well-formed")
{
}

XmlParser::XmlParser()
{
	isRecording = false;
	value = std::auto_ptr<TagValue>(new TagValue());
}

void XmlParser::read(const TagName& root_tag, std::istream& source)
{
	int done = 0, len;
	char	buffer[BufferSize];
	Parser	parser(this);

	rootTagName = root_tag;
	deep = 0;
	rootDeep = -1;
	completed = false;

  while (!done && !completed) {
    source.read(buffer, BufferSize);
    len = source.gcount();
    done = source.eof();

    if (!expat::XML_Parse(parser.parser, buffer, len, done))
      throw ParseError();
	}
}

void XmlParser::startRecording() {
	isRecording = true;
	records.clear();
}

void XmlParser::stopRecording(XmlHandler& handler) {
	isRecording = false;

	for (ActionVector::const_iterator i = records.begin(), last = records.end(); i != last; ++i) {
		switch (i->type) {
		case Action::TagStart:
			handler.startTag(i->name, i->attrs);
			break;

		case Action::TagEnd:
			handler.endTag(i->name);
			break;

		case Action::TagData:
			TagValue v(i->data);
			handler.tagData(i->name, v);
			break;
		}
	}
}

void XmlParser::start(const char *el, const char **attr)
{
	checkCharData();

	currTag = el;
	deep++;

	if (rootDeep < 0 && currTag == rootTagName)
		rootDeep = deep;

	if (!completed && rootDeep >= 0) {
		AttributeMap	map(attr);

		if (isRecording) {
			records.push_back(Action(currTag, map));
		}

		startTag(currTag, map);
	}
}

void XmlParser::end(const char *el)
{
	checkCharData();

	if (!completed) {
		if (rootDeep <= deep) {
			endTag(el);

			if (isRecording) {
				records.push_back(Action(el));
			}
		}

		completed = rootDeep == deep--;
	}
}

void XmlParser::charData(const char *s, int len)
{
	if (!completed && rootDeep <= deep) {

		for (; len &&	isspace(*s); s++, len--) {}
		for (; len && isspace(s[len - 1]); len--) {}

		if (len > 0)
			value->write(s, len);
	}
}

void XmlParser::checkCharData()
{
	if (value->tellp() > static_cast<std::ostream::pos_type>(0)) {
		*value << '\0';

		if (isRecording) {
			records.push_back(Action(currTag, *value));
		}

		tagData(currTag, *value);
		value.reset(new TagValue());
	}
}

void XmlParser::_start(void *data, const char *el, const char **attr)
{
	static_cast<XmlParser*>(data)->start(el, attr);
}

void XmlParser::_end(void *data, const char *el)
{
	static_cast<XmlParser*>(data)->end(el);
}

void XmlParser::_char_data(void *data, const char *s, int len)
{
	static_cast<XmlParser*>(data)->charData(s, len);
}

}
