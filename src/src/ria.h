#ifndef RiaH
#define RiaH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
//#include <boolean>
#include <algorithm>
#include <limits>

#include "metric.h"
#include "observation.h"
#include "utils.h"
#include "rule.h"


/*
template <typename T> 
bool is_consistent(const Rule<T>& rule,
                   const Observation<T>* verify_set,
                   size_t set_size) {
    for(auto obs = verify_set; obs < verify_set + set_size; ++obs)
		if(obs->target != rule.decision && rule(obs->data))
			return false;
	return true;	
}


template <typename T>
uint8_t __ria(const Metric<T>& metric,
             const Observation<T>* dataset,
             size_t dataset_size,
             const T& test_data,
			 const size_t number_of_classes) {
	
	std::vector<size_t> support_set_count(number_of_classes);	
    for (auto observation = dataset; observation < dataset + dataset_size; ++observation) {
        Rule<T> rule(observation->target, metric, observation->data, test_data);
		if (is_consistent(rule, dataset, dataset_size)) 
			++support_set_count[static_cast<int>(observation->target)];
    }
    return (uint8_t) std::distance(
        support_set_count.begin(),
        std::max_element(
            support_set_count.begin(),
            support_set_count.end()
        )
    ); 
}

template <typename T>
uint8_t ria(const Metric<T>& metric,
            const std::vector<Observation<T>>& dataset,
            const T& test_data,
			const size_t number_of_classes) {
    return __ria(metric, dataset.data(), dataset.size(), test_data, number_of_classes); 
}
*/

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
	for(auto& observation : dataset) {
        Rule<T> rule(observation.target, metric, observation.data, test_data);
		if(is_consistent(rule, dataset)) 
			++support_set_count[static_cast<int>(observation.target)];
    }
    return (uint8_t) std::distance(
        support_set_count.begin(),
        std::max_element(
            support_set_count.begin(), 
            support_set_count.end()
        )
    ); 
}

#endif
