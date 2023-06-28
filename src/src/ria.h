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
    if(PRINT_LOG) printf("\"metadata\": [");
	for(auto& observation : dataset) {
        Rule<T> rule(observation.target, metric, observation.data, test_data);
        bool consistent = is_consistent(rule, dataset);
        if(PRINT_LOG)
            printf(
                "{\"ngbr_id\": %d, \"decision\": %d, \"distance\": null, \"consistent\": %s, \"decisive\": true}, ",
                (int) observation.id,
                (int) observation.target,
                consistent ? "true" : "false"
            );
		if(consistent)
			++support_set_count[static_cast<int>(observation.target)];
    }

    uint8_t result = std::distance(
        support_set_count.begin(),
        std::max_element(
            support_set_count.begin(),
            support_set_count.end()
        )
    );
    
    if(PRINT_LOG)
        printf("\b\b], \"prediction\": %d", (int) result);
    
    return result;
}

#endif
