//  Xereader.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include <wchar.h>
#include <wctype.h>
#include <stdio.h>
#include "Xereader.h"
#include "Exception.h"

namespace Xelite
{

	int print(const char*p)
	{
		return 0;
		//return printf(p);
	}
	int print(const char*format, int d)
	{
		return 0;
		//return printf(format, d);
	}
	Xereader::Xereader()
	{
	}


	Xereader::~Xereader()
	{
	}
	enum TagType
	{
		ttNone,
		ttStandard,
		ttQTag,
	};

	enum ReaderState
	{
		rsIdle,
		rsLTFound,
		rsInStartTag,
		rsInText,
		rsInComment,
		rsCDATA,
		rsEntityDef,
		rsEntityUse,
		rsInEndTag,
	};
	enum TagSubState
	{
		tssLookTagName,
		tssLookAttribute,
		tssAttributeName,
		tssLookEqual,
		tssLookValue,
		tssLookEndValue,

	};
	bool isLegalTagNameChar(wchar_t ch)
	{
		return true;
	}
	class ParserState
	{
	public:
		ParserState()
		{
			state = rsIdle;
			tagSubState = tssLookTagName;
			tagType = ttNone;
			endChar = 0;
		}
		ReaderState state;
		TagSubState tagSubState;
		TagType tagType;
		wchar_t endChar;
		std::wstring tag;
		std::wstring nspace;
		std::wstring text;
		std::wstring attributeName;
		std::wstring attributeValue;
		Attributes attributes;

	};
	enum EntityState
	{
		esIdle,
		esName,
		esNameFound,
		esValue,
		esValueFound,
	};
	bool checkEntityDef(const std::wstring&name, const std::wstring&value)
	{
		if (!value.size() || value[0] != L'&')
			return false;
		return true;
	}
	bool treatEntityDef(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState&state)
	{
		std::wstring name;
		std::wstring value;
		EntityState estate = esIdle;
		for (;;)
		{
			if (ch == L'>')
			{
				if (!checkEntityDef(name, value))
					throw XMLException(reader.currentLineNumber(), L"wrong ENTITY syntax ...");
				catcher.endEntity(name, value);
				state.state = rsIdle;
				return true;
			}
			if (iswblank(ch)) switch (estate)
			{
			case esName:
				estate = esNameFound;
				break;
			case esValue:
				estate = esValueFound;
				break;
			default:
				break;
			}
			else switch (estate)
			{
			case esName:
				name += ch;
				break;
			case esValue:
				value += ch;
				break;
			case esNameFound:
				estate = esValue;
				value += ch;
				break;
			case esIdle:
				estate = esName;
				name += ch;
				break;
			case esValueFound:
				throw XMLException(reader.currentLineNumber(), L"wrong ENTITY syntax");
			}
			ch = reader.getChar();
		}
		return false;
	}

