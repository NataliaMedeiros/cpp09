#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	loadDataBase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _rates(other._rates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_rates = other._rates;
	return (*this);
}

BitcoinExchange::~BitcoinExchange(){}

static std::string trim(const std::string& data)
{
	std::string::size_type start = data.find_first_not_of(" \t\r\n");

	if (start == std::string::npos)
		return "";

	std::string::size_type end = data.find_last_not_of(" \t\r\n");
	return data.substr(start, end - start + 1);
}


static bool isDayValid(int d, int m, int y)
{
	int maxDays;

	if (m == 2)
	{
		bool isLeapYear = (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0));
		maxDays = (isLeapYear ? 29 : 28);
	}
	else if (m <=7)
		maxDays = ((m % 2 == 1) ? 31 : 30);
	else
		maxDays = ((m % 2 == 0) ? 31 : 30);

	return (d > 0 && d <= maxDays);
}

static void validateDate(const std::string& date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		throw std::runtime_error("Error: bad input => " + date);
	for (int i = 0; i < static_cast<int>(date.size()); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			throw std::runtime_error("Error: bad input => " + date);
	}

	std::string year, month, day;
	std::stringstream ss(date);
	std::getline(ss, year, '-');
	std::getline(ss, month, '-');
	std::getline(ss, day, '-');
	int y = std::stoi(year);
	int m = std::stoi(month);
	int d = std::stoi(day);

	if (y <= 0 || m < 1 || m > 12
		|| !isDayValid(d, m, y))
		throw std::runtime_error("Error: bad input => " + date);
}

static void validateHeader(const std::string &header, const std::string &expectedHeader)
{
	if (trim(header) != expectedHeader)
		throw std::runtime_error("Error: invalid header");
}

static double getAndValidadeValue(std::string valueStr)
{
	if (valueStr.empty())
		throw std::runtime_error("Error: bad input => " + valueStr);
	size_t consumed = 0;
	for (size_t i = 0; i < valueStr.size(); i++)
	{
		if (isalpha(static_cast<unsigned char>(valueStr[i])))
			throw std::runtime_error("Error: bad input => " + valueStr);
	}
	double value;
	try
	{
		value = std::stod(valueStr, &consumed);
	}
	catch (const std::exception&)
	{
		throw std::runtime_error("Error: bad input => " + valueStr);
	}
	if (consumed != valueStr.size() || !std::isfinite(value))
		throw std::runtime_error("Error: bad input => " + valueStr);
	if(value < 0)
		throw std::runtime_error("Error: not a positive number.");
	if (value > 1000)
		throw std::runtime_error("Error: too large a number.");
	return value;
}

void BitcoinExchange::printRates()
{
	for (auto const &_rates : _rates)
	{
		std::cout << _rates.first << " is: "
		<< std::fixed << std::setprecision(2)
		<< _rates.second << "\n";
	}
}

void BitcoinExchange::loadDataBase(std::string fileName)
{
	std::ifstream file(fileName);
	if(!file.is_open())
	throw std::runtime_error("Error: Data file didn't open correctly");

	std::string header;
	getline(file, header);
	validateHeader(header, "date,exchange_rate");

	std::string line;
	while(getline(file, line))
	{
		std::string::size_type pos = line.find(',');
		if (pos == std::string::npos)
			throw std::runtime_error("Error: Invalid CSV line");
		std::string date = trim(line.substr(0, pos));
		validateDate(date);
		std::string valueStr = trim(line.substr(pos + 1));
		double value;
		size_t consumed = 0;
		try
		{
			value = std::stod(valueStr, &consumed);
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("Error: Invalid value");
		}
		if (consumed != valueStr.size() || !std::isfinite(value) || value < 0)
			throw std::runtime_error("Error: Invalid value");
		_rates.insert({date, value});
	}
	file.close();
}

void BitcoinExchange::exchangeValue(const std::string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
		throw std::invalid_argument("Error: could not open file.");
	if (_rates.empty())
		throw std::runtime_error("Error: Data file is empty");

	std::string header;
	getline(file, header);
	validateHeader(header, "date | value");

	std::cout << std::setprecision(10);

	std::string line;
	while(getline(file, line))
	{
		try
		{
			std::string::size_type pos = line.find("|");
			if (pos == std::string::npos)
    			throw std::runtime_error("Error: bad input => " + line);
			std::string date = trim(line.substr(0, pos));
			validateDate(date);
			double value = getAndValidadeValue(trim(line.substr(pos + 1)));
			auto it =_rates.lower_bound(date);

			if(it == _rates.begin() && it->first != date)
				std::cerr << "Error: no valid date found." << std::endl;
			else
			{
				if(it == _rates.end() || it->first != date)
					it--;
				std::cout << date << " => " << value << " = " << value * it->second << std::endl;
			}
		}
		catch(const std::exception &e){
			std::cerr << e.what() << std::endl;
		}
	}
}

// static int getCurrentYear()
// {
// 	std::time_t now = std::time(NULL);
// 	std::tm* localTime = std::localtime(&now);

// 	int currentYear = localTime->tm_year + 1900;

// 	return (currentYear);
// }
