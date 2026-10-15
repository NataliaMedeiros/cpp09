#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <cmath>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _rates;
		void loadDataBase(std::string fileName);
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void exchangeValue(const std::string& fileName);
		void printRates();
};

#endif
