#include <cstdio>
#include <tuple>
#include <functional>

template <typename T>
struct Metric {
	float operator()(T arg);
};

struct A : Metric<int> {
	float operator()(int arg) {
		return (float) 2*arg;
	}
};

struct B : Metric<float> {
	float operator()(float arg) {
		return 3*arg;
	}
};

template <typename T>
float compute_metric(Metric<T>& metric, T& arg) {
	return metric(arg);	
}

template <typename ...Ts, size_t ...Is>
auto compute_metrics(

template <typename ...Ts> 
struct Apply {
	std::tuple<Metric<Ts>...> metrics;
	Apply(std::tuple<Metric<Ts>...> metrics) : metrics(metrics) {}
		
	auto operator()(std::tuple<Ts...> input) {
		return std::make_tuple(std::get<N>(metrics)(std::get<N>(input))...);
	}
};


int main() {
	
	auto ms = std::make_tuple(
		Metric<int>(), 
		Metric<float>()
	);

	auto ar = std::make_tuple(
		(int) 3,
		(float) 4.0 
	);

	Apply<int, float> apl(ms);

	std::tuple<float, float> res = apl(ar);	
	
	printf("%f\n", std::get<0>(res));
	printf("%f\n", std::get<1>(res));

	return 0;
}
