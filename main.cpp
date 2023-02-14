#include "application.hpp"
#include<cstdlib>
#include<iostream>
#include<stdexcept>

/*
* got to video 4 10 min 30 in
*/



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