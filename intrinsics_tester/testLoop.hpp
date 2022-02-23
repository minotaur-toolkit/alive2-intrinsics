#include <utility>

// Static for loops that support constexpr

template <typename F, std::size_t... S>
constexpr void static_for(F &&function, std::index_sequence<S...>) {
  int unpack[] = {
      0, (void(function(std::integral_constant<std::size_t, S>{})), 0)...};

  (void)unpack;
}

template <std::size_t iterations, typename F>
constexpr void static_for(F &&function) {
  static_for(std::forward<F>(function), std::make_index_sequence<iterations>());
}
