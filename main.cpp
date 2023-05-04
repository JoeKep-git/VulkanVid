/******************************************************************************

File        : main.cpp

Date        : 2023

Author      : Joey Keputa

Description : This file creates an application object and then calls the
				method to run vulkan.

History     :	- v1.00 Adding the code below
				
******************************************************************************/

#include "application.hpp"
#include<cstdlib>
#include<iostream>
#include<stdexcept>

using namespace std;

int main()
{
	lve::FirstApplication app{};
	
	try 
	{
		app.run();
	}
	catch (const exception& e)
	{
		cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}