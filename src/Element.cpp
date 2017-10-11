//  Element.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <iostream>
#include <cstdio>
#include <fstream>

#include "Element.h"
#include "UTFConverter.h"

namespace Xelite
{

	Element::Element()
	{
		empty = true;
	}

	Element::Element(const std::wstring & nspace, const std::wstring & name) :
		nspace(nspace), name(name), parent(NULL)
	{
	}

	Element::Element(const std::wstring & nspace, const std::wstring & name, Attributes & attr) :
		nspace(nspace), name(name), attributes(attr), parent(NULL)
	{
	}

	Element::Element(const std::wstring & nspace, const std::wstring & name, Nodes & children) :
		children(children), nspace(nspace), name(name), parent(NULL), empty(false)
	{
	}

	Element::Element(const std::wstring & nspace, const std::wstring & name, Attributes & attr,
		Nodes & children) : children(children), nspace(nspace), name(name),
		attributes(attr), parent(NULL), empty(false)
	{
	}



	Element::~Element()
	{

		//Nodes::const_iterator i = children.begin();
		//for (; i != children.end(); ++i)
		//{
		//	delete (*i);
		//}
	}
	Element::Element(const Element &elem)
	{
		attributes = elem.attributes;
		name = elem.name;
		nspace = elem.nspace;
		parent = elem.parent;
		children = elem.children;
		//Nodes::const_iterator i = elem.children.begin();
		//for (; i != elem.children.end(); ++i)
		//{
		//	INode * p = (*i)->clone();

		//	p->setParent(this);
		//	children.push_back(p);
		//}

	}

	INode* Element::clone() const
	{
		INode * p = new Element(*this);
		return p;
	}

	bool Element::write(IXMLWriter * writer) const
	{
		writer->writeStartTagBegin(nspace, name);
		Attributes::const_iterator i = attributes.begin();
		for (; i != attributes.end(); ++i)
		{
			writer->writeAttribute(i->first, i->second);
		}
		if (!hasChildren())
		{
			writer->writeEmptyStartTagEnd();
		}
		else
		{
			bool hasChild = false;
			for (Nodes::const_iterator n = children.begin(); n != children.end(); ++n)
			{
				if ((*n)->isA() == ntElement)
				{
					hasChild = true;
					break;
				}
			}
			writer->writeStartTagEnd(hasChild);
			bool wasText = false;
			for (Nodes::const_iterator n = children.begin(); n != children.end(); ++n)
			{
				if ((*n)->isA() == ntElement && wasText)
					writer->write(L"\n");
				(*n)->write(writer);
				if ((*n)->isA() == ntText)
				{
					wasText = true;
				}
				else
					wasText = false;
			}
			if (wasText)
				writer->write(L"\n");
			writer->writeEndTag(nspace, name);
		}
		return false;
	}

	std::wstring Element::getName() const
	{
		return name;
	}

	std::wstring Element::getNamespace() const
	{
		return nspace;
	}

	void Element::setName(const std::wstring & name)
	{
		this->name = name;
	}

	void Element::setNamespace(const std::wstring & nspace)
	{
		this->nspace = nspace;
	}

	void Element::appendChild(INodePtr child)
	{
		children.push_back(child);
	}

	void Element::appendChildren(Nodes & otherchildren)
	{
		Nodes::iterator i = otherchildren.begin();
		for (; i != otherchildren.end(); ++i)
		{
			this->children.push_back((*i));
		}
	}


	Attributes & Element::getAttributes()
	{
		return attributes;
	}

	std::wstring Element::getAttribute(const std::wstring & name) const
	{
		Attributes::const_iterator i = attributes.find(name);
		if (i != attributes.end())
			return i->second;
		return std::wstring();
	}

	bool Element::hasAttribute(const std::wstring & name) const
	{
		if (attributes.find(name) != attributes.end())
			return true;
		return false;
	}

	std::vector<std::wstring> Element::getAttributeNames() const
	{
		std::vector<std::wstring> ret;
		for (Attributes::const_iterator i = attributes.begin(); i != attributes.end(); ++i)
		{
			ret.push_back(i->first);
		}
		return ret;
	}

	void Element::setAttribute(const std::wstring & name, const std::wstring & value)
	{
		Attributes::iterator i = attributes.find(name);
		if (i != attributes.end())
			i->second = value;
		else
			attributes.insert(std::make_pair(name, value));
	}

