//  DOMTree.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include "IDOM.h"
#include "Document.h"

namespace Xelite
{

	class DOMTree : public IDOM
	{
	public:
		DOMTree();
		virtual ~DOMTree();
		virtual IDocumentPtr getDocument();
		virtual IDOM* clone()
		{
			return (new DOMTree(*this));
		}
		virtual DomType isA() const
		{
			return dtDOMTree;
		}

	protected:
		Entities predefinedEntities;
		Entities entities;
		Entities reversePredefinedEntities;
		Entities reverseEntities;
		IDocumentPtr document;
	};

}
