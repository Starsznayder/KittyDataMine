#ifndef RionaH
#define RionaH

#include <cstdio>
#include <vector>
#include <memory>

#include "metric.h"
#include "observation.h"
#include "kpnn.h"
#include "ria.h"


template <typename M, typename T>
uint8_t riona(const Metric<T>& metric,
              const M& metup,
              const std::vector<Observation<T>>& dataset,
              const T& test_data,
              const size_t k,
			  const size_t number_of_classes) {
    
    auto neighbours = plus_nearest_neighbours(
        metric,
        dataset,
        test_data,
        k
    );

    if(PRINT_LOG) {
      printf("\"metadata\": [");
      for(auto& obs : dataset) {

        bool sw = false;
        for(auto& ngbr : neighbours)
            if(ngbr.id == obs.id) { sw = true; break; }
        if(sw) continue;
        
        auto dist = metric(obs.data, test_data);
        printf(
            "{\"ngbr_id\": %d, \"decision\": %d, \"distance\": %f, \"consistent\": null, \"decisive\": false}, ",
            (int) obs.id,
            (int) obs.target,
            dist
        );

      }
    }
    
    auto result = ria(
        metup,
        neighbours,
        test_data, 
        number_of_classes
    );

    return result;
}

#endif
