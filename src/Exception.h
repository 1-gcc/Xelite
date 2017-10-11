//  Exception.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include <wchar.h>

namespace Xelite
{
	class Exception
	{
	public:
		Exception();
		~Exception();
		Exception(const wchar_t*text);
		virtual const wchar_t * getText();
	protected:
		const wchar_t*text;
	};

	class UnicodeException : public Exception
	{
	public:
		UnicodeException();
		~UnicodeException();

		UnicodeException(const wchar_t*format, ...);
	protected:
		wchar_t buffer[1024];

	};
	class XMLException : public Exception
	{
	public:
		XMLException();
		~XMLException();
		XMLException(int line, const wchar_t*text);
		XMLException(const std::wstring&str);
		virtual const wchar_t* getText()
		{
			return wtext.c_str();
		}
		int lineNumber;
		std::wstring wtext;
	};
	class EOFException : public Exception
	{
	public:
		EOFException() :Exception(L"EOF")
		{

		}
		~EOFException()
		{

		}
	};
}
