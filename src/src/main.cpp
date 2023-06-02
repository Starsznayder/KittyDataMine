#include <cstdio>
#include <memory>
#include "knn.h"
#include "observation.h"

void test_ValueDifferenceMetric() {
    std::vector<uint8_t> cls = {1,1,1,2,2,0,0};
    std::vector<uint8_t> vec = {0,1,0,1,2,2,3};
    ValueDifference metric(cls, vec);
    for(size_t i = 0; i < 4; i++) {
        for(size_t j = 0; j < 4; j++) {
            printf(" %f ", metric((uint8_t) i, (uint8_t) j));
        }
        printf("\n"); 
    } 
}

void test_knn_VDM() {
    std::vector<uint8_t> cls = {1,1,1,2,2,0,0};
    std::vector<uint8_t> vec = {0,1,0,1,2,2,3};
    ValueDifference metric(cls, vec);

    
    std::vector<Observation<uint8_t>> dataset;
    for(size_t i = 0; i < cls.size(); ++i) {
        Observation<uint8_t> obs(cls[i], vec[i]);
        dataset.push_back(obs);
    }

    auto nn = nearest_neighbours(metric, dataset, (uint8_t) 3, (size_t) 2);
    
    printf("%d\n", nn[0]->data); 
    printf("%d\n", nn[1]->data); 
}

void test_knn_Tuples_Max() {
    std::vector<uint8_t> cls = {1,1,1};
    std::vector<std::tuple<uint8_t, float>> vec = {
        {1, 1.0},
        {2, 2.0},
        {3, 0.0}
    };
    
    std::vector<Observation<std::tuple<uint8_t, float>>> dataset;
    for(size_t i = 0; i < cls.size(); ++i) {
        Observation<std::tuple<uint8_t, float>> obs(cls[i], vec[i]);
        dataset.push_back(obs);
    }
    
    std::vector<uint8_t> v = {1, 2, 3};

    auto metup = std::make_tuple(ValueDifference{cls, v}, AbsoluteDifference{});
    auto tst   = std::make_tuple((uint8_t) 3, (float) 3.0);

    Max<decltype(metup), decltype(tst)> metric(metup);

    auto nn = nearest_neighbours(
        metric,
        dataset,
        tst,
        2
    );

    printf("%d\n", std::get<0>(nn[0]->data));
    printf("%d\n", std::get<0>(nn[1]->data));

}

void test_knn_Tuples_Manhattan() {
    std::vector<uint8_t> cls = {1,1,1};
    std::vector<std::tuple<uint8_t, float>> vec = {
        {1, 1.0},
        {2, 2.0},
        {3, 0.0}
    };
    
    std::vector<Observation<std::tuple<uint8_t, float>>> dataset;
    for(size_t i = 0; i < cls.size(); ++i) {
        Observation<std::tuple<uint8_t, float>> obs(cls[i], vec[i]);
        dataset.push_back(obs);
    }
    
    std::vector<uint8_t> v = {1, 2, 3};

    auto metup = std::make_tuple(ValueDifference{cls, v}, AbsoluteDifference{});
    auto tst   = std::make_tuple((uint8_t) 3, (float) 3.0);

    Manhattan<decltype(metup), decltype(tst)> metric(metup);

    auto nn = nearest_neighbours(
        metric,
        dataset,
        tst,
        2
    );

    printf("%d\n", std::get<0>(nn[0]->data));
    printf("%d\n", std::get<0>(nn[1]->data));

}

/*int main() { // może być tylko 1 main w targecie
    test_knn_Tuples_Max();
    test_knn_Tuples_Manhattan();
}*/

