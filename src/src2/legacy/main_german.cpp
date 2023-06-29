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


template <size_t K>
std::vector<uint8_t> nth(std::vector<GermanTuple> input) {
    std::vector<uint8_t> output;
    output.reserve(input.size());
    for(auto& e : input) output.push_back(std::get<K>(e));
    return output;
}

std::vector<uint8_t> run_knn(const std::vector<GermanTuple>& X,
                             const std::vector<uint8_t>& y,
                             const std::vector<GermanTuple>& T,
                             size_t k,
                             size_t number_of_classes) {

    auto metup = std::make_tuple(
        ValueDifference(y, nth<0>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<2>(X)),
        ValueDifference(y, nth<3>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<5>(X)),
        ValueDifference(y, nth<6>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<8>(X)),
        ValueDifference(y, nth<9>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<11>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<13>(X)),
        ValueDifference(y, nth<14>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<16>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<18>(X)),
        ValueDifference(y, nth<19>(X))
    );


    Manhattan<decltype(metup), GermanTuple> metric(metup);

    // Prepare dataset
    std::vector<Observation<GermanTuple>> dataset;
    for(size_t i = 0; i < X.size(); ++i) {
        Observation<GermanTuple> obs(y[i], X[i]);
        dataset.push_back(obs);
    }

    std::vector<uint8_t> result;
    for (auto& t : T) {
        auto res = knn(
            metric,
            dataset,
            t,
            k,
            number_of_classes
        );

        result.push_back(res);
    }

    return result;
}


std::vector<uint8_t> run_ria(const std::vector<GermanTuple>& X,
                             const std::vector<uint8_t>& y,
                             const std::vector<GermanTuple>& T,
                             size_t number_of_classes) {

    auto metup = std::make_tuple(
        ValueDifference(y, nth<0>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<2>(X)),
        ValueDifference(y, nth<3>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<5>(X)),
        ValueDifference(y, nth<6>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<8>(X)),
        ValueDifference(y, nth<9>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<11>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<13>(X)),
        ValueDifference(y, nth<14>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<16>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<18>(X)),
        ValueDifference(y, nth<19>(X))
    );


    Max<decltype(metup), GermanTuple> metric(metup);

    // Prepare dataset
    std::vector<Observation<GermanTuple>> dataset;
    for(size_t i = 0; i < X.size(); ++i) {
        Observation<GermanTuple> obs(y[i], X[i]);
        dataset.push_back(obs);
    }

    std::vector<uint8_t> result;
    for (auto& t : T) {
        auto res = ria(
            metric,
            dataset,
            t,
            number_of_classes
        );

        result.push_back(res);
    }

    return result;
}

std::vector<uint8_t> run_riona(const std::vector<GermanTuple>& X,
                             const std::vector<uint8_t>& y,
                             const std::vector<GermanTuple>& T,
                             size_t k,
                             size_t number_of_classes) {

    auto metup = std::make_tuple(
        ValueDifference(y, nth<0>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<2>(X)),
        ValueDifference(y, nth<3>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<5>(X)),
        ValueDifference(y, nth<6>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<8>(X)),
        ValueDifference(y, nth<9>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<11>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<13>(X)),
        ValueDifference(y, nth<14>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<16>(X)),
        AbsoluteDifference{},
        ValueDifference(y, nth<18>(X)),
        ValueDifference(y, nth<19>(X))
    );

    Max<decltype(metup), GermanTuple> metric_knn(metup);
    Max<decltype(metup), GermanTuple> metric_ria(metup);

    // Prepare dataset
    std::vector<Observation<GermanTuple>> dataset;
    for(size_t i = 0; i < X.size(); ++i) {
        Observation<GermanTuple> obs(y[i], X[i]);
        dataset.push_back(obs);
    }

    std::vector<uint8_t> result;
    for (auto& t : T) {
        auto res = riona(
            metric_knn,
            metric_ria,
            dataset,
            t,
            k,
            number_of_classes
        );

        result.push_back(res);
    }

    return result;
}


int main() {

    const size_t number_of_classes = 3;

    // Load Train
    const auto df_train = read_german("data/german.csv");
    const auto X_train = std::get<0>(df_train); 
    const auto y_train = std::get<1>(df_train);

    // Load Test
    const auto df_test = read_german("data/german.csv");
    const auto X_test = std::get<0>(df_test); 
    const auto y_test = std::get<1>(df_test);

    // Verify Loading
    if (false) {
      for (size_t i = 0; i < X_train.size(); ++i) {
        std::cout << (int) y_train[i] << ", " 
                 << std::get<0>(X_train[i]) << ", "
                 << std::get<1>(X_train[i]) << ", "
                 << std::get<2>(X_train[i]) << ", "
                 << std::get<3>(X_train[i]) << std::endl;
       }
    }
    
    printf("train size: %d\n", (int) X_train.size()); 
    printf("test size: %d\n", (int) X_test.size()); 

    auto result_knn = run_knn(
        X_train, y_train, X_test, 1, number_of_classes
    );

    auto result_ria = run_ria(
       X_train, y_train, X_test, number_of_classes
    );

    auto result_riona = run_riona(
       X_train, y_train, X_test, 10, number_of_classes
    );

    if(false) {
    for(auto e : y_test)
        printf("%d", e);
    printf("\n");

    for(auto e : result_knn)
        printf("%d", e);
    printf("\n");

    for(auto e : result_ria)
        printf("%d", e);
    printf("\n");
    
    for(auto e : result_riona)
        printf("%d", e);
    printf("\n");
    }

    std::ofstream result_file;
    result_file.open("data/german_result.csv");
    for(size_t i = 0; i < X_test.size(); ++i)
        result_file << (int) y_test[i] << " "
                    << (int) result_knn[i] << " "
                    << (int) result_ria[i] << " "
                    << (int) result_riona[i] << std::endl;

    return 0;
}
