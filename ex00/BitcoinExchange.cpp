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

static bool isDayValid(std::string day, std::string month, std::string year)
{
	int d = stoi(day);
	int m = stoi(month);
	int y = stoi(year);
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

bool BitcoinExchange::isDateValid(std::string& date)
{
	//valida tamanho e formato da data
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	//valide se exceto os dois '-' o date é composto
	//somente por numeros
	for (int i = 0; i < static_cast<int>(date.size()); i++)
	{
		if (i == 4 || i == 7)
			continue;
		if (!isdigit(date[i]))
			return(false);
	}

	std::string year, month, day;

	std::stringstream ss(date);
	std::getline(ss, year, '-');
	std::getline(ss, month, '-');
	std::getline(ss, day, '-');

	if (stoi(year) <= 0 || stoi(year) > getCurrentYear())
		return (false);

	if (stoi(month) < 1 || stoi(month) > 12)
		return (false);

	if (!isDayValid(day, month, year))
		return (false);

	// std::cout << "year: " << year << std::endl;
	// std::cout << "month : " << month << std::endl;
	// std::cout << "day : " << day << std::endl;
	return (true);
}

void BitcoinExchange::loadDataBase(std::string fileName)
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
		//a data and value
		//trim - to make sure there's no extra space
		//check if date is valid

		int pos = line.find(',');
		std::string date = line.substr(0, pos);
		//antes de entrar na isDateValid é bom ter algum
		//trim garantindo que nao esta sendo pego nenhum espaco
		//extra no comeco ou final
		if (!isDateValid(date))
			throw std::runtime_error("Error: Date is invalid");
		double value = std::stod(line.substr(pos + 1));
		_rates.insert({date, value});
	}
	file.close();
}
