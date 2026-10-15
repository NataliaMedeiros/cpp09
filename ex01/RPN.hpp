#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <stack>
#include <exception>
#include <string.h>
#include <ctype.h>

class RPN
{
	private:
		std::stack<int> nb;
	public:
		RPN();
		RPN(const char* arg);
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();

		bool isArgValid(const char* arg);
		void solvingRPN(const char* arg);
};

#endif
