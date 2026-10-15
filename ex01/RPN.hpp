#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <cctype>

class RPN
{
	private:
		std::stack<int> nb;
		bool isArgValid(const char* arg);
		void solvingRPN(const char* arg);
	public:
		RPN();
		RPN(const char* arg);
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();
};

#endif
