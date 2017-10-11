//  Exception.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <string>
#include "Exception.h"

namespace Xelite
{


	Exception::Exception() :text(NULL)
	{
	}


	Exception::~Exception()
	{
	}

	Exception::Exception(const wchar_t * text) :text(text)
	{
	}

	const wchar_t * Exception::getText()
	{
		return text;
	}

	UnicodeException::UnicodeException()
	{
	}

	UnicodeException::~UnicodeException()
	{
	}

	/*UnicodeException::UnicodeException(const char * text):Exception(text)
	{
	}*/

	UnicodeException::UnicodeException(const wchar_t * format, ...)
	{
		va_list list;
		va_start(list, format);
		vswprintf(buffer, sizeof(buffer) - 1, format, list);
		buffer[sizeof(buffer) - 1] = 0;
		text = buffer;
	}

	XMLException::XMLException()
	{
	}

	XMLException::~XMLException()
	{
	}

	XMLException::XMLException(int line, const wchar_t * text) :Exception(text), lineNumber(line)
	{
		wtext = text;
	}

	XMLException::XMLException(const std::wstring & str)
	{
		wtext = str;
	}
}
