#ifndef UtilsH
#define UtilsH

#include <vector>

template <typename T, typename A>
uint8_t mode(std::vector<T, A> const& vec) {
	return std::static_cast<uint8_t>(std::distance(vec.begin(), std::max_element(vec.begin(), vec.end())));
}

std::vector<size_t> counts(std::vector<uint8_t> data, uint8_t size) {
	std::vector<size_t> histogram(size, 0);
	for(auto& e : data) ++histogram[e];
	return histogram;
}

#endif
