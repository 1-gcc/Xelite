//  Element.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include "IDOM.h"

namespace Xelite
{

	class Element :
		public IElement
	{
	public:
		Element();
		Element(const Element&elem);
		Element(const std::wstring & nspace, const std::wstring & name);
		Element(const std::wstring & nspace, const std::wstring & name, Attributes&attr);
		Element(const std::wstring & nspace, const std::wstring & name, Nodes&children);
		Element(const std::wstring & nspace, const std::wstring & name, Attributes&attr, Nodes&children);
		~Element();
		virtual INode* clone() const;
		virtual bool write(IXMLWriter*writer) const;

		virtual std::wstring getName() const;
		virtual std::wstring getNamespace() const;
		virtual void setName(const std::wstring&name);
		virtual void setNamespace(const std::wstring&nspace);

		virtual void appendChild(INodePtr child);
		virtual void appendChildren(Nodes&children);
		virtual Attributes & getAttributes();
		virtual bool hasAttribute(const std::wstring&name) const;
		virtual std::wstring getAttribute(const std::wstring&name) const;
		virtual std::vector<std::wstring> getAttributeNames() const;
		virtual void setAttribute(const std::wstring&name, const std::wstring&value);

		virtual bool getAttributeString(const std::wstring&name, std::wstring&value) const;
		virtual bool getAttributeUTF8String(const std::wstring&name, std::string&value) const;
		virtual bool getAttributeInt(const std::wstring&name, int&value) const;
		virtual bool getAttributeInt(const std::wstring&name, int&value,
			const wchar_t*inttemplate) const;
		virtual bool getAttributeBool(const std::wstring&name, bool &value,
			const wchar_t*booltemplate) const;

		virtual bool hasChildren() const;
		virtual const Nodes & getChildren() const;
		virtual Nodes & getChildren();
		virtual INodePtr getParent() const;
		virtual void setParent(INodePtr pParent);

		virtual Nodes getChildren(const std::wstring&name) const;
		virtual iterator end();
		virtual const_iterator end() const;
		virtual iterator findFirstChild();
		virtual iterator findFirstChild(const std::wstring&name);
		virtual iterator findLastChild();
		virtual iterator findLastChild(const std::wstring&name);
		virtual const_iterator findFirstChild() const;
		virtual const_iterator findFirstChild(const std::wstring&name) const;
		virtual const_iterator findLastChild() const;
		virtual const_iterator findLastChild(const std::wstring&name) const;

		virtual bool isEmpty() const;
		virtual NodeType isA() const
		{
			return ntElement;
		}
	protected:
		Nodes children;
		std::wstring nspace;
		std::wstring name;
		Attributes attributes;
		INodePtr parent;

		bool empty;
	};

}
