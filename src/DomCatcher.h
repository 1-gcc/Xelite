//  DomCatcher.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include <stack>

#include "IXCatcher.h"
#include "IDOM.h"

namespace Xelite
{
	class DomCatcher :
		public IXCatcher
	{
	public:
		DomCatcher(IDOMBuilder*builder);
		~DomCatcher();
		virtual bool beginQTag(const std::wstring&nspace, const std::wstring&tag);
		virtual bool endQTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr);
		virtual bool beginStartTag(const std::wstring&nspace, const std::wstring&tag);
		virtual bool endStartTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr);
		virtual bool endEmptyTag(const std::wstring&nspace, const std::wstring&tag, Attributes & attr);

		virtual bool beginEndTag();
		virtual bool endEndTag(const std::wstring&nspace, const std::wstring&tag);
		virtual bool beginComment();
		virtual bool endComment(const std::wstring&comment);
		virtual bool beginAttribute();
		virtual bool endAttribute(const std::wstring&attribute);
		virtual bool beginAttributeValue(const std::wstring&attribute);
		virtual bool endAttributeValue(const std::wstring&attribute, const std::wstring&value);
		virtual bool beginText();
		virtual bool endText(const std::wstring&text);
		virtual bool beginEntity();
		virtual bool endEntity(const std::wstring&entity, const std::wstring&value);
		virtual IDOMPtr getDOM();
	protected:
		virtual bool beginProlog(const std::wstring&tag);
		virtual bool endProlog(const std::wstring&tag, const std::wstring&prologtext, IProlog*prolog);

		bool inTag;
		bool inProlog;
		std::stack<IElementPtr> elementStack;
		IDOMBuilder* builder;
		IDOMPtr dom;
		IElementPtr currentElement;
	};

}
