#define PRINT_LOG true

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <tuple>

// #include "parsers.h"
#include "observation.h"
#include "metric.h"
#include "kmnn.h"
#include "ria.h"
#include "riona.h"

using IrisTuple = std::tuple<float, float, float, float>;

std::tuple<std::vector<IrisTuple>,
           std::vector<uint8_t>> read_iris(std::string path) {

    std::ifstream File(path);

    std::vector<IrisTuple> data_vec;
    std::vector<uint8_t> cls_vec;

    float cls;
    float d0, d1, d2, d3;

    while (File >> cls >> d0 >> d1 >> d2 >> d3) {
        data_vec.push_back(IrisTuple(d0, d1, d2, d3));
        cls_vec.push_back((uint8_t) cls);
    }
    return std::make_tuple(data_vec, cls_vec);
}


std::tuple<
    std::vector<uint8_t>,
    std::vector<uint8_t>,
    std::vector<uint8_t>,
    std::vector<uint8_t>>
    run_all(const std::vector<IrisTuple>& X,
            const std::vector<uint8_t>& y,
            size_t k,
            size_t number_of_classes) {

    auto metup = std::make_tuple(
        AbsoluteDifference{},
        AbsoluteDifference{},
        AbsoluteDifference{},
        AbsoluteDifference{}
    );

    Manhattan<decltype(metup), IrisTuple> metric_knn(metup);
    Max<decltype(metup), IrisTuple> metric_ria(metup);

    std::vector<uint8_t> result_kmnn;
    std::vector<uint8_t> result_kpnn;
    std::vector<uint8_t> result_ria;
    std::vector<uint8_t> result_riona;
    
    if(PRINT_LOG) printf("\"metadata\": [\n");
    for(size_t i = 0; i < X.size(); ++i) {
        
        auto test_obs = X[i];

        std::vector<Observation<IrisTuple>> dataset;
        for(size_t j = 0; j < X.size(); ++j) {
            if(j == i) continue;
            Observation<IrisTuple> obs(j, y[j], X[j]);
            dataset.push_back(obs);
        }
        
        if(PRINT_LOG) printf("{");
        if(PRINT_LOG) printf("\"kmnn\": {\"target\": %d, ", (int) y[i]);
        result_kmnn.push_back(kmnn(
            metric_knn,
            dataset,
            test_obs,
            k,
            number_of_classes
        ));
        if(PRINT_LOG) printf("\b\b},");
        
        if(PRINT_LOG) printf("\"kpnn\": {\"target\": %d, ", (int) y[i]);
        result_kpnn.push_back(kpnn(
            metric_knn,
            dataset,
            test_obs,
            k,
            number_of_classes
        ));
        if(PRINT_LOG) printf("\b\b}, ");

        if(PRINT_LOG) printf("\"ria\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_ria.push_back(ria(
            metric_ria,
            dataset,
            test_obs,
            number_of_classes
        ));
        if(PRINT_LOG) printf("}, ");

        if(PRINT_LOG) printf("\"riona\": {\"target\": %d, \"metadata\": [", (int) y[i]);
        result_riona.push_back(riona(
            metric_knn,
            metric_ria,
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

    const size_t number_of_classes = 3;
    const size_t k = 5;

    // Load Train
    const auto df = read_iris("data/csv/iris.csv");
    const auto X = std::get<0>(df); 
    const auto y = std::get<1>(df);

    printf("{\n");
    printf("\"file_name\": \"data/iris.csv\",\n");
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
    
    printf("},\n");
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
        printf(i+1 == y.size() ? "%d],\n" : "%d, ", result_riona[i]);

    printf("}\n");
    printf("}\n");

    return 0;
}
