//  FileReader.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include <istream>

#include "IReader.h"

namespace Xelite
{

	class FileReader :
		public IReader
	{
	public:
		FileReader(std::istream&str);
		~FileReader();
		virtual wchar_t getChar();
		virtual wchar_t peekChar() const;
		virtual bool nextChar();
		virtual int currentLineNumber() const;

	protected:
		std::istream&str;
		int lineNumber;
	};

}
