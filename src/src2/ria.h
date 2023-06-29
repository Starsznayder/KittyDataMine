#ifndef RiaH
#define RiaH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
#include <limits>

#include "metric.h"
#include "observation.h"
#include "utils.h"
#include "rule.h"

#ifndef PRINT_LOG
#define PRINT_LOG false
#endif


template <typename M, typename T> 
bool is_consistent(const Rule<M, T>& rule, const std::vector<Observation<T>> verify_set) {
	for(auto& obs : verify_set)
		if(obs.target != rule.decision && rule(obs.data))
			return false;
	return true;	
}

template <typename M, typename T, size_t number_of_classes>
uint8_t ria(const M& metrics,
            const std::vector<Observation<T>>& dataset,
            const T& test_data) {
	
	std::array<size_t, number_of_classes> support_set_count;

	for(auto& observation : dataset) {
        Rule<M, T> rule(observation.target, metrics, observation.data, test_data);
		if(is_consistent(rule, dataset))
			++support_set_count[static_cast<int>(observation.target)];
    }

    uint8_t result = std::distance(
        support_set_count.begin(),
        std::max_element(
            support_set_count.begin(),
            support_set_count.end()
        )
    );
    
    return result;
}

#endif
