//  XMLWriter.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//

#include "XMLWriter.h"
#include "Exception.h"

namespace Xelite
{

	bool XMLWriter::writeIndent(int level)
	{
		if (level > 0)
		{
			std::wstring tab(level << 1, L' ');
			write(tab);
		}
		return true;
	}


	XMLWriter::XMLWriter(std::wostream & os, bool pretty) : os(os), pretty(pretty), level(0)
	{
	}

	XMLWriter::~XMLWriter()
	{
	}

	bool XMLWriter::write(const std::wstring & str)
	{
		os << str;
		return false;
	}

	bool XMLWriter::writeText(const std::wstring & text)
	{
		write(convert(text));

		return true;
	}

	bool XMLWriter::writeComment(const std::wstring & comment)
	{
		if (!checkComment(comment))
			return false;
		if (pretty)
			writeIndent(level);
		write(L"<!--");
		write(comment);
		write(L"-->");
		if (pretty)
			write(L"\n");
		return true;
	}

	bool XMLWriter::writeStartTagBegin(const std::wstring & nspace, const std::wstring & tag)
	{
		if (!checkTag(tag))
			return false;
		if (pretty)
			writeIndent(level++);
		write(L"<");
		write(tag);
		return true;
	}


	bool XMLWriter::writeStartTagEnd(bool hasChild)
	{
		if (pretty && hasChild)
			return write(L">\n");
		return write(L">");
	}

	bool XMLWriter::writeEmptyStartTagEnd()
	{
		if (pretty)
		{
			level--;
			return write(L"/>\n");
		}

		return write(L"/>");
	}

	bool XMLWriter::writeEndTag(const std::wstring & nspace, const std::wstring & tag)
	{
		if (!checkTag(tag))
			return false;
		if (pretty)
			writeIndent(--level);

		write(L"</");
		if (pretty)
			write(tag + L">\n");
		else
			write(tag + L">");
		return true;
	}

	bool XMLWriter::writeAttribute(const std::wstring & name, const std::wstring & value)
	{
		if (!checkTag(name))
			return false;
		std::wstring xvalue;
		bool useApostroph = false;
		if (!convertValue(value, xvalue, useApostroph))
			return false;
		write(L" ");
		write(name);
		write(L"=");
		if (useApostroph)
			write(L"\'");
		else
			write(L"\"");
		write(xvalue);
		if (useApostroph)
			write(L"\'");
		else
			write(L"\"");
		return true;
	}

	bool XMLWriter::writeEOL()
	{
		os << "\n";
		return true;
	}
	/*
	wchar_t getNextCharacter(std::wstring::const_iterator&i,const std::wstring&tag)
	{
		unsigned char byte = *i;
		wchar_t ch;
		if ((byte & 0x80) == 0)
			ch = 0x007f & byte;
		else
		{
			size_t len = 0;
			for (len = 0; byte & (0x80 >> len); ++len)
			{
				if (len > 4)
					throw UnicodeException(L"Leading byte %d exceeds %d length of 4", byte, len);
			}
			len--;
			if (len == 0)
				throw UnicodeException(L"invalid utf8 character %02x", byte);
			unsigned char mask = 0xff;
			mask <<= (7 - len);
			mask = ~mask;
			wchar_t rest = (mask & byte) & 0x000000ff;
			ch = rest; // << (len * 6);
			++i;
			for (size_t j = 1; j <= len; ++j, ++i)
			{
				rest = *i & ~0x80;
				ch |= rest << ((j) * 6);
			}
			if (ch & 0xffff0000)
				throw UnicodeException(L"unicode char too big for utf-16 wchar_t");

		}
		return ch;
	}*/
	std::wstring XMLWriter::convert(const std::wstring & text)
	{
		std::wstring out;
		std::wstring::const_iterator i = text.begin();
		wchar_t ch = *i;
		for (; i != text.end(); ch = *++i)
		{
			switch (ch)
			{
			case L'&':
				out += L"&amp;";
				break;
			case L'>':
				out += L"&gt;";
				break;
			case L'<':
				out += L"&lt;";
				break;
			case L'\"':
				out += L"&quote;";
				break;
			case L'\'':
				out += L"&apos;";
				break;
			default:
				out += ch;
				break;
			}

		}
		return out;
	}
	bool XMLWriter::checkTag(const std::wstring & tag)
	{
		std::wstring::const_iterator i = tag.begin();
		wchar_t ch = *i;
		if (ch != L'_' && !iswalpha(ch))
			return false;
		for (; i != tag.end(); ++i)
		{
			ch = *i;
			if (ch == L'_' || ch == L'-' || ch == L'.' || iswdigit(ch) || iswalpha(ch) || ch == L':')
				continue;
			return false;
		}
		return true;

	}

	bool XMLWriter::checkComment(const std::wstring & text)
	{
		return true;
	}

	bool XMLWriter::convertValue(const std::wstring & value, std::wstring & out, bool & useApostroph)
	{
		try
		{
			out = convert(value);
		}
		catch (Exception e)
		{
			return false;
		}
		useApostroph = false;

		return true;
	}
}
