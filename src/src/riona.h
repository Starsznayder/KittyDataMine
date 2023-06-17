#ifndef RionaH
#define RionaH

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
#include "knn.h"
#include "ria.h"


template <typename T>
uint8_t riona(const Metric<T>& metric_knn,
              const Metric<T>& metric_ria,
              const std::vector<Observation<T>>& dataset,
              const T& test_data,
              const size_t k,
			  const size_t number_of_classes) {
    
    auto neighbours = nearest_neighbours(
        metric_knn,
        dataset,
        test_data,
        k
    );
    
    return __ria(
        metric_ria,
        neighbours.data(),
        neighbours.size(),
        test_data, 
        number_of_classes
    );
}

#endif
