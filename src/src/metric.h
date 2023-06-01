#ifndef MetricH
#define MetricH

#include <cstdio>
#include <vector>
#include <numeric>
#include <memory>
#include <algorithm>


template <typename T>
struct Metric {
    virtual float operator()(T left, T right) = 0;
};


struct ValueDifference : public Metric<uint8_t> {

    std::vector<std::vector<float>> distances;

    ValueDifference(std::vector<uint8_t>& cls, std::vector<uint8_t>& vec) : distances() {
        uint8_t len_cls = *std::max_element(cls.begin(), cls.end()) + 1; 
        uint8_t len_vec = *std::max_element(vec.begin(), vec.end()) + 1;
        
        std::vector<std::vector<float>> prob;
        prob.resize((int) len_vec);
        for(size_t i = 0; i < len_vec; ++i)
            prob[i].resize((int) len_cls, 0);
        
        for(size_t i = 0; i < cls.size(); ++i)
            prob[vec[i]][cls[i]] += 1.0; 

        for(size_t j = 0; j < len_vec; ++j) {
            float sum = std::reduce(prob[j].begin(), prob[j].end());
            for(size_t i = 0; i < len_cls; ++i) prob[j][i] /= sum;
        }

        distances.resize((int) len_vec);
        for(size_t i = 0; i < len_vec; ++i)
            distances[i].resize((int) len_vec, 0);
        
        for(size_t i = 0; i < len_vec; ++i)
            for(size_t j = 0; j < len_vec; ++j)
                for(size_t c = 0; c < len_cls; ++c)
                    distances[i][j] += std::abs(prob[i][c] - prob[j][c]);
    }

    float operator()(uint8_t left, uint8_t right) { 
        return distances[left][right];
    }
};


struct Manhattan : public Metric<float> {
    
    float operator()(float left, float right) {
        return std::abs(left - right); 
    }
};


template <typename... Types>
struct Max : public Metric<std::tuple<Types...>> {
	std::tuple<Metric<Types>...> metrics;
	Max(std::tuple<Metric<Types>...> metrics) : metrics(metrics) {}
	float operator()(std::tuple<Types...> left,
				     std::tuple<Types...> right) {
		return *std::max_element(
	}
}

#endif
