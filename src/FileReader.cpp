//  FileReader.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <istream>

#include "FileReader.h"
#include "Exception.h"

namespace Xelite
{

	FileReader::FileReader(std::istream&str) :str(str), lineNumber(1)
	{

	}


	FileReader::~FileReader()
	{
	}

	wchar_t FileReader::getChar()
	{

		char iChar;

		unsigned char byte = 0;
		if (!str.get(iChar))
			throw EOFException();

		byte = (unsigned char)iChar;
		wchar_t ch = 0;

		if ((byte & 0x80) == 0)
			ch = 0x007f & byte;
		else
		{
			size_t len = 0;
			//wprintf(L"[%04x ", byte);
			for (len = 0; byte & (0x80 >> len); ++len)
			{
				if (len > 4)
					throw UnicodeException(L"Leading byte %02x exceeds with %d length of 4", byte, len);
			}
			len--;
			if (len == 0)
				throw UnicodeException(L"invalid utf8 character %02x", byte);
			unsigned char mask = 0xff;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
			mask <<= (7 - len);
#pragma GCC diagnostic pop
			mask = (unsigned char)~mask;
			wchar_t rest = (mask & byte) & 0x000000ff;
			ch = rest << (len * 6);
			for (size_t j = 1; j <= len; ++j)
			{
				unsigned char xbyte = 0;
				if (!str.get(iChar))
					throw EOFException();
				xbyte = (unsigned char)iChar;
				//wprintf(L"%04x ", xbyte);

				rest = xbyte & ~0x80;
				ch |= rest << ((len - j) * 6);
			}
			//wprintf(L"]");
			if (ch & 0xffff0000)
				throw UnicodeException(L"unicode char too big for utf-16 wchar_t");

		}
		if (ch == L'\n')
		{
			lineNumber++;
		}
		//wprintf(L"%04x ", ch);
		return ch;
	}


	wchar_t FileReader::peekChar() const
	{
		return L'\0';
	}

	bool FileReader::nextChar()
	{
		return false;
	}

	int FileReader::currentLineNumber() const
	{
		return lineNumber;
	}
}
