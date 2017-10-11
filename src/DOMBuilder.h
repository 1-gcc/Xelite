//  DOMBuilder.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include "IDOM.h"
namespace Xelite
{
	class DOMBuilder :public IDOMBuilder
	{
	public:
		DOMBuilder();
		~DOMBuilder();
		virtual IDOMPtr  createDOM();
		virtual IDocumentPtr  createDocument();
		virtual IElementPtr  createElement();
		virtual ITextPtr  createText();
		virtual ICommentPtr  createComment();
		virtual IElementPtr  createElement(IElement&toCopy);
		virtual IElementPtr  createElement(const std::wstring&nspace, const std::wstring&tag);
		virtual IElementPtr  createElement(const std::wstring&nspace, const std::wstring&tag, Nodes&children);
		virtual IElementPtr  createElement(const std::wstring&nspace, const std::wstring&tag, Attributes&attr);
		virtual IElementPtr  createElement(const std::wstring&nspace, const std::wstring&tag,
			Attributes&attr, Nodes&children);
		virtual ITextPtr  createText(const std::wstring&text);
		virtual ICommentPtr  createComment(const std::wstring&comment);

	};

}
