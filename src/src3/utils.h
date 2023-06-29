#ifndef UtilsH
#define UtilsH

#include <vector>
#include <memory>
#include <algorithm>


float accuracy(std::vector<uint8_t> target,
               std::vector<uint8_t> prediction) {
    size_t s = 0;
    for(size_t i = 0; i < target.size(); ++i) {
        s += target[i] == prediction[i];
    }
    return static_cast<float>(s)/target.size();
}

float balanced_accuracy(std::vector<uint8_t> target,
                        std::vector<uint8_t> prediction,
                        size_t number_of_classes) {
    
    std::vector<size_t> true_cls(number_of_classes);
    std::vector<size_t> all_cls(number_of_classes);
    for(size_t i = 0; i < target.size(); ++i) {
        true_cls[static_cast<int>(target[i])] += target[i] == prediction[i];
        all_cls[static_cast<int>(target[i])]++;
    }
    float s = 0;
    for(size_t i = 0; i < number_of_classes; ++i)
        s += static_cast<float>(true_cls[i])/all_cls[i];
    return s / number_of_classes;
}

#endif
