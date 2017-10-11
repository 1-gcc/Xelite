//  IDOM.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

// Copyright (C) 2017 Martin Brunn see License.txt for details

#include <set>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include "IXMLWriter.h"

namespace Xelite
{

	class INode;
	class IComment;
	class IText;
	class IElement;
	class IDocument;
	class IDOM;
	class IDOMBuilder;
	class IXMLWriter;
	class IProlog;
	typedef std::shared_ptr<INode> INodePtr;
	typedef std::shared_ptr<IComment> ICommentPtr;
	typedef std::shared_ptr<IText> ITextPtr;
	typedef std::shared_ptr<IDocument> IDocumentPtr;
	typedef std::shared_ptr<IDOM> IDOMPtr;
	typedef std::shared_ptr<IDOMBuilder> IDOMBuilderPtr;
	typedef std::shared_ptr<IXMLWriter> IXMLWriterPtr;
	typedef std::shared_ptr<IElement> IElementPtr;
	typedef std::shared_ptr<IProlog> IPrologPtr;

	typedef std::vector<std::shared_ptr<INode>> Nodes;
	typedef std::map<std::wstring, std::wstring> Entities;

	typedef std::shared_ptr<Nodes> NodesPtr;

	extern Entities predefinedEntities;
	extern Entities entities;
	typedef std::map<std::wstring, std::wstring> Attributes;
	enum NodeType
	{
		ntNone,
		ntDocument,
		ntXML,
		ntComment,
		ntText,
		ntElement,
		ntCDATA,
		ntENTITY,
		ntDOCTYPE,
	};
	class INode
	{
	public:
		virtual ~INode()
		{

		}
		virtual bool write(IXMLWriter*writer) const = 0;
		virtual INode* clone() const = 0;
		virtual INodePtr getParent() const = 0;
		virtual void setParent(INodePtr pParent) = 0;
		virtual NodeType isA() const = 0;
	};

	class IProlog : public INode
	{
	public:
		virtual ~IProlog()
		{

		}
		virtual std::wstring getName() const = 0;
		//virtual const Attributes & getAttributes() const = 0;
		virtual std::wstring getVersion() const = 0;
		virtual std::wstring getEncoding() const = 0;
	};
	class IElement :public INode
	{
	public:
		const std::set<const wchar_t*> boolTrue =
		{
			 L"T",
			 L"TRUE",
			 L"Y",
			 L"YES",
			 L"1",
			 L"ON",
		};
		const std::set<const wchar_t*> boolFalse =
		{
			L"F",
			L"FALSE",
			L"N",
			L"NO",
			L"0",
			L"OFF",
		};
		const std::vector<const wchar_t *> inttemplates =
		{
			L"0x%x",
			L"#%x",
			L"%d",
		};
		class iterator
		{
		protected:
			NodesPtr owner;
			size_t current;
			size_t count;
		public:
			static const size_t END = (size_t)-1;
		public:

			iterator(NodesPtr pelem) :owner(pelem), current(0)
			{
				count = pelem->size();
				//std::wcerr << L"iterator:" << current << L":" << count << L"\n";
			}
			iterator(NodesPtr pelem, size_t count, size_t current) :
				owner(pelem), current(current), count(count)
			{
				//std::wcerr << L"iterator:" << current << L":" << count << L"\n";
			}
			virtual ~iterator()
			{
				//std::wcerr << L"~iterator:" << current << L"\n";
			}
			// postfix
			iterator operator ++(int)
			{
				iterator i = *this;
				if (current < count - 1)
					current++;
				else
					current = END;
				//std::wcerr << L"current++:" << current << L"\n";
				return i;
			}
			// prefix
			iterator & operator ++()
			{
				if (current < count - 1)
					current++;
				else
					current = END;
				//std::wcerr << L"++current:" << current << L"\n";
				return *this;
			}
			iterator & operator --()
			{
				if (current > 0 && current < count)
					current--;
				else
					current = END;
				return *this;
			}
			// postfix
			iterator operator --(int)
			{
				iterator i = *this;
				if (current > 0 && current < count)
					current--;
				else
					current = END;
				//std::wcerr << L"current++:" << current << L"\n";
				return i;
			}
			IElement * operator *() const
			{
				if (current == END)
					return NULL;
				return (IElement*)owner.get()->at(current).get();
			}
			bool operator ==(const iterator& compare) const
			{
				if (current == compare.current)
					return true;
				return false;
			}
			bool operator !=(const iterator& compare) const
			{
				if (current != compare.current)
				{
					//std::wcerr << L"current!=:" << current << L":" << compare.current << L"\n";

					return true;
				}
				return false;
			}
		};
		class const_iterator
		{
		protected:
			NodesPtr owner;
			size_t current;
			size_t count;
		public:
			static const size_t END = (size_t)-1;
		public:
			const_iterator(const NodesPtr pelem) :owner(pelem)
			{
				count = pelem->size();
				if (count)
					current = count - 1;
				else
					current = count;
			}
			const_iterator(NodesPtr pelem, size_t count, size_t current) :
				owner(pelem), current(current), count(count)
			{

			}
			virtual ~const_iterator()
			{
				//std::wcerr << L"~iterator:" << current << L"\n";
			}
			// postfix
			const_iterator operator ++(int)
			{
				const_iterator i = *this;
				if (current < count - 1)
					current++;
				else
					current = END;
				//std::wcerr << L"current++:" << current << L"\n";
				return i;
			}
			// prefix
			const_iterator & operator ++()
			{
				if (current < count - 1)
					current++;
				else
					current = END;
				//std::wcerr << L"++current:" << current << L"\n";
				return *this;
			}
			const_iterator & operator --()
			{
				if (current > 0 && current < count)
					current--;
				else
					current = END;
				return *this;
			}
			// postfix
			const_iterator operator --(int)
			{
				const_iterator i = *this;
				if (current > 0 && current < count)
					current--;
				else
					current = END;
				//std::wcerr << L"current++:" << current << L"\n";
				return i;
			}
			IElement * operator *() const
			{
				if (current == END)
					return NULL;
				return (IElement*)owner.get()->at(current).get();
			}
			bool operator ==(const const_iterator& compare) const
			{
				if (current == compare.current)
					return true;
				return false;
			}
			bool operator !=(const const_iterator& compare) const
			{
				if (current != compare.current)
				{
					//std::wcerr << L"current!=:" << current << L":" << compare.current << L"\n";

					return true;
				}
				return false;
			}

		};
		virtual std::wstring getName() const = 0;
		virtual std::wstring getNamespace() const = 0;
		virtual void setName(const std::wstring&name) = 0;
		virtual void setNamespace(const std::wstring&nspace) = 0;

