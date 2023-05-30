#include "printToFile.hpp"

/******************************************************************************

File        : printToFile.cpp

Date        : 2023

Author      : Joey Keputa

Description : Handles printing the fps to a file.
******************************************************************************/

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