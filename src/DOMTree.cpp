//  DOMTree.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include "DOMTree.h"

namespace Xelite
{


	DOMTree::DOMTree()
	{
		predefinedEntities.insert(std::make_pair(L"quot", L"\""));
		predefinedEntities.insert(std::make_pair(L"apos", L"\'"));
		predefinedEntities.insert(std::make_pair(L"gt", L">"));
		predefinedEntities.insert(std::make_pair(L"lt", L"<"));
		predefinedEntities.insert(std::make_pair(L"amp", L"&"));
		for (Entities::iterator i = predefinedEntities.begin(); i != predefinedEntities.end(); ++i)
		{
			reversePredefinedEntities.insert(std::make_pair(i->second, i->first));
		}
		document = IDocumentPtr(new Document);
	}


	DOMTree::~DOMTree()
	{
	}

	IDocumentPtr DOMTree::getDocument()
	{
		return document;
	}
}