		virtual Attributes & getAttributes() = 0;
		virtual std::vector<std::wstring> getAttributeNames() const = 0;
		virtual std::wstring getAttribute(const std::wstring&name) const = 0;
		virtual bool getAttributeString(const std::wstring&name, std::wstring&value) const = 0;
		virtual bool getAttributeUTF8String(const std::wstring&name, std::string&value) const = 0;
		virtual bool getAttributeInt(const std::wstring&name, int&value) const = 0;
		virtual bool getAttributeInt(const std::wstring&name, int&value,
			const wchar_t*inttemplate) const = 0;
		virtual bool getAttributeBool(const std::wstring&name, bool &value,
			const wchar_t*booltemplate) const = 0;
		virtual bool hasAttribute(const std::wstring&name) const = 0;
		virtual void setAttribute(const std::wstring&name, const std::wstring&value) = 0;

		virtual bool hasChildren() const = 0;
		virtual const Nodes & getChildren() const = 0;
		virtual Nodes & getChildren() = 0;
		virtual Nodes getChildren(const std::wstring&name) const = 0;

		virtual bool isEmpty() const = 0;
		virtual void appendChild(INodePtr child) = 0;
		virtual void appendChildren(Nodes&children) = 0;
		virtual iterator end() = 0;
		virtual const_iterator end() const = 0;
		virtual iterator findFirstChild() = 0;
		virtual iterator findFirstChild(const std::wstring&name) = 0;
		virtual iterator findLastChild() = 0;
		virtual iterator findLastChild(const std::wstring&name) = 0;
		virtual const_iterator findFirstChild() const = 0;
		virtual const_iterator findFirstChild(const std::wstring&name) const = 0;
		virtual const_iterator findLastChild() const = 0;
		virtual const_iterator findLastChild(const std::wstring&name) const = 0;

	};
	class IText :public INode
	{
	public:
		virtual std::wstring getText() const = 0;
		virtual std::string getTextUTF8() const = 0;
		virtual void setText(const std::wstring&text) = 0;
	};
	class IComment :public INode
	{
	public:
		virtual std::wstring getText() const = 0;
		virtual void setText(const std::wstring&text) = 0;
	};
	class IDocument : public INode
	{
	public:
		virtual ~IDocument()
		{

		}
		virtual IPrologPtr getProlog() const = 0;
		virtual IElementPtr getRoot() const = 0;
		virtual bool addEntity(const std::wstring& name, const std::wstring& value) = 0;
		virtual bool setProlog(IPrologPtr prolog) = 0;
		virtual bool setRoot(IElementPtr root) = 0;
	};
	enum DomType
	{
		dtNone,
		dtDOMTree,
	};

	class IDOM
	{
	public:

		virtual ~IDOM()
		{

		}
		virtual DomType isA() const = 0;
		virtual IDOM* clone() = 0;
		virtual IDocumentPtr getDocument() = 0;
	};

	class IDOMBuilder
	{
	public:
		virtual ~IDOMBuilder()
		{

		}
		virtual IDOMPtr createDOM() = 0;
		virtual IDocumentPtr createDocument() = 0;
		virtual IElementPtr createElement() = 0;
		virtual ITextPtr createText() = 0;
		virtual ICommentPtr createComment() = 0;
		virtual IElementPtr createElement(IElement&toCopy) = 0;
		virtual IElementPtr createElement(const std::wstring&nspace, const std::wstring&tag) = 0;
		virtual IElementPtr createElement(const std::wstring&nspace,
			const std::wstring&tag, Nodes&children) = 0;
		virtual IElementPtr createElement(const std::wstring&nspace,
			const std::wstring&tag, Attributes&attr) = 0;
		virtual IElementPtr createElement(const std::wstring&nspace, const std::wstring&tag,
			Attributes&attr, Nodes&children) = 0;
		virtual ITextPtr createText(const std::wstring&text) = 0;
		virtual ICommentPtr createComment(const std::wstring&comment) = 0;
	};

	typedef IXMLWriter * (*CreateXMLWriter)(std::wostream&ostr, bool pretty);
	typedef IDOMBuilder * (*CreateDOMBuilder)();
	typedef IDOM* (*ParseXMLUTF8Stream)(std::istream&is, bool preserveWhitespace);
	typedef IDOM* (*ParseXMLUTF8String)(std::string&str, bool preserveWhitespace);
	typedef IDOM* (*ParseXMLUTF8File)(const char*fileName, bool preserveWhitespace);
	typedef void(*DeleteDOM)(IDOM*pDOM);
	typedef void(*DeleteDOMBuilder)(IDOMBuilder*pDOMBuilder);
	typedef void(*DeleteXMLWriter)(IXMLWriter*pXMLWriter);
}
