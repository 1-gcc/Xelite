//  Xereader.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include "IDOM.h"
#include "IXCatcher.h"
#include "IReader.h"

namespace Xelite
{
	class Xereader
	{
	public:
		Xereader();
		~Xereader();
		bool parse(IReader&reader, IXCatcher&catcher, bool preserveWhitespace);
	protected:
	};

}
