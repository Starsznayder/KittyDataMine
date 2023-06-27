#ifndef ObservationH
#define ObservationH

#include <memory>

template <typename T>
struct Observation {
    const size_t id;
	const uint8_t target; 
	const T& data; 
    Observation(const size_t id, const uint8_t target, const T& data)
        : id(id), target(target), data(data) {}
};

#endif
