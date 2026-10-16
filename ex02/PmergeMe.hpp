#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <vector>
#include <algorithm>
#include <chrono>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>

class PmergeMe
{
	private:
		std::vector<int> _input;
		std::vector<int> _vecResult;
		std::deque<int>  _deqResult;

		template <typename Cont>
		void fordJohnson(Cont& ids) const;

		template <typename IdCont, typename OutCont>
		double sortAndTime(OutCont& out) const;
	public:
		PmergeMe();
		explicit PmergeMe(const std::vector<int>& input);
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
