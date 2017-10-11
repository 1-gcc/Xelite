//  UTFConverter.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
// (C) 2017 Copyright Martin Brunn   see License.txt for details

#include "UTFConverter.h"
#include "Exception.h"
#include <iomanip>
#include <ios>

using namespace Xelite;
namespace cardac
{
	namespace converter
	{

		UTFConverter::UTFConverter()
		{
		}


		UTFConverter::~UTFConverter()
		{
		}

		std::wstring UTFConverter::convertMultiByteToWideString(const std::string&utf8String)
		{
			std::wstring utf16String;
			char iChar;
			std::string::const_iterator i = utf8String.begin();
			unsigned char byte = 0;
			for (;;)
			{
				if (i == utf8String.end())
					throw EOFException();
				iChar = *i;
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
						if (i == utf8String.end())
							throw EOFException();
						iChar = *i;
						xbyte = (unsigned char)iChar;
						//wprintf(L"%04x ", xbyte);

						rest = xbyte & ~0x80;
						ch |= rest << ((len - j) * 6);
					}
					//wprintf(L"]");
					if (ch & 0xffff0000)
						throw UnicodeException(L"unicode char too big for utf-16 wchar_t");

				}
				utf16String += ch;
			}
			return utf16String;
		}
		std::string UTFConverter::convertWideStringToMultiByte(const std::wstring&utf16String)
		{
			std::string utf8StringReturn;
			std::wstring::const_iterator i = utf16String.begin();
			for (; i != utf16String.end(); ++i)
			{
				std::string utf8String;
				char32_t ch = *i;
				char32_t lead = 0x0001;
				char32_t mask = lead << 7;
				if (ch < mask)
				{
					char32_t val = (ch & ~mask);
					unsigned char c = (unsigned char)(val);
					utf8StringReturn += c;
					continue;
				}

				int len = 2;

				if (ch > 0x0800)
				{
					len++;
				}
				if (ch > 0x10000)
				{
					len++;
				}
				char32_t val = ch;
				char32_t lbits = 0x80;

				for (int j = 0; j < len; ++j)
				{
					char32_t c = val & 0x003F;
					val >>= 6;
					std::string s;
					if (j == len - 1)
						c |= lbits;
					else
					{
						c |= 0x80;
						lbits >>= 1;
						lbits |= 0x80;
					}
					s += (char)c;
					utf8String.insert(0, s);
				}
				utf8StringReturn += utf8String;
			}
			return utf8StringReturn;
		}
	}
}
