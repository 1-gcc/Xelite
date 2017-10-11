//  IXCatcher.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include <string>

#include "IDOM.h"
namespace Xelite
{
	class IXCatcher
	{
	public:
		virtual bool beginQTag(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual bool endQTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr) = 0;
		virtual bool beginStartTag(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual bool endStartTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr) = 0;
		virtual bool endEmptyTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr) = 0;

		virtual bool beginEndTag() = 0;
		virtual bool endEndTag(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual bool beginComment() = 0;
		virtual bool endComment(const std::wstring&comment) = 0;
		virtual bool beginAttribute() = 0;
		virtual bool endAttribute(const std::wstring&attribute) = 0;
		virtual bool beginAttributeValue(const std::wstring&attribute) = 0;
		virtual bool endAttributeValue(const std::wstring&attribute, const std::wstring&value) = 0;
		virtual bool beginText() = 0;
		virtual bool endText(const std::wstring&text) = 0;
		virtual bool beginEntity() = 0;
		virtual bool endEntity(const std::wstring&entity, const std::wstring&value) = 0;
	};

}
