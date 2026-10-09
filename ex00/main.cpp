#include "BitcoinExchange.hpp"

int main(int argc, char** argv)
{
	if (argc != 2){
		std::cout << "Wrong amount of arguments" << std::endl;
		return 1;
	}
	try{
		BitcoinExchange btc;
		btc.exchangeValue(argv[1]);
		//chamar algo que lide com o input
		//validade input
	}
	catch(const std::exception &e){
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
