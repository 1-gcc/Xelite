//  IWriter.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

// Copyright (C) 2017 Martin Brunn see License.txt for details

#include <string>

namespace Xelite
{

	class IWriter
	{
	public:
		virtual ~IWriter()
		{

		}

		virtual bool write(const std::wstring&str) = 0;
	};

}
