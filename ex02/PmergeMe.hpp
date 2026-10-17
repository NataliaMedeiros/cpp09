#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <vector>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

class PmergeMe
{
	private:
		std::vector<int> _input;
		std::vector<int> _vecResult;
		std::deque<int>  _deqResult;

		// void fordJohnsonVector(std::vector<std::size_t>&);
		std::vector<std::size_t> fordJohnsonVector(const std::vector<std::size_t>& ids) const;

		// void fordJohnsonDeque(std::deque<std::size_t>&);
		std::deque<std::size_t> fordJohnsonDeque(const std::deque<std::size_t>& ids) const;

	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe& operator=(const PmergeMe& other);
		~PmergeMe();

		double sortWithVector();   // sorts, returns time in microseconds
		double sortWithDeque();    // sorts, returns time in microseconds

		const std::vector<int>& getInput() const;
		const std::vector<int>& getVectorResult() const;
		const std::deque<int>&  getDequeResult() const;

		void parser(int argc, char** argv);
};

#endif
