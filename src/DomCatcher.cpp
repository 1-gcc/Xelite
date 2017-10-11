//  DomCatcher.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <stdio.h>
#include "DomCatcher.h"
#include "Prolog.h"
#include "Exception.h"

namespace Xelite
{

	DomCatcher::DomCatcher(IDOMBuilder*builder) :builder(builder), currentElement(NULL)
	{
		inTag = false;
		dom = builder->createDOM();
		if (dom->isA() == dtDOMTree)
		{

		}
		else
			throw Exception(L"DOMTree error");
	}


	DomCatcher::~DomCatcher()
	{

	}

	// protected
	bool DomCatcher::beginProlog(const std::wstring & tag)
	{
		//wprintf(L"prolog<%S>\n", tag.c_str());
		return true;
	}

	// protected
	bool DomCatcher::endProlog(const std::wstring & tag, const std::wstring & prologtext, IProlog * prolog)
	{
		//wprintf(L"prolog<%S>\n", prologtext.c_str());
		return true;
	}

	bool DomCatcher::beginQTag(const std::wstring & nspace, const std::wstring & tag)
	{
		return false;
	}

	bool DomCatcher::endQTag(const std::wstring & nspace, const std::wstring & tag, Attributes & attr)
	{
		//wprintf(L"?<%S:%S>? %d attributes\n", nspace.c_str(), tag.c_str(), (int)attr.size());
		std::wstring version;
		std::wstring encoding;
		Attributes::iterator i = attr.find(L"version");
		if (i != attr.end())
			version = i->second;
		i = attr.find(L"encoding");
		if (i != attr.end())
			encoding = i->second;
		// TODO problem: who is owner of Prolog object?
		dom->getDocument()->setProlog(IPrologPtr(new Prolog(tag, version, encoding)));
		return false;
	}

	bool DomCatcher::beginStartTag(const std::wstring & nspace, const std::wstring & tag)
	{
		return false;
	}


	bool DomCatcher::endStartTag(const std::wstring & nspace, const std::wstring & tag, Attributes & attr)
	{
		//wprintf(L"<%S:%S> %d attributes\n", nspace.c_str(), tag.c_str(),(int)attr.size());
		IElementPtr elem = builder->createElement(nspace, tag, attr);
		if (!currentElement)
		{
			currentElement = elem;
			dom->getDocument()->setRoot(currentElement);
		}
		else
		{
			currentElement->appendChild(elem);
			elementStack.push(currentElement);
			currentElement = elem;
		}
		return false;
	}

	bool DomCatcher::endEmptyTag(const std::wstring & nspace, const std::wstring & tag, Attributes & attr)
	{
		//wprintf(L"<%S:%S> %d attributes\n", nspace.c_str(), tag.c_str(), (int)attr.size());
		IElementPtr elem = builder->createElement(nspace, tag, attr);
		if (!currentElement)
		{
			currentElement = elem;
			dom->getDocument()->setRoot(currentElement);
		}
		else
		{
			currentElement->appendChild(elem);
		}

		return false;
	}

	bool DomCatcher::beginEndTag()
	{
		return false;
	}

	bool DomCatcher::endEndTag(const std::wstring & nspace, const std::wstring & tag)
	{
		//wprintf(L"</%S:[%S]>\n", nspace.c_str(), tag.c_str());
		if (!currentElement)
			throw XMLException(0, L"Missing root element");
		std::wstring xnspace = currentElement->getNamespace();
		std::wstring xtag = currentElement->getName();
		//wprintf(L"</%S:%S>\n", xnspace.c_str(), xtag.c_str());

		if (xnspace != nspace || xtag != tag)
			throw XMLException(L"unbalanced closing tag </" + tag + L"> expected </" + xtag + L">");
		if (elementStack.size())
		{
			currentElement = elementStack.top();
			elementStack.pop();
		}
		return false;
	}

	bool DomCatcher::beginComment()
	{
		return false;
	}

	bool DomCatcher::endComment(const std::wstring & comment)
	{
		//wprintf(L"end comment <%S>\n", comment.c_str());
		currentElement->appendChild(builder->createComment(comment));
		return true;
	}

	bool DomCatcher::beginAttribute()
	{
		//wprintf(L"begin attribute\n");
		return true;
	}

	bool DomCatcher::endAttribute(const std::wstring & attribute)
	{
		//wprintf(L"end attribute <%S>\n",attribute.c_str());

		return true;
	}

	bool DomCatcher::beginAttributeValue(const std::wstring & attribute)
	{
		return true;
	}

	bool DomCatcher::endAttributeValue(const std::wstring & attribute, const std::wstring & value)
	{
		//wprintf(L"attr <%S>=<%S>\n", attribute.c_str(), value.c_str());
		return true;
	}

	bool DomCatcher::beginText()
	{
		return false;
	}

	bool DomCatcher::endText(const std::wstring & text)
	{
		// ignore on start of document
		if (!currentElement)
			return false;
		//wprintf(L"text=(%d)<%S", text.size(),text.c_str());
		//wprintf(L">\n");
		currentElement->appendChild(builder->createText(text));
		return true;
	}

	bool DomCatcher::beginEntity()
	{
		return false;
	}

	bool DomCatcher::endEntity(const std::wstring & entity, const std::wstring& value)
	{
		return false;
	}
	IDOMPtr DomCatcher::getDOM()
	{
		return this->dom;
	}
}
