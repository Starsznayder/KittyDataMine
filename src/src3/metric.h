#ifndef MetricH
#define MetricH

#include <cstdio>
#include <vector>
#include <numeric> // this has reduce inside;
#include <memory>
#include <algorithm>

template <typename T>
struct Metric {
    virtual float operator()(const T& left, const T& right) const=0;
};


struct ValueDifference : public Metric<uint8_t> {

    std::vector<std::vector<float>> distances;

    ValueDifference(const std::vector<uint8_t>& cls,
                    const std::vector<uint8_t>& vec,
                    const int len_cls,
                    const int len_vec) : distances() {
        // uint8_t len_cls = *std::max_element(cls.begin(), cls.end()) + 1; 
        // uint8_t len_vec = *std::max_element(vec.begin(), vec.end()) + 1;

        
        std::vector<std::vector<float>> prob;
        prob.resize(len_vec);
        for(size_t i = 0; i < len_vec; ++i)
            prob[i].resize(len_cls, 0);
        
        for(size_t i = 0; i < cls.size(); ++i)
            prob[vec[i]][cls[i]] += 1.0; 

        for(size_t j = 0; j < len_vec; ++j) {
            float sum = std::reduce(prob[j].begin(), prob[j].end());
            for(size_t i = 0; i < len_cls; ++i) prob[j][i] /= sum;
        }

        distances.resize(len_vec);
        for(size_t i = 0; i < len_vec; ++i)
            distances[i].resize(len_vec, 0);
        
        for(size_t i = 0; i < len_vec; ++i)
            for(size_t j = 0; j < len_vec; ++j)
                for(size_t c = 0; c < len_cls; ++c)
                    distances[i][j] += std::abs(prob[i][c] - prob[j][c]);
    }

    float operator()(const uint8_t& left,
                     const uint8_t& right) const { 
        return distances[left][right];
    }
};


struct AbsoluteDifference : public Metric<float> {
    float operator()(const float& left,
                     const float& right) const {
        return std::abs(left - right); 
    }
};


// -------------------- TUPLES ------------------- //
template <typename M, typename T>
float compute_metric(const M& metric, const T& lhs, const T& rhs) {
	return metric(lhs, rhs);	
}

template <typename ...Ms, typename ...As, size_t ...Is>
std::array<float, sizeof...(Is)> compute_metrics_helper(const std::tuple<Ms...>& metrics,
                                                        const std::tuple<As...>& lhss,
                                                        const std::tuple<As...>& rhss,
                                                        const std::index_sequence<Is...>) {
    return { compute_metric(std::get<Is>(metrics),
							std::get<Is>(lhss),
                            std::get<Is>(rhss))... };
}

template <typename ...Ms, typename ...Ts>
std::array<float, sizeof...(Ms)> compute_metrics(const std::tuple<Ms...>& metrics,
		                                         const std::tuple<Ts...>& lhss,
                                                 const std::tuple<Ts...>& rhss) {
	static_assert(sizeof...(Ms) == sizeof...(Ts), "wrong sizes");
	return compute_metrics_helper(metrics, lhss, rhss, std::make_index_sequence<sizeof...(Ts)>());
}


template <typename MetTypes, typename ArgTypes>
struct Max : public Metric<ArgTypes> {
    MetTypes& metrics;
    Max(MetTypes& metrics) : metrics(metrics) {}
	float operator()(const ArgTypes& lhss,
				     const ArgTypes& rhss) const {
        auto diffs = compute_metrics(metrics, lhss, rhss);
		return *std::max_element(diffs.begin(), diffs.end());
	}
};

template <typename MetTypes, typename ArgTypes>
struct Manhattan : public Metric<ArgTypes> {
    MetTypes& metrics; 
    Manhattan(MetTypes& metrics) : metrics(metrics) {}
    float operator()(const ArgTypes& lhss,
                     const ArgTypes& rhss) const {
        auto diffs = compute_metrics(metrics, lhss, rhss);
        return std::reduce(diffs.begin(), diffs.end());
    }
};


#endif
