//  DOMBuilder.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include "DOMBuilder.h"

#include "Element.h"
#include "Document.h"
#include "Comment.h"
#include "Text.h"
#include "DOMTree.h"

namespace Xelite
{

	DOMBuilder::DOMBuilder()
	{
	}


	DOMBuilder::~DOMBuilder()
	{
	}

	IDOMPtr DOMBuilder::createDOM()
	{
		return IDOMPtr(new DOMTree());
	}

	IDocumentPtr DOMBuilder::createDocument()
	{
		return IDocumentPtr(new Document());
	}

	IElementPtr DOMBuilder::createElement()
	{
		return IElementPtr(new Element());
	}

	ITextPtr DOMBuilder::createText()
	{
		return ITextPtr(new Text());
	}

	ICommentPtr DOMBuilder::createComment()
	{
		return ICommentPtr(new Comment());
	}

	IElementPtr DOMBuilder::createElement(IElement & toCopy)
	{
		return IElementPtr((IElement*)toCopy.clone());
	}

	IElementPtr DOMBuilder::createElement(const std::wstring & nspace, const std::wstring & tag)
	{
		return IElementPtr(new Element(nspace, tag));

	}

	IElementPtr DOMBuilder::createElement(const std::wstring & nspace, const std::wstring & tag, Nodes & children)
	{
		return IElementPtr(new Element(nspace, tag, children));
	}

	IElementPtr DOMBuilder::createElement(const std::wstring & nspace, const std::wstring & tag, Attributes & attr)
	{
		return IElementPtr(new Element(nspace, tag, attr));
	}

	IElementPtr DOMBuilder::createElement(const std::wstring & nspace, const std::wstring & tag, Attributes & attr, Nodes & children)
	{
		return IElementPtr(new Element(nspace, tag, attr, children));
	}

	ITextPtr DOMBuilder::createText(const std::wstring & text)
	{
		return ITextPtr(new Text(text));
	}

	ICommentPtr DOMBuilder::createComment(const std::wstring & comment)
	{
		return ICommentPtr(new Comment(comment));
	}
}
