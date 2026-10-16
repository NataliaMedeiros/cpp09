#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const std::vector<int>& input) : _input(input) {}
PmergeMe::PmergeMe(const PmergeMe& other)
    : _input(other._input), _vecResult(other._vecResult), _deqResult(other._deqResult) {}
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

const std::vector<int>& PmergeMe::getInput() const        { return _input; }
const std::vector<int>& PmergeMe::getVectorResult() const { return _vecResult; }
const std::deque<int>&  PmergeMe::getDequeResult() const  { return _deqResult; }

// ids = indices into _input, sorted in place by the value they point to.
template <typename Cont>
void PmergeMe::fordJohnson(Cont& ids) const
{
    const std::size_t n = ids.size();
    if (n < 2) return;

    const std::vector<int>& d = _input;
    auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

    // 1. Pair up: winner = bigger, loser = smaller
    Cont winners;
    std::vector<std::size_t> loserOf(d.size(), 0);
    for (std::size_t i = 0; i + 1 < n; i += 2) {
        std::size_t a = ids[i], b = ids[i + 1];
        if (cmp(a, b)) std::swap(a, b);
        winners.push_back(a);
        loserOf[a] = b;
    }
    const bool hasStraggler = (n % 2 == 1);
    const std::size_t straggler = hasStraggler ? ids[n - 1] : 0;

    // 2. Recursively sort the winners
    fordJohnson(winners);

    // 3. Main chain: b1 (free), then the sorted winners
    Cont chain;
    chain.push_back(loserOf[winners[0]]);
    chain.insert(chain.end(), winners.begin(), winners.end());

    // 4. Insert b2..bm in Jacobsthal order (3,2 | 5,4 | 11,10,...,6 | ...)
    const std::size_t m = winners.size();
    std::size_t done = 1, jPrev = 1, jCur = 3;
    while (done < m) {
        const std::size_t top = std::min(jCur, m);
        for (std::size_t k = top; k > done; --k) {
            const std::size_t w = winners[k - 1];
            const std::size_t b = loserOf[w];
            typename Cont::iterator bound = std::find(chain.begin(), chain.end(), w);
            typename Cont::iterator pos = std::lower_bound(chain.begin(), bound, b, cmp);
            chain.insert(pos, b);
        }
        done = top;
        const std::size_t next = jCur + 2 * jPrev;
        jPrev = jCur;
        jCur = next;
    }

    // 5. Straggler: binary search the whole chain
    if (hasStraggler)
        chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler, cmp), straggler);

    ids = std::move(chain);
}

template <typename IdCont, typename OutCont>
double PmergeMe::sortAndTime(OutCont& out) const
{
	//cria ids com os indices
    IdCont ids;
    for (std::size_t i = 0; i < _input.size(); ++i)
		ids.push_back(i);   // not timed

    auto start = std::chrono::steady_clock::now();
    fordJohnson(ids);
    auto end = std::chrono::steady_clock::now();

    out.clear();
    for (typename IdCont::const_iterator it = ids.begin(); it != ids.end(); ++it)
        out.push_back(_input[*it]);

    return std::chrono::duration<double, std::micro>(end - start).count();
}

double PmergeMe::sortWithVector()
{
    return sortAndTime<std::vector<std::size_t> >(_vecResult);
}

double PmergeMe::sortWithDeque()
{
    return sortAndTime<std::deque<std::size_t> >(_deqResult);
}

void PmergeMe::parser(int argc, char** argv)
{
	_input.clear();
	for (int i = 1; i < argc; ++i) {
		char* end;
		long v = std::strtol(argv[i], &end, 10);
		if (*argv[i] == '\0' || *end != '\0' || v <= 0 || v > INT_MAX) {
			std::cerr << "Error\n";
			return ;
		}
		_input.push_back(static_cast<int>(v));
	}
}
