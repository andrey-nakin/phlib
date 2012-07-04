/*
 * xmlparser.h --
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

#ifndef	__MDXMLPARSER_H_78943578934534535678345745647
#define	__MDXMLPARSER_H_78943578934534535678345745647

#include	<string>
#include	<map>
#include	<sstream>
#include	<vector>
#include	<memory>
#include	"namedexception.h"

extern "C" {
namespace expat {
#include	<expat.h>
}
}

namespace phlib {

struct XmlHandler {

	typedef std::string TagName;
	typedef std::string AttrValue;
	typedef std::stringstream TagValue;

	class	AttributeMap : public std::map<TagName, AttrValue> {
	public:
		AttributeMap() {}
		AttributeMap(const char **attr);
	};

	virtual ~XmlHandler() {}

	virtual void startTag(const TagName&, const AttributeMap&) {}
	virtual void endTag(const TagName&) {}
	virtual void tagData(const TagName&, TagValue&) {}

};

class XmlParser : public XmlHandler {

	XmlParser(const XmlParser&);
	XmlParser& operator=(const XmlParser&);

public:

	enum {
		BufferSize = 4096
	};

  class ParseError : public NamedException {
  public:
    ParseError();
  };

	XmlParser();
	~XmlParser() {}

	struct Action {
		typedef enum {TagStart, TagData, TagEnd} action_type;

		action_type type;
		TagName name;
		AttributeMap attrs;
		std::string data;

		Action(const TagName& name) :
			type(TagEnd), name(name)
		{}

		Action(const TagName& name, const AttributeMap& attrs) :
			type(TagStart), name(name), attrs(attrs)
		{}

		Action(const TagName& name, const TagValue& value) :
			type(TagData), name(name), data(value.str())
		{}
	};

	typedef std::vector<Action> ActionVector;

protected:

	void read(const TagName& root_tag, std::istream& source);
	void startRecording();
	void stopRecording(XmlHandler&);

private:

	struct Parser {
	  expat::XML_Parser	parser;

		Parser(XmlParser*);
		~Parser();
	};

	friend struct Parser;

	TagName	rootTagName, currTag;
	std::auto_ptr<TagValue>	value;
	int	deep, rootDeep;
	bool	completed;
	bool isRecording;
	ActionVector records;

	void start(const char *el, const char **attr);
	void end(const char *el);
	void charData(const char *s, int len);

	void checkCharData();

	static void _start(void*, const char*, const char**);
	static void _end(void*, const char*);
	static void _char_data(void*, const char*, int);
};

}

#endif	//	__MDXMLPARSER_H_78943578934534535678345745647
