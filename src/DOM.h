//  DOM.h
// (C) Copyright 2017 by Martin Brunn  see License.txt for details
//
#pragma once

#include "IDOM.h"

namespace Xelite
{

	class DOM : public IDOM
	{
	public:
		DOM();
		~DOM();
		virtual IProlog * getProlog() const;
		bool addProlog(IProlog*prolog);
	};

}
