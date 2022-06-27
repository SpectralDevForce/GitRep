#pragma once
#ifndef _WRITEREADER_H
#define _WRITEREADER_H


#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>


namespace spectral
{

	class WriteReader
	{
	public:
		WriteReader(std::string path, int data);
	};

}


#endif