	bool treatLTFound(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState&state)
	{
		if (ch == L'<')
		{
			throw XMLException(reader.currentLineNumber(), L"Syntax error less than < in tag");
		}
		else if (ch == L'!')
		{
			wchar_t CDATA[] = L"[CDATA[";
			wchar_t ENTITY[] = L"ENTITY ";
			size_t len = wcslen(CDATA);
			std::wstring helper = L"<";

			helper += ch;
			bool first = true;
			bool cdata = false;
			bool entity = false;
			for (size_t i = 0; i < len; i++)
			{
				ch = reader.getChar();
				if (first)
				{
					if (ch == L'-')
					{
						ch = reader.getChar();
						if (ch == L'-')
						{
							state.state = rsInComment;
							helper.clear();
							return true;
						}
					}

					first = false;
					if (ch == CDATA[i])
						cdata = true;
					else if (ch == ENTITY[i])
						entity = true;
					helper += ch;
					continue;
				}
				else if (cdata)
				{
					if (ch != CDATA[i])
					{
						throw XMLException(reader.currentLineNumber(), L"invalid CDATA statement");
					}
				}
				else if (entity)
				{
					if (ch != ENTITY[i])
						throw XMLException(reader.currentLineNumber(), L"invalid ENTITY statement");

				}
				helper += ch;
			}
			if (helper == L"<![CDATA[")
			{
				state.text += helper;
				state.state = rsCDATA;
			}
			else if (helper == L"<!ENTITY ")
			{
				state.state = rsEntityDef;
			}
			helper.clear();
			return true;
		}
		switch (ch)
		{
		case L'/':

			state.state = rsInEndTag;
			state.tag.clear();
			state.nspace.clear();
			return true;

		case L'?':
			state.tagType = ttQTag;

			ch = reader.getChar();
			if (ch == L'>')
			{
				throw XMLException(reader.currentLineNumber(), L"Syntax error tag name expected, but '>' found");
			}
			break;

		case L'>':
		{
			throw XMLException(reader.currentLineNumber(),
				L"Syntax error greater than > found instead of tag name");
		}
		default:
			state.tagType = ttStandard;
			break;
		}
		state.state = rsInStartTag;
		state.tagSubState = tssLookTagName;
		state.tag.clear();
		state.nspace.clear();

		state.tag += ch;
		return false;
	}
	bool treatIdle(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState&state)
	{
		if (ch != L'<')
		{
			state.state = rsInText;
			catcher.beginText();
			state.text += ch;

		}
		else
		{
			state.state = rsLTFound;
		}
		return false;
	}
	bool treatLookAttribute(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (iswblank(ch))
			return true;
		if (ch == L'/' || ch == L'?')
		{
			bool q = false;
			if (ch == L'?')
				q = true;
			ch = reader.getChar();
			if (ch == L'>')
			{
				state.state = rsIdle;
				if (q)
					catcher.endQTag(state.nspace, state.tag, state.attributes);
				else
				{
					catcher.endEmptyTag(state.nspace, state.tag, state.attributes);
				}
				state.attributes.clear();
				state.nspace.clear();
				state.tag.clear();

			}
			else
				throw XMLException(reader.currentLineNumber(), L"missing '>' after / or ?");
		}
		else if (ch == L'>')
		{
			state.state = rsIdle;
			catcher.endStartTag(state.nspace, state.tag, state.attributes);
			state.attributes.clear();
			state.nspace.clear();
			state.tag.clear();
		}
		else
		{
			state.tagSubState = tssAttributeName;
			state.attributeName.clear();
			state.attributeValue.clear();
			state.attributeName += ch;
		}
		return false;
	}
	bool treatAttributeName(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (ch == L'=' || iswblank(ch))
		{
			catcher.endAttribute(state.attributeName);
			if (ch == L'=')
			{
				state.tagSubState = tssLookValue;
			}
			else
			{
				state.tagSubState = tssLookEqual;
			}
		}
		else
		{
			state.attributeName += ch;
		}
		return false;
	}
	bool treatLookValue(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (iswblank(ch))
			return true;
		if (ch == '\"' || ch == '\'')
		{
			catcher.beginAttributeValue(state.attributeName);
			state.tagSubState = tssLookEndValue;
			state.endChar = ch;
		}
		else
			throw XMLException(reader.currentLineNumber(),
				L"Syntax error: no quote or apostroph found");
		return false;
	}
	bool treatLookEqual(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (ch == L'=')
		{
			catcher.endAttribute(state.attributeName);
			state.tagSubState = tssLookValue;
		}
		else if (iswblank(ch))
			return true;
		else
			throw XMLException(reader.currentLineNumber(),
				L"Syntax error: missing '=' of attribute");
		return false;
	}
	bool treatLookEndValue(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (ch == state.endChar)
		{
			state.tagSubState = tssLookAttribute;
			state.state = rsInStartTag;
			catcher.endAttributeValue(state.attributeName, state.attributeValue);
			state.attributes[state.attributeName] = state.attributeValue;
			state.attributeName.clear();
			state.attributeValue.clear();
		}
		else
		{
			state.attributeValue += ch;
		}
		return false;
	}
	bool treatLookTagName(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		if (!state.tag.size())
		{
			switch (ch)
			{
			case L'>':
				throw XMLException(reader.currentLineNumber(),
					L"Syntax error '>' found but tag empty");
				break;
			}
		}
		switch (ch)
		{
		case L'/':
			if (state.tag.size())
			{
				ch = reader.getChar();
			}
			else
			{
				throw XMLException(reader.currentLineNumber(),
					L"Syntax error: '/' in wrong place");
			}
			if (ch != L'>')
			{
				throw XMLException(reader.currentLineNumber(),
					L"Syntax error '/' found but '>' not found where expected");
			}
			catcher.endEmptyTag(state.nspace, state.tag, state.attributes);
			state.attributes.clear();
			state.nspace.clear();
			state.tag.clear();
			state.state = rsIdle;
			break;
		case L'>':
			catcher.endStartTag(state.nspace, state.tag, state.attributes);
			state.attributes.clear();
			state.nspace.clear();
			state.tag.clear();
			state.state = rsIdle;
			break;
		default:
			if (iswblank(ch))
			{
				catcher.beginStartTag(state.nspace, state.tag);
				state.tagSubState = tssLookAttribute;
			}
			else
				state.tag += ch;
		}
		return false;

	}
	bool treatInStartTag(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState & state)
	{
		switch (state.tagSubState)
		{
		case tssLookTagName:
		{
			if (treatLookTagName(ch, reader, catcher, state))
				return true;
			break;
		}
		case tssLookAttribute:
			if (treatLookAttribute(ch, reader, catcher, state))
				return true;
			break;
		case tssAttributeName:
			treatAttributeName(ch, reader, catcher, state);
			break;
		case tssLookEqual:
			if (treatLookEqual(ch, reader, catcher, state))
				return true;
			break;
		case tssLookValue:
			if (treatLookValue(ch, reader, catcher, state))
				return true;
			break;
		case tssLookEndValue:
			treatLookEndValue(ch, reader, catcher, state);
		}
		return false;
	}
	bool treatInComment(wchar_t ch, IReader&reader, IXCatcher&catcher, ParserState&state)
	{
		if (ch == L'-')
		{
			std::wstring helper;
			helper += ch;
			ch = reader.getChar();
			if (ch == L'-')
			{
				helper += ch;
				ch = reader.getChar();
				if (ch == L'>')
				{
					state.state = rsIdle;
					catcher.endComment(state.text);
					state.text.clear();
					return false;
				}
				helper += ch;
			}
			state.text += helper;
		}
		else
			state.text += ch;
		return false;
	}
	bool xiswblank(wchar_t ch)
	{
		return (iswblank(ch) || ch == L'\n' || ch == L'\r');
	}
	std::wstring rtrim(const std::wstring&in)
	{
		if (in.empty())
			return in;
		size_t pos = in.size();

		std::wstring::const_reverse_iterator r = in.rbegin();
		for (; r != in.rend(); r++, pos--)
		{
			if (!xiswblank(*r))
			{
				break;
			}
		}
		return in.substr(0, pos);
	}
	std::wstring trim(const std::wstring&in)
	{
		if (in.empty())
			return in;
		size_t end = in.size();

		std::wstring::const_reverse_iterator r = in.rbegin();
		for (; r != in.rend() && end != 0; ++r, end--)
		{
			if (!xiswblank(*r))
			{
				break;
			}
		}
		if (end == 0)
			return std::wstring(L"");
		std::wstring::const_iterator i = in.begin();
		size_t begin = 0;
		for (; i != in.end(); ++i, ++begin)
		{
			if (!xiswblank(*i))
			{
				break;
			}
		}
		return in.substr(begin, end - begin);
	}

