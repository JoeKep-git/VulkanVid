#include "printToFile.hpp"

PrintToFile::PrintToFile()
{
	myFile.open("fpsPolygonsTest.txt");
}

void PrintToFile::printingMethod(int fps)
{
	myFile << fps << endl;
}

void PrintToFile::closeFile()
{
	myFile.close();
}