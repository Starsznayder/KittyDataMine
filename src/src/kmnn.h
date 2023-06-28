#ifndef KmnnH
#define KmnnH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
#include <limits>

#include "utils.h"
#include "metric.h"
#include "observation.h"

#ifndef PRINT_LOG
#define PRINT_LOG false
#endif

template <typename T>
std::vector<Observation<T>> minus_nearest_neighbours(const Metric<T>& metric,
                                                     const std::vector<Observation<T>>& dataset,
                                                     const T& element,
                                                     const size_t k) {

    std::vector<float> distances(k, std::numeric_limits<float>::infinity());
    std::vector<const Observation<T> *> neighbours(k);


    for(auto it = dataset.begin(); it != dataset.end(); ++it) {
        auto new_ngbr = &(*it);
        float new_dist = metric(new_ngbr->data, element);
        size_t i = 0;
        while(i < k && new_dist < distances[i]) ++i; 
        while(i > 0) {
            --i;
            std::swap(distances[i], new_dist);         
            std::swap(neighbours[i], new_ngbr); 
        }
    }
    std::vector<Observation<T>> result;
    std::transform(
        neighbours.begin(),
        neighbours.end(),
        std::back_inserter(result),
        [](const Observation<T>* n){ return *n; }
    );
    return result;
}

template <typename T>
uint8_t kmnn(const Metric<T>& metric,
	 		 const std::vector<Observation<T>>& dataset, 
			 const T element, 
			 const size_t k,
			 const size_t number_of_classes) {
	auto neighbours = minus_nearest_neighbours(metric, dataset, element, k);
    std::vector<size_t> hist(number_of_classes, 0);
    for(auto& n : neighbours) ++hist[n.target];
	auto result =  std::max_element(hist.begin(), hist.end()) - hist.begin();

    if(PRINT_LOG) {
      printf(
        " \"prediction\": %d, \"metadata\": [",
        (int) result
      );
      for(auto& obs : dataset) {
        auto dist = metric(obs.data, element);
        printf(
            "{\"ngbr_id\": %d, \"decision\": %d, \"distance\": %f, \"consistent\": null, ",
            (int) obs.id,
            (int) obs.target,
            dist
            //obs.target == result ? "true" : "false"
        );

        bool sw = false;
        for(auto& ngbr : neighbours)
            if(ngbr.id == obs.id) { sw = true; break; }
        printf("\"decisive\": %s}, ", sw ? "true" : "false");
      }
      printf("\b\b]");
    }

    return result;
}

#endif