	bool Element::getAttributeString(const std::wstring & name, std::wstring & value) const
	{
		Attributes::const_iterator i = attributes.find(name);
		if (i == attributes.end())
			return false;
		value = i->second;
		return true;
	}

	bool Element::getAttributeUTF8String(const std::wstring & name, std::string & value) const
	{
		Attributes::const_iterator i = attributes.find(name);
		if (i == attributes.end())
			return false;
		cardac::converter::UTFConverter conv;
		value = conv.convertWideStringToMultiByte(i->second);
		return true;
	}

	bool Element::getAttributeInt(const std::wstring & name, int & value) const
	{
		return getAttributeInt(name, value, L"%d");
	}

	bool Element::getAttributeInt(const std::wstring & name, int & value, const wchar_t * inttemplate) const
	{
		Attributes::const_iterator i = attributes.find(name);
		if (i == attributes.end())
			return false;
		int in = 0;
		if (swscanf(i->second.c_str(), inttemplate, &in) == 1)
		{
			value = in;
			return true;
		}
		return false;
	}

	bool Element::getAttributeBool(const std::wstring & name, bool & value, const wchar_t * booltemplate) const
	{
		Attributes::const_iterator i = attributes.find(name);
		if (i == attributes.end())
			return false;
		if (i->second == L"T")
			value = true;
		else if (i->second == L"F")
			value = false;
		else
			return false;
		return true;
	}


	bool Element::hasChildren() const
	{
		return children.size() ? true : false;
	}

	const Nodes & Element::getChildren() const
	{
		return children;
	}
	Nodes & Element::getChildren()
	{
		return children;
	}

	INodePtr Element::getParent() const
	{
		return parent;
	}

	void Element::setParent(INodePtr pParent)
	{
		parent = pParent;
	}

	Nodes Element::getChildren(const std::wstring & name) const
	{
		Nodes nodesNew;
		for (Nodes::const_iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->isA() == ntElement && ((IElement*)i->get())->getName() == name)
				nodesNew.push_back(*i);
		}
		return nodesNew;

	}

	IElement::iterator Element::end()
	{
		return iterator(NULL, 0, iterator::END);
	}
	IElement::const_iterator Element::end() const
	{
		return const_iterator(NULL, 0, iterator::END);
	}

	IElement::iterator Element::findFirstChild()
	{
		return iterator(NodesPtr(new Nodes(children)));
	}

	IElement::iterator Element::findFirstChild(const std::wstring & name)
	{
		Nodes * nodesNew = new Nodes;
		for (Nodes::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->isA() == ntElement && ((IElement*)i->get())->getName() == name)
				nodesNew->push_back(*i);
		}
		return iterator(NodesPtr(nodesNew));
	}

	IElement::iterator Element::findLastChild()
	{
		if (children.size())
			return iterator(NodesPtr(new Nodes(children)), children.size(), children.size() - 1);
		return end();
	}

	IElement::iterator Element::findLastChild(const std::wstring & name)
	{
		Nodes * nodesNew = new Nodes;
		for (Nodes::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->isA() == ntElement && ((IElement*)i->get())->getName() == name)
				nodesNew->push_back(*i);
		}
		return iterator(NodesPtr(nodesNew), children.size(), children.size() - 1);
	}

	IElement::const_iterator Element::findFirstChild() const
	{
		return const_iterator(NodesPtr(new Nodes(children)));
	}

	IElement::const_iterator Element::findFirstChild(const std::wstring & name) const
	{
		Nodes * nodesNew = new Nodes;
		for (Nodes::const_iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->isA() == ntElement && ((IElement*)i->get())->getName() == name)
				nodesNew->push_back(*i);
		}
		return const_iterator(NodesPtr(nodesNew));
	}

	IElement::const_iterator Element::findLastChild() const
	{
		if (children.size())
			return const_iterator(NodesPtr(new Nodes(children)), children.size(), children.size() - 1);
		return end();
	}

	IElement::const_iterator Element::findLastChild(const std::wstring & name) const
	{
		Nodes * nodesNew = new Nodes;
		for (Nodes::const_iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->isA() == ntElement && ((IElement*)i->get())->getName() == name)
				nodesNew->push_back(*i);
		}
		return const_iterator(NodesPtr(nodesNew), nodesNew->size(),
			nodesNew->size() ? nodesNew->size() - 1 : 0);
	}

	bool Element::isEmpty() const
	{
		return false;
	}
}
