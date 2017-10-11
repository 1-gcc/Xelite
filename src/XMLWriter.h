//  XMLWriter.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include <ostream>

#include "IXMLWriter.h"
namespace Xelite
{
	class XMLWriter :
		public IXMLWriter
	{
	public:
		XMLWriter(std::wostream & os, bool pretty);
		~XMLWriter();
		virtual bool write(const std::wstring&str);

		virtual bool writeText(const std::wstring&text);
		virtual bool writeComment(const std::wstring&comment);
		virtual bool writeStartTagBegin(const std::wstring&nspace, const std::wstring&tag);
		virtual bool writeStartTagEnd(bool hasChild);
		virtual bool writeEmptyStartTagEnd();
		virtual bool writeEndTag(const std::wstring&nspace, const std::wstring&tag);
		virtual bool writeAttribute(const std::wstring&name, const std::wstring&value);
	protected:
		bool writeEOL();
		bool writeIndent(int level);
		std::wstring convert(const std::wstring&text);
		bool checkTag(const std::wstring &tag);
		bool checkComment(const std::wstring &tag);
		bool convertValue(const std::wstring &value, std::wstring&out, bool & useApostroph);
		std::wostream & os;
		bool pretty;
		int level;
	};

}
