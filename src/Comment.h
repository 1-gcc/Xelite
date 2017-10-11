//  Comment.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include "IDOM.h"

namespace Xelite
{

	class IXMLWriter;
	class Comment :
		public IComment
	{
	public:
		Comment();
		Comment(const Comment&other);
		Comment(const std::wstring&comment);
		~Comment();
		virtual INode* clone() const;
		virtual bool write(IXMLWriter*writer) const;

		virtual std::wstring getText() const;
		virtual void setText(const std::wstring&text);
		virtual INodePtr getParent() const;
		virtual void setParent(INodePtr pParent);
		virtual NodeType isA() const
		{
			return ntComment;
		}
	protected:
		std::wstring comment;
		INodePtr parent;
	};

}
