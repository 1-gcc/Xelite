//  exports.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
// (C) 2017 Copyright Martin Brunn
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <wchar.h>
#include <locale.h>
#include <string.h>

#include <errno.h>
#include <mm_malloc.h>
#include "DomCatcher.h"
#include "Xereader.h"
#include "FileReader.h"
#include "Exception.h"
#include "DOMBuilder.h"
#include "XMLWriter.h"
#include "UTFConverter.h"
#include "Alloc.h"

using namespace Xelite;
extern "C"
{
	void vminfo();
	IXMLWriter * createXMLWriter(std::wostream&ostr, bool pretty);
	IDOMBuilder * createDOMBuilder();
	cardac::converter::IMultiByteWideStringConverter * createConverter(const char*type);
	IDOM* parseXMLUTF8Stream(std::istream&is, bool preserveWhitespace);
	IDOM* parseXMLUTF8String(std::string&str, bool preserveWhitespace);
	IDOM* parseXMLUTF8File(const char*fileName, bool preserveWhitespace);
	void deleteDOM(IDOM*pDOM);
	void deleteDOMBuilder(IDOMBuilder*pDOMBuilder);
	void deleteXMLWriter(IXMLWriter*pXMLWriter);
	void deleteConverter(cardac::converter::IMultiByteWideStringConverter * converter);
}
std::map<void*, size_t, std::less<void*>, cardac::Alloc<std::pair<void*, size_t> > > memSize;

size_t allMemory = 0;




void vminfo()
{
	std::wcerr << "Memory:" << allMemory << L"\n";
	/*	int bufsz = 1024;
	char buffer[bufsz];
	FILE * fp = fopen("/proc/self/status", "r");
	if (fp)
	{
	while (fgets(buffer, bufsz, fp))
	{
	if (!strncmp(buffer, "Vm", 2))
	std::wcout << buffer;
	}
	fclose(fp);
	}
	else
	{
	std::wcout << "errno:" << errno << "\n";
	}
	*/
}
void * operator new[](size_t size) throw(std::bad_alloc)
{
	//std::wcerr << L"new[" << size << L"]\n";

	allMemory += size;
	void * p = malloc(size);
	memSize[p] = size;
	return p;
};
void * operator new(size_t size)
{
	//std::wcerr << L"new:" << size << L"\n";
	allMemory += size;
	void * p = malloc(size);
	memSize[p] = size;
	return p;
};
void operator delete(void*p)
{
	//std::wcerr << L"delete\n";

	if (!p)
		return;
	std::map<void*, size_t>::iterator i = memSize.find(p);
	if (i == memSize.end())
	{
		return;
	}
	allMemory -= i->second;
	memSize.erase(i);
	return free(p);
}
void operator delete[](void*p)
{
	if (!p)
		return;
	std::map<void*, size_t>::iterator i = memSize.find(p);
	if (i == memSize.end())
	{
		return;
	}
	allMemory -= i->second;
	memSize.erase(i);
	return free(p);
}
IDOM* parseXMLUTF8String(std::string&str, bool preserveWhitespace)
{
	std::istringstream is(str.c_str());
	return parseXMLUTF8Stream(is, preserveWhitespace);
}
IDOM* parseXMLUTF8File(const char*fileName, bool preserveWhitespace)
{
	std::ifstream file(fileName);
	return parseXMLUTF8Stream(file, preserveWhitespace);
}
void deleteDOMBuilder(IDOMBuilder * pDOMBuilder)
{
	delete pDOMBuilder;
}
void deleteXMLWriter(IXMLWriter * pXMLWriter)
{
	delete pXMLWriter;
}
void deleteConverter(cardac::converter::IMultiByteWideStringConverter * converter)
{
	delete converter;
}
void deleteDOM(IDOM * pDOM)
{
	delete pDOM;
}
IXMLWriter * createXMLWriter(std::wostream&ostr, bool pretty)
{
	return new XMLWriter(ostr, pretty);
}
IDOMBuilder * createDOMBuilder()
{
	return new DOMBuilder();
}
cardac::converter::IMultiByteWideStringConverter * createConverter(const char * type)
{
	return new cardac::converter::UTFConverter();
}
IDOM* parseXMLUTF8Stream(std::istream&is, bool preserveWhitespace)
{

	try
	{
		std::wcerr << L"vor DOMBuilder\n";
		vminfo();
		DOMBuilder domBuilder;
		DomCatcher dom(&domBuilder);
		Xereader xer;
		FileReader filereader(is);
		std::wcerr << L"nach FileReader\n";
		vminfo();

		std::wcerr << L"vor getDOM 1\n";
		IDOMPtr pDOM = dom.getDOM();
		if (pDOM && pDOM->isA() == dtDOMTree)
		{
		}
		else
		{
			throw Exception(L"No DOMTree");
		}
		setlocale(LC_ALL, "");
		try
		{
			xer.parse(filereader, dom, preserveWhitespace);
		}
		catch (XMLException ex)
		{
			wprintf(L"XMLException:line %d <%S>\n", ex.lineNumber, ex.getText());
			return NULL;
		}
		catch (Exception ex)
		{
			wprintf(L"Exception:<%S>\n", ex.getText());
			if (wcscmp(ex.getText(), L"EOF"))
			{
				fwprintf(stderr, L"Terminating\n");
				return(NULL);
			}
			else
			{
				IDOMPtr pDOM3 = dom.getDOM();
				if (pDOM3 && pDOM3->isA() == dtDOMTree)
				{
					return pDOM3->clone();
				}

			}
		}
		IDOMPtr pDOM3 = dom.getDOM();
		if (pDOM3 && pDOM3->isA() == dtDOMTree)
		{
			std::wcerr << L"vor clone\n";
			vminfo();
			IDOM* pDOM = pDOM3->clone();
			std::wcerr << L"nach clone\n";
			vminfo();
			return pDOM;

		}
	}
	catch (XMLException ex)
	{
		wprintf(L"[%d]<%S>\n", ex.lineNumber, ex.getText());
		return(NULL);
	}
	catch (Exception ex)
	{
		wprintf(L"Exception:<%S>\n", ex.getText());
	}
	catch (std::exception e)
	{
		std::wcerr << L"std exception\n";
	}
	std::wcerr << L"return NULL\n";
	return NULL;
}
