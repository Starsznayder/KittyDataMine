#ifndef KpnnH
#define KpnnH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
#include <limits>

#include "utils.h"
#include "metric.h"
#include "observation.h"

template <typename T>
std::vector<Observation<T>> plus_nearest_neighbours(const Metric<T>& metric,
                                                    const std::vector<Observation<T>>& dataset,
                                                    const T& element,
                                                    const size_t k) {

    std::vector<float> distances;
    distances.reserve(dataset.size());
    std:transform(
        dataset.begin(),
        dataset.end(),
        std::back_inserter(distances),
        [metric, element](const Observation<T>& obs) { metric(obs.data, element); }
    );
    
    std::vector<float> best_k_distances(k, std::numeric_limits<float>::infinity());
    for(auto distance : distances) {
        size_t i = 0;
        while(i < k && distance < distances[i]) i++;
        for(; i > 0; --i) std::swap(distances[i], distance);
    }

    std::vector<Observation<T>> result;
    auto boundary = distances[0];
    for(size_t i = 0; i < dataset.size(); ++i)
        if (distances[i] <= boundary)
            result.push_back(dataset[i]);
    return result;
    
}

template <typename T>
uint8_t kpnn(const Metric<T>& metric,
	 		 const std::vector<Observation<T>>& dataset,
			 const T element,
			 const size_t k,
			 const size_t number_of_classes) {
	auto neighbours = plus_nearest_neighbours(metric, dataset, element, k);
    std::vector<size_t> hist(number_of_classes, 0);
    for(auto& n : neighbours) ++hist[n.target];
	return std::max_element(hist.begin(), hist.end()) - hist.begin();
}

#endif
