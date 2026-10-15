#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const char* arg)
{
	if (!isArgValid(arg))
		throw std::invalid_argument("invalid argument");
	solvingRPN(arg);
}

RPN::RPN(const RPN& other) : nb(other.nb) {}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
		nb = other.nb;
	return (*this);
}

RPN::~RPN(){}

bool RPN::isArgValid(const char* arg)
{
	if (arg[0] == '\0')
		return (false);
	std::string operators = "+-*/";

	for (int i = 0; arg[i] != '\0'; i++)
	{
		if (!isdigit(arg[i]) && arg[i] != ' '
			&& operators.find(arg[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

static void printStack(std::stack<int> s)
{
    if (s.empty())
        return;

    int x = s.top();

    s.pop();

    printStack(s);

    std::cout << x << std::endl;;
}

static int getNumber(const std::stack<int>& nb)
{
	if (nb.empty())
		throw std::runtime_error("Invalid expression");
	return (nb.top());
}

static int calculate(char arg, int a, int b)
{
	switch (arg)
	{
		case '+':
			return (a + b);
		case '-':
			return (a - b);
		case '*':
			return (a * b);
		case '/':
			if (b == 0)
				throw std::runtime_error ("Division by zero");
			return (a / b);
		default:
			throw std::runtime_error ("Invalid operator");
	}
}

void RPN::solvingRPN(const char* arg)
{
	std::string operators = "+-*/";
	int a;
	int b;

	for (int i = 0; arg[i] != '\0'; i++)
	{
		if (isdigit(arg[i]))
		{
			if (arg[i + 1] != ' '&& arg[i + 1] != '\0')
				throw std::runtime_error("Number bigger than 9");
			nb.push(arg[i] - '0');
		}
		if (operators.find(arg[i]) != std::string::npos)
		{
			b = getNumber(nb);
			nb.pop();
			a = getNumber(nb);
			nb.pop();
			nb.push(calculate(arg[i], a, b));
		}
	}
	if (nb.size() != 1)
		throw std::runtime_error("Invalid expression");
	printStack(nb);
}
