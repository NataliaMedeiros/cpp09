#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other)
	: _input(other._input), _vecResult(other._vecResult),
	_deqResult(other._deqResult) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other) {
		_input = other._input;
		_vecResult = other._vecResult;
		_deqResult = other._deqResult;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

const std::vector<int>& PmergeMe::getInput() const
{
	return _input;
}

const std::vector<int>& PmergeMe::getVectorResult() const
{
	return _vecResult;
}

const std::deque<int>&  PmergeMe::getDequeResult() const
{
	return _deqResult;
}

std::vector<std::size_t> PmergeMe::fordJohnsonVector(const std::vector<std::size_t>& ids) const
{
	const std::size_t n = ids.size();
	if (n < 2)
		return ids;

	const std::vector<int>& d = _input;
	auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

	std::vector<std::size_t> winners;
	std::vector<std::size_t> loserOf(d.size(), 0);
	for (std::size_t i = 0; i + 1 < n; i += 2) {
		std::size_t a = ids[i], b = ids[i + 1];
		if (cmp(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}
	const bool hasLeftover = (n % 2 == 1);
	const std::size_t leftover = hasLeftover ? ids[n - 1] : 0;

	winners = fordJohnsonVector(winners);

	std::vector<std::size_t> chain;
	chain.push_back(loserOf[winners[0]]);
	chain.insert(chain.end(), winners.begin(), winners.end());

	const std::size_t pairCount = winners.size();
	const std::size_t pendCount = pairCount + (hasLeftover ? 1 : 0);
	std::size_t totalInserted = 1, prevLimit = 1, currLimit = 3;
	while (totalInserted < pendCount) {
		const std::size_t groupEnd = std::min(currLimit, pendCount);
		for (std::size_t k = groupEnd - 1; k >= totalInserted; --k) {
			std::size_t l;
			std::vector<std::size_t>::iterator bound;
			if (k < pairCount) {
				const std::size_t w = winners[k];
				l = loserOf[w];
				bound = std::find(chain.begin(), chain.end(), w);
			}
			else
			{
				l = leftover;
				bound = chain.end();
			}
			std::vector<std::size_t>::iterator pos = std::lower_bound(chain.begin(), bound, l, cmp);
			chain.insert(pos, l);
		}
		totalInserted = groupEnd;
		const std::size_t next = currLimit + 2 * prevLimit;
		prevLimit = currLimit;
		currLimit = next;
	}

	return chain;
}

std::deque<std::size_t> PmergeMe::fordJohnsonDeque(const std::deque<std::size_t>& ids) const
{
	const std::size_t n = ids.size();
	if (n < 2)
		return ids;

	const std::vector<int>& d = _input;
	auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

	std::deque<std::size_t> winners;
	std::vector<std::size_t> loserOf(d.size(), 0);
	for (std::size_t i = 0; i + 1 < n; i += 2) {
		std::size_t a = ids[i], b = ids[i + 1];
		if (cmp(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}
	const bool hasLeftover = (n % 2 == 1);
	const std::size_t leftover = hasLeftover ? ids[n - 1] : 0;

	winners = fordJohnsonDeque(winners);

	std::deque<std::size_t> chain;
	chain.push_back(loserOf[winners[0]]);
	chain.insert(chain.end(), winners.begin(), winners.end());

	const std::size_t pairCount = winners.size();
	const std::size_t pendCount = pairCount + (hasLeftover ? 1 : 0);
	std::size_t totalInserted = 1, prevLimit = 1, currLimit = 3;
	while (totalInserted < pendCount) {
		const std::size_t groupEnd = std::min(currLimit, pendCount);
		for (std::size_t k = groupEnd - 1; k >= totalInserted; --k) {
			std::size_t l;
			std::deque<std::size_t>::iterator bound;
			if (k < pairCount)
			{
				const std::size_t w = winners[k];
				l = loserOf[w];
				bound = std::find(chain.begin(), chain.end(), w);
			}
			else
			{
				l = leftover;
				bound = chain.end();
			}
			std::deque<std::size_t>::iterator pos = std::lower_bound(chain.begin(), bound, l, cmp);
			chain.insert(pos, l);
		}
		totalInserted = groupEnd;
		const std::size_t next = currLimit + 2 * prevLimit;
		prevLimit = currLimit;
		currLimit = next;
	}
	return chain;
}

double PmergeMe::sortWithVector()
{
	auto start = std::chrono::steady_clock::now();
	std::vector<std::size_t> ids;
	for (std::size_t i = 0; i < _input.size(); ++i)
		ids.push_back(i);

	const std::vector<std::size_t> sorted = fordJohnsonVector(ids);

	_vecResult.clear();
	for (std::vector<std::size_t>::const_iterator it = sorted.begin(); it != sorted.end(); ++it)
		_vecResult.push_back(_input[*it]);

	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double, std::micro>(end - start).count();
}

double PmergeMe::sortWithDeque()
{
	auto start = std::chrono::steady_clock::now();
	std::deque<std::size_t> ids;
	for (std::size_t i = 0; i < _input.size(); ++i)
		ids.push_back(i);


		const std::deque<std::size_t> sorted = fordJohnsonDeque(ids);

	_deqResult.clear();
	for (std::deque<std::size_t>::const_iterator it = sorted.begin(); it != sorted.end(); ++it)
		_deqResult.push_back(_input[*it]);

    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void PmergeMe::parser(int argc, char** argv)
{
	_input.clear();
	for (int i = 1; i < argc; ++i) {
		char* end;
		long v = std::strtol(argv[i], &end, 10);
		if (*argv[i] == '\0' || *end != '\0' || v <= 0 || v > INT_MAX)
			throw std::runtime_error("Error");
		_input.push_back(static_cast<int>(v));
	}
}

// Ford-Johnson (merge-insertion sort): sorts using few comparisons.
// 1. Pair up the elements and compare each pair (winner = larger).
// 2. Recursively sort the winners.
// 3. Insert the losers back, with binary search, in Jacobsthal order
//    (an unpaired element counts as the last loser).

// Group limits follow the Jacobsthal numbers (3, 5, 11, 21...).
