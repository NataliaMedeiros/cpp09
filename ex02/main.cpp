#include "PmergeMe.hpp"

template <typename C>
static void show(const C& c)
{
    std::size_t i = 0;
    for (typename C::const_iterator it = c.begin(); it != c.end() && i < 20; ++it, ++i)
        std::cout << *it << ' ';
    std::cout << (c.size() > 20 ? "[...]" : "") << '\n';
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: Insufficient number of arguments\n";
		return 1;
	}
	try{
		PmergeMe sorter;
		sorter.parser(argc, argv);

		double tVec = sorter.sortWithVector();
		double tDeq = sorter.sortWithDeque();

		std::cout << "Before: "; show(sorter.getInput());
		std::cout << "After: "; show(sorter.getVectorResult());
		std::cout << "Time to process a range of " << sorter.getInput().size()
				<< " elements with std::vector : " << tVec << " us\n";
		std::cout << "Time to process a range of " << sorter.getInput().size()
				<< " elements with std::deque  : " << tDeq << " us\n";

		if (!std::is_sorted(sorter.getVectorResult().begin(), sorter.getVectorResult().end())
			|| !std::equal(sorter.getVectorResult().begin(), sorter.getVectorResult().end(),
						sorter.getDequeResult().begin()))
		{
			std::cerr << "BUG: result not sorted or containers disagree\n";
			return 1;
		}
	}
	catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
