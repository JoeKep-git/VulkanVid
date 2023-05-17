#pragma once

#include <iostream>
#include <fstream>

using namespace std;

class PrintToFile
{
private:
	ofstream myFile;


public:
	PrintToFile();
	void printingMethod(int fps);
	void closeFile();
};