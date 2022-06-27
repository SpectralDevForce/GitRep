
#include "writereader.h"


namespace spectral
{

	WriteReader::WriteReader(std::string path,int data)
	{
		std::fstream file;
		file.open(path, std::fstream::in | std::fstream::out | std::fstream::app);

		if (!file.is_open())
		{
			printf("Error open file for write Point.\n");
		}
		else
		{
			SetConsoleCP(1251);
			file << data << "\n";
			SetConsoleCP(866);
		}

		file.close();
	}

}
