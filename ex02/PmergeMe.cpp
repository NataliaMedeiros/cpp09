#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}
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
// template <typename Cont>
// void PmergeMe::fordJohnson(Cont& ids) const
// {
//     const std::size_t n = ids.size();
//     if (n < 2) return;

//     const std::vector<int>& d = _input;
//     auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

//     // 1. Pair up: winner = bigger, loser = smaller
//     Cont winners;
//     std::vector<std::size_t> loserOf(d.size(), 0);
//     for (std::size_t i = 0; i + 1 < n; i += 2) {
//         std::size_t a = ids[i], b = ids[i + 1];
//         if (cmp(a, b)) std::swap(a, b);
//         winners.push_back(a);
//         loserOf[a] = b;
//     }
//     const bool hasStraggler = (n % 2 == 1);
//     const std::size_t straggler = hasStraggler ? ids[n - 1] : 0;

//     // 2. Recursively sort the winners
//     fordJohnson(winners);

//     // 3. Main chain: b1 (free), then the sorted winners
//     Cont chain;
//     chain.push_back(loserOf[winners[0]]);
//     chain.insert(chain.end(), winners.begin(), winners.end());

//     // 4. Insert b2..bm in Jacobsthal order (3,2 | 5,4 | 11,10,...,6 | ...)
//     const std::size_t m = winners.size();
//     std::size_t done = 1, jPrev = 1, jCur = 3;
//     while (done < m) {
//         const std::size_t top = std::min(jCur, m);
//         for (std::size_t k = top; k > done; --k) {
//             const std::size_t w = winners[k - 1];
//             const std::size_t b = loserOf[w];
//             typename Cont::iterator bound = std::find(chain.begin(), chain.end(), w);
//             typename Cont::iterator pos = std::lower_bound(chain.begin(), bound, b, cmp);
//             chain.insert(pos, b);
//         }
//         done = top;
//         const std::size_t next = jCur + 2 * jPrev;
//         jPrev = jCur;
//         jCur = next;
//     }

//     // 5. Straggler: binary search the whole chain
//     if (hasStraggler)
//         chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler, cmp), straggler);

//     ids = std::move(chain);
// }

//OutCont means Output Container e é o tipo do container de saida
//no caso desse exercicio o OutCont ou é o out daindo para
//vector<int> as _vecResult ou para deque<int> saindo para o deqResult
// template <typename IdCont, typename OutCont>
// double PmergeMe::sortAndTime(OutCont& out) const
// {
// 	//inicia a contagem do tempo
// 	auto start = std::chrono::steady_clock::now();
// 	//cria ids com os indices
//     IdCont ids;
//     for (std::size_t i = 0; i < _input.size(); ++i)
// 		ids.push_back(i);

// 	//sorting
//     fordJohnson(ids);

//     out.clear();
//     for (typename IdCont::const_iterator it = ids.begin();it != ids.end(); ++it)
// 	out.push_back(_input[*it]);

// 	//marca final
//     auto end = std::chrono::steady_clock::now();
//     return std::chrono::duration<double, std::micro>(end - start).count();
// }

void PmergeMe::fordJohnsonVector(std::vector<std::size_t>& ids)
{
	const std::size_t n = ids.size();
	if (n < 2)
		return;

	const std::vector<int>& d = _input;
	auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

	// 1. forma os pares: vencedor = maior, perdedor = menor
	std::vector<std::size_t> winners;
	std::vector<std::size_t> loserOf(d.size(), 0);
	for (std::size_t i = 0; i + 1 < n; i += 2) {
		std::size_t a = ids[i], b = ids[i + 1];
		if (cmp(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}
	const bool hasStraggler = (n % 2 == 1);
	const std::size_t straggler = hasStraggler ? ids[n - 1] : 0;

	// 2. ordena os vencedores recursivamente
	fordJohnsonVector(winners);

	// 3. cadeia principal: b1 (de graca) + vencedores ordenados
	std::vector<std::size_t> chain;
	chain.push_back(loserOf[winners[0]]);
	chain.insert(chain.end(), winners.begin(), winners.end());

	// 4. insere b2..bm na ordem de Jacobsthal
	const std::size_t m = winners.size();
	std::size_t done = 1, jPrev = 1, jCur = 3;
	while (done < m) {
		const std::size_t top = std::min(jCur, m);
		for (std::size_t k = top; k > done; --k) {
			const std::size_t w = winners[k - 1];
			const std::size_t b = loserOf[w];
			std::vector<std::size_t>::iterator bound = std::find(chain.begin(), chain.end(), w);
			std::vector<std::size_t>::iterator pos = std::lower_bound(chain.begin(), bound, b, cmp);
			chain.insert(pos, b);
		}
		done = top;
		const std::size_t next = jCur + 2 * jPrev;
		jPrev = jCur;
		jCur = next;
	}

	// 5. o elemento sobrando: busca binaria na cadeia inteira
	if (hasStraggler)
		chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler, cmp), straggler);

	ids = std::move(chain);
}

