#ifndef RuleH
#define RuleH 

#include <memory>
#include "metric.h"
#include "observation.h"

template <typename MetTypes, typename ArgTypes>
struct Rule {
	const uint8_t decision;
	const MetTypes& metrics;
	const ArgTypes& alpha;
	const ArgTypes& beta;
    std::array<float, std::tuple_size<ArgTypes>::value> distances;
	Rule(const uint8_t decision, const MetTypes& metrics, const ArgTypes& alpha, const ArgTypes& beta)
		: decision(decision), metrics(metrics), alpha(alpha), beta(beta) {	
		distances = compute_metrics(metrics, alpha, beta);
	}
	bool operator()(const ArgTypes& gamma) const {	
        auto dist_alpha = compute_metrics(metrics, alpha, gamma);
        auto dist_beta  = compute_metrics(metrics, beta,  gamma);
        for(size_t i = 0; i < distances.size(); ++i)
            if(distances[i] < dist_alpha[i] || distances[i] < dist_beta[i])
                return false;
        return true;
	}
};

#endif
