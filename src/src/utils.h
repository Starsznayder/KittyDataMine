#ifndef UtilsH
#define UtilsH

#include <vector>
#include <memory>
#include <algorithm>

uint8_t mode(std::vector<uint8_t> data, size_t size) {
	std::vector<size_t> hist(size, 0);
	for(auto& e : data) ++hist[e];
	return std::max_element(hist.begin(), hist.end()) - hist.begin();
}

#endif
