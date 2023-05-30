#include <cstdio>
#include <memory>
#include "knn.h"

int main() {
    std::vector<uint8_t> cls = {1,1,1,2,2,0,0};
    std::vector<uint8_t> vec = {0,1,0,1,2,2,3};
    ValueDifference metric(cls, vec);
    for(size_t i = 0; i < 4; i++) {
        for(size_t j = 0; j < 4; j++) {
            printf(" %f ", metric((uint8_t) i, (uint8_t) j));
        }
        printf("\n"); 
    } 

    auto nn = nearest_neighbours(metric, vec, (uint8_t) 3, (size_t) 2);


    printf("%d\n", nn[0]); 
    printf("%d\n", nn[1]); 
    
}

