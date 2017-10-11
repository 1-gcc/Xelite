# Xelite
XML Parser C++

Original version 1.0.0.0 of Shared Library xelite.so

A simple XML Parser

creates IDOM *  
	IDOM interface allow access to a DOM tree
	only tested under Ubuntu 16.04 x64 with g++
	
	g++ (Ubuntu 5.4.0-6ubuntu1~16.04.5) 5.4.0 20160609

comes with a test driver and a sample xml file 'loup.xml'

Makefiles use $HOME/obj for intermediate objects,

$(HOME)/deploy/lib for output library 'xelite.so'

$(HOME)/deploy/bin for output test executable 'xelitetest'

bin directory must be in PATH and
lib directory in LD_LIBRARY_PATH in order to let
'make run' work

uses IDOM.h , IXMLWriter.h, IWriter.h of $(HOME)/include/Xelite directory

and Alloc.h , IWSConverter.h of $(HOME)/include directory.

Find these files attached to other projects.
A copy of each file will be included in future versions.
