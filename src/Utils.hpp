#pragma once

#include <variant>
#include <utility>

template<class... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Overload(Ts...) -> Overload<Ts...>;

template<class Variant, class... Fs>

decltype(auto) match(Variant&& value, Fs&&... cases) {
    return std::visit(
        Overload{std::forward<Fs>(cases)...},
        std::forward<Variant>(value)
    );
}