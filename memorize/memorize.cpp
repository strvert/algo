namespace hash_tuple {
template <typename T>
size_t hash_combine(const size_t seed, const T& v) noexcept
{
    if constexpr (sizeof(size_t) == 8) {
        return seed ^ (std::hash<T>()(v) + 0x9e3779b97f4a7c15 + (seed << 12) + (seed >> 4));
    } else if constexpr (sizeof(size_t) == 4) {
        return seed ^ (std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
    } else {
        static_assert(sizeof(size_t) == 4 || sizeof(size_t) == 8);
    }
}

template <int N>
struct hash_tuple_core {
    template <typename Tuple>
    size_t operator()(const Tuple& keyval) const noexcept
    {
        return hash_combine(hash_tuple_core<N - 1>()(keyval), std::get<N - 1>(keyval));
    }
};
template <>
struct hash_tuple_core<0> {
    template <typename Tuple>
    size_t operator()(const Tuple&) const noexcept { return 0; }
};
}

template <typename T>
struct function_traits;
template <typename RetT, typename... Args>
struct function_traits<RetT(Args...)> {
    using result_type = RetT;
    using args_tuple = std::tuple<Args...>;
};

template <typename BodyT, typename Signatures>
struct Memo {
    using ArgsTuple = typename function_traits<Signatures>::args_tuple;
    using RetT = typename function_traits<Signatures>::result_type;
    RetT operator()(const auto&... args) noexcept
    {
        return self(args...);
    }
    RetT self(const auto&... args) noexcept
    {
        auto [it, inserted] = memo.emplace(std::make_tuple(args...), RetT {});
        if (!inserted) {
            return it->second;
        }
        return it->second = static_cast<BodyT&>(*this).body(args...);
    }

    using MemoT = std::unordered_map<ArgsTuple, RetT, std::function<size_t(const ArgsTuple&)>>;
    MemoT memo = MemoT(0, [](const ArgsTuple& keyval) noexcept -> size_t {
        return hash_tuple::hash_tuple_core<std::tuple_size<ArgsTuple>::value>()(keyval);
    });
};
