//  IReader.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
class IReader
{
public:
	virtual wchar_t getChar() = 0;
	virtual wchar_t peekChar() const = 0;
	virtual bool nextChar() = 0;
	virtual int currentLineNumber() const = 0;
};
