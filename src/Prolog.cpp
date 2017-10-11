//  Prolog.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include "Prolog.h"

namespace Xelite
{

	Prolog::Prolog(const std::wstring & name, const std::wstring & version, const std::wstring & encoding) :
		version(version), encoding(encoding), name(name), parent(NULL)
	{
	}

	Prolog::~Prolog()
	{
	}

	std::wstring Prolog::getName() const
	{
		return name;
	}

	std::wstring Prolog::getVersion() const
	{
		return version;
	}

	std::wstring Prolog::getEncoding() const
	{
		return encoding;
	}

	bool Prolog::write(IXMLWriter * writer) const
	{
		writer->write(L"<?xml version=\"");
		writer->write(version);
		writer->write(L"\" encoding=\"");
		writer->write(encoding);
		writer->write(L"\" ?>\n");
		return false;
	}

	INodePtr Prolog::getParent() const
	{
		return parent;
	}

	void Prolog::setParent(INodePtr pParent)
	{
		parent = pParent;
	}
}
