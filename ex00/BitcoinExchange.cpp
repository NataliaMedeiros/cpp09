#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	loadDataBase("data.csv");
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

static int getCurrentYear()
{
	std::time_t now = std::time(NULL);
	std::tm* localTime = std::localtime(&now);

	int currentYear = localTime->tm_year + 1900;

	return (currentYear);
}

static std::string trim(std::string data)
{
	std::string dataTrimmed = "";
	int start = 0;
	int end = data.size() - 1;

	while(data[start] == ' '){
		start++;
	}
	while(data[end] == ' '){
		end--;
	}
	dataTrimmed = data.substr(start, end + 1);
	return (dataTrimmed);
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

	return (d <= maxDays);
}

static bool isDateValid(const std::string& date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (int i = 0; i < static_cast<int>(date.size()); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(static_cast<unsigned char>(date[i])))
			return(false);
	}

	std::string year, month, day;

	std::stringstream ss(date);
	std::getline(ss, year, '-');
	std::getline(ss, month, '-');
	std::getline(ss, day, '-');

	int y = stoi(year);
	int m = stoi(month);
	int d = stoi(day);

	if (y <= 0 || y > getCurrentYear())
		return (false);
	if (m < 1 || m > 12)
		return (false);
	if (!isDayValid(d, m, y))
		return (false);
	return (true);
}

//checar se precisa pegar 2 casas decimais e arrumar
void BitcoinExchange::loadDataBase(std::string fileName)
{
	std::string line;
	std::ifstream file(fileName);

	if(!file.is_open())
		throw std::runtime_error("Error: Data file didn't open correctly");
	getline(file, line);
	while(getline(file, line))
	{
		std::string::size_type pos = line.find(',');
		if (pos == std::string::npos)
			throw std::runtime_error("Error: Invalid CSV line");
		std::string date = trim(line.substr(0, pos));
		if (!isDateValid(date))
			throw std::runtime_error("Error: Date is invalid");
		std::string valueStr = trim(line.substr(pos + 1));
		size_t consumed;
		double value = std::stod(valueStr, &consumed);
		if (consumed != valueStr.size() || !std::isfinite(value))
			throw std::runtime_error("Error: Invalid value");
		// std::cout << "----> date: " << date << "--->value: " << value << std::endl;
		_rates.insert({date, value});
	}
	for (auto _rates : _rates)
	{
		std::cout << _rates.first << " is: " << _rates.second << "\n";
	}
	file.close();
}

static bool isHeaderValid(const std::string& header){
	if (header.empty())
		return (false);
	if (header != "date | value")
		return (false);
	return (true);
}

static double getAndValidadeValue(std::string valueStr)
{
	size_t consumed = 0;
	double value = std::stod(valueStr, &consumed);
	if (consumed != valueStr.size() || !std::isfinite(value))
	{
		std::cerr << "Error: bad input => " << valueStr << std::endl;
		return (-1);
	}
	if(value < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return (-1);
	}
	if (value > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return (-1);
	}
	return value;
}

void BitcoinExchange::exchangeValue(const std::string& fileName)
{
	//checks input name
	if (fileName.substr(fileName.find(".")) != ".txt")
		throw std::invalid_argument("Check input format");
	//open input file
	std::ifstream file(fileName);
	//check if input file opened correctly
	if (!file.is_open())
		throw std::invalid_argument("Invalid file");
	//check data inside the file
	std::string header;
	getline(file, header);
	//validade is header is at the correct format
	if (!isHeaderValid(header))
		throw std::invalid_argument("Invalid header");
	std::string line;
	while(getline(file, line))
	{
		//preciso lidar com quando o pos nao é encontrado por exemplo
		// na linha em que eu nao coloco nenhum value para a date
		std::string::size_type pos = line.find("|");
		std::string date = trim(line.substr(0, pos));
		//vou precisar criar uma funcao especifica para input
		if (!isDateValid(date))
			std::cout << "--Error" << std::endl;
		std::string valueStr = trim(line.substr(pos + 1));
		//se o value for invalido ele vai retornar -1
		//e entao preciso pensar no que vou fazer com esse -1
		double value = getAndValidadeValue(valueStr);
		// std::cout << "|" << value << "|" << std::endl;
		//TODO estudar melhor map e refazer essa parte
		if (value != -1)
		{
			auto it =_rates.lower_bound(date);
			double result = 0;

			if(it == _rates.begin() && it->first != date)
				std::cout << "Error: no valid date found." << std::endl;
			else
			{
				if (it->first != date)
					it--;
				result = value * it->second;
				std::cout << date << " => " << value << " = " << result << std::endl;
			}
		}
	}
}
