#pragma once

#include <type_traits>

namespace skizzay::utilz {

template<class ...Fs> struct overload : Fs... { using Fs::operator()...; };
template<class ...Fs> overload(Fs &&...) -> overload<std::remove_const_t<std::remove_reference_t<Fs>>...>;

}
