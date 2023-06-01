#ifndef RiaH
#define RiaH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <bool>
#include <algorithm>
#include <limits>
#include "metric.h"
#include "observation.h"
#include "utils.h"


template <typename T> 
bool is_consistent(const Rule<T>& rule, const std::vector<Observation<T>> verify_set) {
	for(auto& obs : verify_set)
		if(obs.target != rule.decision && rule(obs.data))
			return false;
	return true;	
}

template <typename T>
uint8_t ria(const Metric<T>& metric,
            const std::vector<Observation<T>>& dataset,
            const T& test_data,
			const size_t number_of_classes) {
	
	std::vector<size_t> support_set_count(number_of_classes);	
	for(auto& observation : dataset) 
		if(is_consistent(Rule(observation.target, metric, observation.data, test_data), dataset)) 
			++support_set[static_cast<int>(observation.target)];
	return mode(support_set_count);
}

#endif
