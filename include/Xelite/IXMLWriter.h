//  IXMLWriter.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

// (C) Copyright 2017 Martin Brunn see License.txt for details

#include "IWriter.h"

namespace Xelite
{
	class IXMLWriter :
		public IWriter
	{
	public:
		virtual bool writeText(const std::wstring&text) = 0;
		virtual bool writeComment(const std::wstring&comment) = 0;
		virtual bool writeStartTagBegin(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual bool writeStartTagEnd(bool hasChild) = 0;
		virtual bool writeEmptyStartTagEnd() = 0;
		virtual bool writeEndTag(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual bool writeAttribute(const std::wstring&name, const std::wstring&value) = 0;

	};

}
