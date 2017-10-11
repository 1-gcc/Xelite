//  Text.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include "IDOM.h"
namespace Xelite
{
	class Text : public IText
	{
	public:
		Text();
		Text(const std::wstring&text);
		virtual INode* clone() const;
		virtual bool write(IXMLWriter*writer) const;
		virtual std::wstring getText() const;
		virtual std::string getTextUTF8() const;
		virtual void setText(const std::wstring&text);
		virtual INodePtr getParent() const;
		virtual void setParent(INodePtr pParent);
		virtual NodeType isA() const
		{
			return ntText;
		}

	protected:
		std::wstring text;
		INodePtr parent;
	};
}
