//  Document.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include "IDOM.h"

namespace Xelite
{

	class Document :
		public IDocument
	{
	public:
		Document();
		Document(const Document&doc)
		{
			root = (IElementPtr)((IElement*)doc.root->clone());
			prolog = IPrologPtr((IProlog*)doc.getProlog()->clone());
			entities = doc.entities;
			reverseEntities = doc.reverseEntities;
		}
		~Document();
		virtual IPrologPtr getProlog() const;
		virtual IElementPtr getRoot() const;
		virtual bool addEntity(const std::wstring& name, const std::wstring& value);
		virtual bool setProlog(IPrologPtr prolog);
		virtual bool setRoot(IElementPtr root);
		virtual bool write(IXMLWriter*writer) const;
		virtual IDocument * clone() const
		{
			return new Document(*this);
		}
		virtual INodePtr getParent() const
		{
			return NULL;
		}
		virtual void setParent(INodePtr pParent)
		{

		}

		virtual NodeType isA() const
		{
			return ntDocument;
		}
	protected:
		IPrologPtr prolog;
		IElementPtr root;
		Entities entities;
		Entities reverseEntities;
	};
	/*
	class DOM : public IDOM
	{
	public:
		virtual IDocument * getDocument();
	protected:
		Document document;
	};
	*/
}