void PmergeMe::fordJohnsonDeque(std::deque<std::size_t>& ids)
{
	const std::size_t n = ids.size();
	if (n < 2)
		return;

	const std::vector<int>& d = _input;
	auto cmp = [&d](std::size_t a, std::size_t b) { return d[a] < d[b]; };

	// 1. forma os pares: vencedor = maior, perdedor = menor
	std::deque<std::size_t> winners;
	std::vector<std::size_t> loserOf(d.size(), 0);
	for (std::size_t i = 0; i + 1 < n; i += 2) {
		std::size_t a = ids[i], b = ids[i + 1];
		if (cmp(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}
	const bool hasStraggler = (n % 2 == 1);
	const std::size_t straggler = hasStraggler ? ids[n - 1] : 0;

	// 2. ordena os vencedores recursivamente
	fordJohnsonDeque(winners);

	// 3. cadeia principal: b1 (de graca) + vencedores ordenados
	std::deque<std::size_t> chain;
	chain.push_back(loserOf[winners[0]]);
	chain.insert(chain.end(), winners.begin(), winners.end());

	// 4. insere b2..bm na ordem de Jacobsthal
	const std::size_t m = winners.size();
	std::size_t done = 1, jPrev = 1, jCur = 3;
	while (done < m) {
		const std::size_t top = std::min(jCur, m);
		for (std::size_t k = top; k > done; --k) {
			const std::size_t w = winners[k - 1];
			const std::size_t b = loserOf[w];
			std::deque<std::size_t>::iterator bound = std::find(chain.begin(), chain.end(), w);
			std::deque<std::size_t>::iterator pos = std::lower_bound(chain.begin(), bound, b, cmp);
			chain.insert(pos, b);
		}
		done = top;
		const std::size_t next = jCur + 2 * jPrev;
		jPrev = jCur;
		jCur = next;
	}

	// 5. o elemento sobrando: busca binaria na cadeia inteira
	if (hasStraggler)
		chain.insert(std::lower_bound(chain.begin(), chain.end(), straggler, cmp), straggler);

	ids = std::move(chain);
}

double PmergeMe::sortWithVector()
{
	//inicia a contagem do tempo
	auto start = std::chrono::steady_clock::now();
	//cria ids com os indices (numeros de posicao de cada elemento)
	//usar indice é melhor que valor quando o codigo aceita duplicados
	std::vector<std::size_t> ids;
	for (std::size_t i = 0; i < _input.size(); ++i)
		ids.push_back(i);

	//sorting
	fordJohnsonVector(ids);

	_vecResult.clear();
	for (std::vector<std::size_t>::const_iterator it = ids.begin();it != ids.end(); ++it)
		_vecResult.push_back(_input[*it]);

	//marca final
	auto end = std::chrono::steady_clock::now();
	return std::chrono::duration<double, std::micro>(end - start).count();
}

double PmergeMe::sortWithDeque()
{
	//inicia a contagem do tempo
	// steady_clock: um relógio monotônico.
		//Ele só avança e nunca é ajustado
		// (por mudança de horário, fuso ou
		// sincronização de rede). É o relógio
		// certo para medir intervalos.
	//::now(): função estática que retorna o instante atual desse relógio.
	//auto start: o compilador deduz o tipo da variável sozinho.
	auto start = std::chrono::steady_clock::now();
	//cria ids com os indices
    std::deque<std::size_t> ids;
    for (std::size_t i = 0; i < _input.size(); ++i)
		ids.push_back(i);

	//sorting
    fordJohnsonDeque(ids);

    _deqResult.clear();
    for (std::deque<std::size_t>::const_iterator it = ids.begin();it != ids.end(); ++it)
		_deqResult.push_back(_input[*it]);

	//marca final da contagem do tempo
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
