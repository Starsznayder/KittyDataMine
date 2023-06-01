#ifndef RuleH
#define RuleH 

#include <memory>
#include "metric.h"


template <typename T>
struct Rule {
	uint8_t decision;
	float distance;
	Metric<T>& metric;
	T& alpha;
	T& beta;
	Rule(uint8_t decision, Metric<T>& metric, T& alpha, T& beta)
		: decision(decision), metric(metric), alpha(alpha), beta(beta) {	
		distance = metric(alpha, beta);
	}
	bool operator()(const T& gamma) {	
		return metric(gamma, alpha) <= distance && metric(gamma, beta) <= distance;
	}
}

#endif
