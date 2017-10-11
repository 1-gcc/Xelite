//  UTFConverter.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once
#include <string>
#include "IWSConverter.h"

namespace cardac
{
	namespace converter
	{
		class UTFConverter : public IMultiByteWideStringConverter
		{
		public:
			UTFConverter();
			~UTFConverter();
			virtual std::wstring convertMultiByteToWideString(const std::string&utf8String);
			virtual std::string convertWideStringToMultiByte(const std::wstring&utf16String);
		};

	}
}
