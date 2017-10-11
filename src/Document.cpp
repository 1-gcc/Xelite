//  Document.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include "Document.h"

namespace Xelite
{


	Document::Document() : prolog(NULL), root(NULL)
	{
	}


	Document::~Document()
	{
	}

	IPrologPtr Document::getProlog() const
	{
		return prolog;
	}

	IElementPtr Document::getRoot() const
	{
		return root;
	}

	bool Document::addEntity(const std::wstring & name, const std::wstring & value)
	{
		entities[name] = value;
		reverseEntities[value] = name;
		return false;
	}

	bool Document::setProlog(IPrologPtr prolog)
	{
		this->prolog = prolog;
		return true;
	}

	bool Document::setRoot(IElementPtr  root)
	{
		this->root = root;
		return true;
	}

	bool Document::write(IXMLWriter * writer) const
	{
		prolog->write(writer);
		return getRoot()->write(writer);
	}

}