	bool Xereader::parse(IReader & reader, IXCatcher & catcher, bool preserveWhitespace)
	{
		ParserState state;
		wchar_t ch = 0;

		for (;;)
		{
			try
			{
				ch = reader.getChar();
			}
			catch (Exception ex)
			{
				if (wcscmp(ex.getText(), L"EOF"))
				{
					throw ex;
				}
				return true;
			}
			try
			{
				switch (state.state)
				{
				case rsIdle:
				{
					if ((!preserveWhitespace) && xiswblank(ch))
						continue;
					if (treatIdle(ch, reader, catcher, state))
						continue;
					break;
				}
				case rsInText:
				{
					print("text\n");
					if (ch != L'<')
					{
						state.text += ch;
					}
					else
					{
						std::wstring text = preserveWhitespace ? state.text : rtrim(state.text);
						catcher.endText(text);
						state.text.clear();
						state.state = rsLTFound;
					}
					break;
				}
				case rsLTFound:
				{
					print("LT found\n");

					if (treatLTFound(ch, reader, catcher, state))
						continue;
					break;
				}
				case rsInStartTag:
					print("start tag %d\n", state.tagSubState);

					if (treatInStartTag(ch, reader, catcher, state))
						continue;
					break;
				case rsInEndTag:
					print("end tag\n");

					if (ch == L'>')
					{
						catcher.endEndTag(state.nspace, state.tag);
						state.tag.clear();
						state.nspace.clear();
						state.state = rsIdle;
					}
					state.tag += ch;
					break;
				case rsInComment:
					print("comment\n");

					treatInComment(ch, reader, catcher, state);
					break;
				case rsEntityDef:
					treatEntityDef(ch, reader, catcher, state);
					break;
				default:
					throw UnicodeException(L"state not supported %d\n", state);
				}
			}
			catch (XMLException ex)
			{
				ex.lineNumber = reader.currentLineNumber();
				throw ex;
			}
		}

		return false;
	}
}
