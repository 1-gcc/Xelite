//  IWSConverter.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include <string>
namespace cardac
{
	namespace converter
	{
		class IWSConverter
		{
		public:
			virtual std::wstring convert(const std::wstring&source) = 0;

		};

		class IMultiByteWideStringConverter
		{
		public:
			virtual ~IMultiByteWideStringConverter()
			{

			}
			virtual std::wstring convertMultiByteToWideString(const std::string&utf8String) = 0;
			virtual std::string convertWideStringToMultiByte(const std::wstring&utf16String) = 0;
		};
		typedef IWSConverter * (*IWSConverterCreate)(const char * type);
		typedef void(*IWSConverterDelete)(IWSConverter *);
		typedef IMultiByteWideStringConverter * (*CreateConverter)(const char*type);
		typedef void(*DeleteConverter)(IMultiByteWideStringConverter * converter);

	}
}
