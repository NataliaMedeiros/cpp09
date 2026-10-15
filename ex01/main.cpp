#include "RPN.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: Check arguments" << std::endl;
		return (1);
	}
	try{
		RPN rpn(argv[1]);
	}
	catch (const std::exception &e){
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}
