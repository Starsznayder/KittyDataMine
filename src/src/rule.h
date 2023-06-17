#ifndef RuleH
#define RuleH 

#include <memory>
#include "metric.h"
#include "observation.h"


template <typename T>
struct Rule {
	const uint8_t decision;
	const Metric<T>& metric;
	const T& alpha;
	const T& beta;
    float distance;
	Rule(const uint8_t decision, const Metric<T>& metric, const T& alpha, const T& beta)
		: decision(decision), metric(metric), alpha(alpha), beta(beta) {	
		distance = metric(alpha, beta);
	}
	bool operator()(const T& gamma) const {	
		return metric(gamma, alpha) <= distance && metric(gamma, beta) <= distance;
	}
};


#endif
