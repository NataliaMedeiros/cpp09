#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <time.h>

class BitcoinExchange
{
	private:
		std::map<std::string, double> _rates;
		//the key is the date that will be a string and
		// the exchange_rate is the value
		void loadDataBase(std::string fileName);
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		void exchangeValue(const std::string& fileName);
};

#endif
