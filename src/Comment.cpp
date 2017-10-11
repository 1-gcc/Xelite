//  Comment.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <iostream>
#include "Comment.h"

namespace Xelite
{


	Comment::Comment() :parent(NULL)
	{
	}

	Comment::Comment(const std::wstring & comment) : comment(comment), parent(NULL)
	{
	}
	Comment::Comment(const Comment&other)
	{
		comment = other.comment;
		parent = other.parent;
	}

	Comment::~Comment()
	{
	}

	INode* Comment::clone() const
	{
		return new Comment(*this);
	}

	bool Comment::write(IXMLWriter * writer) const
	{
		writer->writeComment(comment);
		return true;
	}

	std::wstring Comment::getText() const
	{
		return comment;
	}

	void Comment::setText(const std::wstring & text)
	{
		comment = text;
	}

	INodePtr Comment::getParent() const
	{
		return parent;
	}

	void Comment::setParent(INodePtr pParent)
	{
		parent = pParent;
	}
}
