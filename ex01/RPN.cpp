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

void RPN::solvingRPN(const char* arg)
{
	int n;
	std::string operators = "+-*/";
	int a;
	int b;

	for (int i = 0; arg[i] != '\0'; i++)
	{
		if (isdigit(arg[i]))
		{
			if (arg[i + 1] != ' '&& arg[i + 1] != '\0')
				throw std::runtime_error("Number bigger than 9");
			n = arg[i] - '0';
			nb.push(arg[i] - '0');
		}
		if (operators.find(arg[i]) != std::string::npos)
		{
			if (nb.empty())
				throw std::runtime_error("Invalid expression");
			b = nb.top();
			nb.pop();
			if (nb.empty())
				throw std::runtime_error("Invalid expression");
			a = nb.top();
			nb.pop();
			switch (arg[i])
			{
				case '+':
					n = a + b;
					break;
				case '-':
					n = a - b;
					break;
				case '*':
					n = a * b;
					break;
				case '/':
					if (b == 0)
						throw std::runtime_error ("Division by zero");
					n = a / b;
					break;
				default:
					break;
			}
			nb.push(n);
		}
	}
	if (nb.size() != 1)
		throw std::runtime_error("Invalid expression");
	printStack(nb);
}
