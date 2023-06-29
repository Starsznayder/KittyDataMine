#include <iostream>
#include <tuple>
#include <functional>

template <typename T>
struct Metric {
	virtual float operator()(T arg)=0;
};

struct A : Metric<int> {
	float operator()(int arg) {
		return (float) 2 * arg;
	}
};

struct B : Metric<float> {
	float operator()(float arg) {
		return 3*arg;
	}
};

template <typename M, typename T>
float compute_metric(M metric, T arg) {
	return metric(arg);	
}

template <typename ...Ms, typename ...As, size_t ...Is>
auto compute_metrics_helper(std::tuple<Ms...> metrics, std::tuple<As...> args, std::index_sequence<Is...>) {
	return std::make_tuple(compute_metric(std::get<Is>(metrics),
									      std::get<Is>(args))...);
}

template <typename ...Ms, typename ...Ts>
auto compute_metrics(std::tuple<Ms...> metrics,
		             std::tuple<Ts...> args) {
	static_assert(sizeof...(Ms) == sizeof...(Ts), "wrong sizes");
	return compute_metrics_helper(metrics, args, std::make_index_sequence<sizeof ...(Ts)>());
}

// template <typename ...Ts> 
// struct Apply {
// 	std::tuple<Metric<Ts>...> metrics;
// 	Apply(std::tuple<Metric<Ts>...> metrics) : metrics(metrics) {}
// 		
// 	auto operator()(std::tuple<Ts...> input) {
// 		return std::make_tuple(std::get<N>(metrics)(std::get<N>(input))...);
// 	}
// };

//nie może być więcej niż  jeden main na target w cmake
/*int main() {


	A m_int; 
	B m_float; 
	
	int a_int = 3; 
	float a_float = 4.0;

	auto ms = std::make_tuple(
		m_int, m_float
	);

	auto ar = std::make_tuple(
		a_int, 
		a_float
	);

    //Apply<int, float> apl(ms);

	std::tuple<float, float> res = compute_metrics(ms, ar);	
    std::cout << std::get<0>(res) << std::endl;
    std::cout << std::get<1>(res) << std::endl;

	return 0;
}*/
