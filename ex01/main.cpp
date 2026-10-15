#include "RPN.hpp"

int main(int argv, char** argc)
{
	if (argv != 2)
		std::cerr << "Error: Check arguments" << std::endl;
	try{
		RPN rpn(argc[1]);
	}
	catch (const std::exception &e){
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}
