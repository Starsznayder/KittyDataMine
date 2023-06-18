#ifndef ParserH
#define ParserH

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "parsers.h"
#include "observation.h"
#include "metric.h"
#include "knn.h"
#include "ria.h"
#include "riona.h"


void run_knn(const std::vector<IrisTuple>& X,
             const std::vector<uint8_t>& y,
             const std::vector<IrisTuple>& T,
             size_t k,
             size_t number_of_classes) {
    
    std::vector<Observation<IrisTuple>> dataset;
    for(size_t i = 0; i < X.size(); ++i) {
        Observation<IrisTuple> obs(y[i], X[i]);
        dataset.push_back(obs);
    }
    
    std::vector<uint8_t> v = {1, 2, 3};

    auto metup = std::make_tuple(
        AbsoluteDifference{},
        AbsoluteDifference{},
        AbsoluteDifference{},
        AbsoluteDifference{}
    );

    Manhattan<decltype(metup), IrisTuple> metric(metup);
    
    for (auto& t : T) {
        auto result = knn(
            metric,
            dataset,
            T[0],
            k,
            number_of_classes
        );

        printf("%d\n", result);
    }
}

int main() {
    auto df = read_iris();
    auto X = std::get<0>(df); 
    auto y = std::get<1>(df);

    // Verify Loading
    //for (const auto& i : X) {
    for (size_t i = 0; i < X.size(); ++i) {
       std::cout << y[i] << ", " 
                 << std::get<0>(X[i]) << ", "
                 << std::get<1>(X[i]) << ", "
                 << std::get<2>(X[i]) << ", "
                 << std::get<3>(X[i]) << std::endl;
    }

    for (const auto& i : y) {
        //printf("%d\n", i);
    }

    return 0;
}

#endif
