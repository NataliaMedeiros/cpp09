#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	LoadDataBase("data.csv");
	//Here I need to call a function that you load the
	//data base and fill the map (_rates)
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _rates(other._rates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_rates = other._rates;
	return (*this);
}

BitcoinExchange::~BitcoinExchange(){}

void BitcoinExchange::LoadDataBase(std::string fileName)
{
	std::string line;
	std::ifstream file(fileName);

	if(!file.is_open())
		throw std::runtime_error("Error: Data file didn't open correctly");
	getline(file, line);
	while(getline(file, line))
	{
		if (line.empty())
			getline(file, line);
		//aqui eu preciso pensar em edge case relacionados
		//a data and vale
		//trim - to make sure there's no extra space
		std::cout << line << std::endl;
		int pos = line.find(',');
		std::string date = line.substr(0, pos);
		std::cout << "****|" << date << "|" << std::endl;
		double value = std::stod(line.substr(pos + 1));
		std::cout << "value: |" << value <<"|" << std::endl;
		_rates.insert({date, value});
	}
	file.close();
}
