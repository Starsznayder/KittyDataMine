#ifndef ApplyNH
#define ApplyNH

template<typename ...F>
class ApplyN
{
public:
    // using return_type = std::tuple<typename F::return_type...>;
		

    ApplyN(const Metric<F>&... ms) : metrics_{ms...} {}

	float operator()(const std::tuple<F...> args) const
    {
        return with_indices(t, IndicesFor<std::tuple<F...> >{});
    }

protected:
    std::tuple<Metric<F>...> metrics_;

    template <typename T, std::size_t... Indices>
    float with_indices(const T& t, indices<Indices...>) const
    {
        return float{std::get<Indices>(metrics_)(t)...};
    }
};

#endif
