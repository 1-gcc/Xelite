//  Text.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <iostream>
#include "Text.h"
#include "UTFConverter.h"

namespace Xelite
{

	Text::Text()
	{
	}

	Text::Text(const std::wstring & text) :text(text), parent(NULL)
	{
	}

	INode*  Text::clone() const
	{
		return new Text(*this);
	}

	bool Text::write(IXMLWriter * writer) const
	{
		writer->write(text);
		return false;
	}

	std::wstring Text::getText() const
	{
		return text;
	}

	std::string Text::getTextUTF8() const
	{
		cardac::converter::UTFConverter conv;
		return conv.convertWideStringToMultiByte(text);
	}

	void Text::setText(const std::wstring & text)
	{
		this->text = text;
	}

	INodePtr Text::getParent() const
	{
		return parent;
	}

	void Text::setParent(INodePtr pParent)
	{
		parent = pParent;
	}

}
