//  main.cpp
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#include "IDOM.h"
#include "IXMLWriter.h"
#include "IWSConverter.h"
#include "Alloc.h"

#include <dlfcn.h>

using namespace Xelite;


#ifndef RTLD_NOW
#define RTLD_NOW 2
#define RTLD_GLOBAL 0x0100
#endif
std::map<void*, size_t, std::less<void*>, cardac::Alloc<std::pair<void*, size_t> > > memSize;

size_t allMemory = 0;
void vminfo()
{
	std::wcerr << "Memory:" << allMemory << L"\n";
}
void * operator new[](size_t size) throw()
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

using namespace cardac::converter;
class Main
{
public:
	CreateConverter createConverter;
	DeleteConverter deleteConverter;
	//void(*vminfo)();
	ParseXMLUTF8Stream parseXMLUTF8Stream;
	ParseXMLUTF8File parseXMLUTF8File;
	DeleteDOM deleteDOM;
	CreateXMLWriter createXMLWriter;
	DeleteXMLWriter deleteXMLWriter;
	void * lib = NULL;
	Main()
	{
		lib = dlopen("xelite.so", RTLD_NOW | RTLD_GLOBAL);
		if (lib == NULL)
		{
			char* err = dlerror();
			fprintf(stderr, "dlopen error %s\n", err);
		}
	}
	bool init()
	{
		/*vminfo = (void(*)())dlsym(lib, "vminfo");
		if (!vminfo)
		{
			fprintf(stderr, "vminfo missing");
			return false;
		}*/
		deleteConverter = (DeleteConverter)dlsym(lib, "deleteConverter");
		if (!deleteConverter)
		{
			fprintf(stderr, "deleteConverter missing");
			return false;
		}
		parseXMLUTF8Stream = (ParseXMLUTF8Stream)dlsym(lib, "parseXMLUTF8Stream");
		if (!parseXMLUTF8Stream)
		{
			fprintf(stderr, "parseXMLUTF8Stream missing");
			return false;
		}
		parseXMLUTF8File = (ParseXMLUTF8File)dlsym(lib, "parseXMLUTF8File");
		if (!parseXMLUTF8File)
		{
			fprintf(stderr, "parseXMLUTF8File missing");
			return false;
		}
		deleteDOM = (DeleteDOM)dlsym(lib, "deleteDOM");
		if (!deleteDOM)
		{
			fprintf(stderr, "deleteDOM missing");
				return false;
		}
		createXMLWriter = (CreateXMLWriter)dlsym(lib, "createXMLWriter");
		if (!createXMLWriter)
		{
			fprintf(stderr, "createXMLWriter missing");
				return false;
		}
		deleteXMLWriter = (DeleteXMLWriter)dlsym(lib, "deleteXMLWriter");
		if (!deleteXMLWriter)
		{
			fprintf(stderr, "deleteXMLWriter missing");
				return false;
		}
		return true;
	}
	~Main()
	{

		if (lib)
			dlclose(lib);
		lib = NULL;

	}
	int run(int argc,char**argv)
	{
		IXMLWriter * writer = createXMLWriter(std::wcout, true);
		if (argc == 1)
		{
			std::wcerr << L"vor parseXMLUtf8Stream\n";
			vminfo();
			IDOM * pDOM = parseXMLUTF8Stream(std::cin, false);
			std::wcerr << L"nach parseXMLUtf8Stream\n";
			vminfo();
			if (pDOM)
			{
				pDOM->getDocument()->write(writer);

				vminfo();
				deleteDOM(pDOM);
				vminfo();
			}

		}
		while (--argc)
		{
			IDOM * pDOM = parseXMLUTF8File(*++argv, false);
			if (pDOM)
			{
				pDOM->getDocument()->write(writer);
				deleteDOM(pDOM);
			}
		}
		deleteXMLWriter(writer);
		return 0;
	}
};
int main(int argc, char**argv)
{
	Main main;
	if (main.init())
		main.run(argc, argv);
	return 0;
}
