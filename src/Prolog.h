//  Prolog.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include "IDOM.h"
//#include "Element.h"

namespace Xelite
{

	class Prolog :
		/*public Element , */public IProlog
	{
	public:
		Prolog(const std::wstring&name, const std::wstring&version, const std::wstring&encoding);
		virtual ~Prolog();
		virtual std::wstring getName() const;
		virtual std::wstring getVersion() const;
		virtual std::wstring getEncoding() const;
		virtual bool write(IXMLWriter*writer) const;
		virtual INodePtr getParent() const;
		virtual void setParent(INodePtr pParent);

		virtual NodeType isA() const
		{
			return ntXML;
		}
		virtual INode * clone() const
		{
			return new Prolog(*this);
		}
	protected:
		std::wstring version;
		std::wstring encoding;
		std::wstring name;
		INodePtr parent;
	};

}
