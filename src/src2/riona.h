#ifndef RionaH
#define RionaH

#include <cstdio>
#include <vector>
#include <memory>

#include "metric.h"
#include "observation.h"
#include "kpnn.h"
#include "ria.h"


template <typename M, typename T, size_t number_of_classes>
uint8_t riona(const Metric<T>& metric,
              const M& metup,
              const std::vector<Observation<T>>& dataset,
              const T& test_data,
              const size_t k) {
    
    auto neighbours = plus_nearest_neighbours(
        metric,
        dataset,
        test_data,
        k
    );

    auto result = ria(
        metup,
        neighbours,
        test_data
    );

    return result;
}

#endif
