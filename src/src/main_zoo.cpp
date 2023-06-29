#define PRINT_LOG true

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "observation.h"
#include "metric.h"
#include "kmnn.h"
#include "ria.h"
#include "riona.h"

using ZooTuple = std::tuple<
    uint8_t, uint8_t, uint8_t, uint8_t,
    uint8_t, uint8_t, uint8_t, uint8_t,
    uint8_t, uint8_t, uint8_t, uint8_t,
    uint8_t, uint8_t, uint8_t, uint8_t
>;

/*
@attribute HAIR integer [0, 1]
@attribute FEATHERS integer [0, 1]
@attribute EGGS integer [0, 1]
@attribute MILK integer [0, 1]
@attribute AIRBORNE integer [0, 1]
@attribute AQUATIC integer [0, 1]
@attribute PREDATOR integer [0, 1]
@attribute TOOTHED integer [0, 1]
@attribute BACKBONE integer [0, 1]
@attribute BREATHES integer [0, 1]
@attribute VENOMOUS integer [0, 1]
@attribute FINS integer [0, 1]
@attribute LEGS integer [0, 8]
@attribute TAIL integer [0, 1]
@attribute DOMESTIC integer [0, 1]
@attribute CATSIZE integer [0, 1]
@attribute class {1, 2, 3, 4, 5, 6, 7}
*/

std::tuple<std::vector<ZooTuple>,
           std::vector<uint8_t>> read_zoo(std::string path) {

    std::ifstream File(path);

    std::vector<ZooTuple> data_vec;
    std::vector<uint8_t> cls_vec;

    float cls;
    float d0, d1, d2, d3, 
          d4, d5, d6, d7,
          d8, d9, d10, d11,
          d12, d13, d14, d15;

    while (File >> cls >> d0 >> d1 >> d2 >> d3
                       >> d4 >> d5 >> d6 >> d7
                       >> d8 >> d9 >> d10 >> d11
                       >> d12 >> d13 >> d14 >> d15) {
        data_vec.push_back(ZooTuple(
            (uint8_t) d0, (uint8_t) d1, (uint8_t) d2, (uint8_t) d3,
            (uint8_t) d4, (uint8_t) d5, (uint8_t) d6, (uint8_t) d7,
            (uint8_t) d8, (uint8_t) d9, (uint8_t) d10, (uint8_t) d11,
            (uint8_t) d12, (uint8_t) d13, (uint8_t) d14, (uint8_t) d15
        ));
        cls_vec.push_back((uint8_t) cls);
    }
    return std::make_tuple(data_vec, cls_vec);
}


std::tuple<
    std::vector<uint8_t>,
    std::vector<uint8_t>,
    std::vector<uint8_t>,
    std::vector<uint8_t>>
    run_all(const std::vector<ZooTuple>& X,
            const std::vector<uint8_t>& y,
            size_t k,
            size_t number_of_classes) {

    auto metup = std::make_tuple(
        ValueDifference(y, nth<0, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<1, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<2, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<3, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<4, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<5, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<6, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<7, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<8, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<9, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<10, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<11, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<12, ZooTuple>(X), 7, 6),
        ValueDifference(y, nth<13, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<14, ZooTuple>(X), 7, 2),
        ValueDifference(y, nth<15, ZooTuple>(X), 7, 2)
    );

    Manhattan<decltype(metup), ZooTuple> metric(metup);

    std::vector<uint8_t> result_kmnn;
    std::vector<uint8_t> result_kpnn;
    std::vector<uint8_t> result_ria;
    std::vector<uint8_t> result_riona;
    
    if(PRINT_LOG) printf("\"metadata\": [\n");
    for(size_t i = 0; i < X.size(); ++i) {
        
        auto test_obs = X[i];

        std::vector<Observation<ZooTuple>> dataset;
        for(size_t j = 0; j < X.size(); ++j) {
            if(j == i) continue;
            Observation<ZooTuple> obs(j, y[j], X[j]);
            dataset.push_back(obs);
        }

        if(PRINT_LOG) printf("{");
        if(PRINT_LOG) printf("\"kmnn\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_kmnn.push_back(kmnn(
            metric,
            dataset,
            test_obs,
            k,
            number_of_classes
        ));
        if(PRINT_LOG) printf("},\n");

        if(PRINT_LOG) printf("\"kpnn\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_kpnn.push_back(kpnn(
            metric,
            dataset,
            test_obs,
            k,
            number_of_classes
        ));
        if(PRINT_LOG) printf("},\n");

        if(PRINT_LOG) printf("\"ria\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_ria.push_back(ria(
            metup,
            dataset,
            test_obs,
            number_of_classes
        ));
        if(PRINT_LOG) printf("},\n");

        if(PRINT_LOG) printf("\"riona\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_riona.push_back(riona(
            metric,
            metup,
            dataset,
            test_obs,
            k,
            number_of_classes
        ));
        if(PRINT_LOG) printf("}}");
        if(PRINT_LOG && i + 1 < X.size()) printf(",\n");
    } 
    if(PRINT_LOG) printf("],\n");

    return std::make_tuple(
        result_kmnn,
        result_kpnn,
        result_ria,
        result_riona
   );
}


