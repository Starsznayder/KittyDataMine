#ifndef ObservationH
#define ObservationH

#include <memory>

template <typename T>
struct Observation {
	const uint8_t target; 
	const T& data; 
    Observation(const uint8_t target, const T& data) : target(target), data(data) {}
};

#endif
