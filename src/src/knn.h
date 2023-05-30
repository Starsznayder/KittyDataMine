#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>
#include <limits>
#include "metric.h"

template <typename T>
std::vector<T> nearest_neighbours(Metric<T>& metric,
                                  std::vector<T>& dataset,
                                  T element,
                                  size_t k) {
    std::vector<float> distances(k, std::numeric_limits<float>::infinity());
    std::vector<T> neighbours(k);

    for(auto& new_ngbr : dataset) {
        float new_dist = metric(new_ngbr, element);
        size_t i = 0;
        while(i < k && new_dist < distances[i]) ++i; // --i;
        while(i > 0) {
            i--;
            std::swap(distances[i], new_dist);
            std::swap(neighbours[i], new_ngbr);  
        }
        // for(size_t i = 0; i < k; ++i) { 
        //     float& old_dist = distances[i];
        //     T& old_ngbr = neighbours[i];
        //     if (old_dist < new_dist) break;
        //     std::swap(old_dist, new_dist);
        //     std::swap(old_ngbr, new_ngbr);
        // }
    }
    return neighbours;
}