int main() {

    const size_t number_of_classes = 7;
    const size_t k = 5;

    // Load Train
    const auto df = read_zoo("data/csv/zoo.csv");
    const auto X = std::get<0>(df); 
    const auto y = std::get<1>(df);

    printf("{\n");
    printf("\"file_name\": \"data/csv/zoo.csv\",\n");
    printf("\"number_of_observations\": %d,\n", (int) y.size());
    printf("\"number_of_attributes\": %d,\n", 4);
    printf("\"k\": %d,\n", (int) k);

    auto results = run_all(
       X, y, k, number_of_classes
    );
    
    auto result_kmnn  = std::get<0>(results);
    auto result_kpnn  = std::get<1>(results);
    auto result_ria   = std::get<2>(results);
    auto result_riona = std::get<3>(results);

    printf("\"measures\": {\n");
    printf("\"accuracy\": {\n");
    
    printf("\"kmnn\": %f,\n", accuracy(y, result_kmnn));
    printf("\"kpnn\": %f,\n", accuracy(y, result_kpnn));
    printf("\"ria\": %f,\n",  accuracy(y, result_ria));
    printf("\"riona\": %f\n", accuracy(y, result_riona));

    printf("},\n");
    printf("\"balanced_accuracy\": {\n");
    printf("\"kmnn\": %f,\n", balanced_accuracy(y, result_kmnn, number_of_classes));
    printf("\"kpnn\": %f,\n", balanced_accuracy(y, result_kpnn, number_of_classes));
    printf("\"ria\": %f,\n",  balanced_accuracy(y, result_ria, number_of_classes));
    printf("\"riona\": %f\n", balanced_accuracy(y, result_riona, number_of_classes));
    
    printf("}\n");
    printf("},\n");

    printf("\"predictions\": {\n");
    printf("\"target\": [");
    for(size_t i = 0; i < y.size(); ++i)
        printf(i+1 == y.size() ? "%d],\n" : "%d, ", y[i]);
    
    printf("\"kmnn\": [");
    for(size_t i = 0; i < y.size(); ++i)
        printf(i+1 == y.size() ? "%d],\n" : "%d, ", result_kmnn[i]);

    printf("\"kpnn\": [");
    for(size_t i = 0; i < y.size(); ++i)
        printf(i+1 == y.size() ? "%d],\n" : "%d, ", result_kpnn[i]);

    printf("\"ria\": [");
    for(size_t i = 0; i < y.size(); ++i)
        printf(i+1 == y.size() ? "%d],\n" : "%d, ", result_ria[i]);

    printf("\"riona\": [");
    for(size_t i = 0; i < y.size(); ++i)
        printf(i+1 == y.size() ? "%d]\n" : "%d, ", result_riona[i]);

    printf("}\n");
    printf("}\n");

    return 0;
}
