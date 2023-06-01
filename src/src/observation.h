#ifndef ObservationH
#define ObservationH

#include <memory>

template <typename T>
struct Observation {
	uint8_t target; 
	T& data; 
}

#endif
