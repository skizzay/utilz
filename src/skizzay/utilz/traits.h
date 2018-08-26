#pragma once

#include "skizzay/utilz/detected.h"
#include <chrono>
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace skizzay::utilz {

template<class T, template<class ...> class Template>
class is_template {
   static nonesuch test(...);
   template<class ...Args>
   static Template<Args...> test(Template<Args...> * const);

public:
   using type = decltype(is_template<T, Template>::test(static_cast<T *>(nullptr)));
   using value_t = std::is_same<T, type>;
   static constexpr bool value = value_t::value;
};

template<template<class ...> class Template>
class is_template<nonesuch, Template> {
public:
   using type = nonesuch;
   using value_t = std::false_type;
   static constexpr bool value = value_t::value;
};

template<class T, template<class ...> class Template>
inline constexpr bool is_template_v = is_template<T, Template>::value;

template<class T, template<class ...> class Template>
struct inherits_from_template : std::is_base_of<typename is_template<T, Template>::type, T> {};

template<class T, template<class ...> class Template>
inline constexpr bool inherits_from_template_v  = inherits_from_template<T, Template>::value;

template<template<class ...> class Template>
struct inherits_from_template<nonesuch, Template> : std::false_type {};

template <class T, class U>
using equality_t = decltype(std::declval<std::remove_reference_t<T> const &>() ==
                          std::declval<std::remove_reference_t<U> const &>());

template <class T, class U>
using inequality_t = decltype(std::declval<std::remove_reference_t<T> const &>() !=
                            std::declval<std::remove_reference_t<U> const &>());

template <class T>
using begin_t = decltype(std::begin(std::declval<T>()));

template <class T>
using end_t = decltype(std::end(std::declval<T>()));

template <class T>
using increment_t = decltype(++std::declval<T>());

template <class T>
using begin_not_equal_to_end = decltype(std::declval<begin_t<T>>() != std::declval<end_t<T>>());

template<class T>
using dereference_t = decltype(*std::declval<T>());

template<class T>
using pointer_operator_t = decltype(std::declval<T>().operator->());

template<class, class=void>
struct is_dereferencible_to_reference : std::false_type {};
template<class T>
struct is_dereferencible_to_reference<T, std::void_t<dereference_t<T>>> : std::is_reference<dereference_t<T>> {};

template<class T>
using is_pointer_like = std::disjunction<std::is_pointer<T>,
      std::conjunction<is_dereferencible_to_reference<T>,
                       is_detected<pointer_operator_t, T>
      >>;
template<class T>
inline constexpr bool is_pointer_like_v = is_pointer_like<T>::value;

template<class T>
inline constexpr std::enable_if_t<!is_pointer_like<T>::value, std::remove_reference_t<T> &> to_reference(T &t) noexcept {
   return static_cast<std::remove_reference_t<T> &>(t);
}

template<class T>
inline constexpr auto to_reference(T &t) noexcept -> dereference_t<std::remove_reference_t<T>> {
   return *t;
}

template<class T>
using is_range = std::conjunction<
   is_detected<begin_t, T>,
   is_pointer_like<detected_t<begin_t, T>>,
   std::is_same<std::remove_reference_t<detected_t<begin_t, T>>,
                std::remove_reference_t<detected_t<increment_t, std::add_lvalue_reference_t<detected_t<begin_t, T>>>>>,
   is_detected_convertible<bool, begin_not_equal_to_end, T>
>;
template<class T>
inline constexpr bool is_range_v = is_range<T>::value;

template<class T, class U=T>
using equivalent = std::conjunction<
   is_detected_convertible<bool, equality_t, T, U>,
   is_detected_convertible<bool, equality_t, U, T>,
   is_detected_convertible<bool, inequality_t, T, U>,
   is_detected_convertible<bool, inequality_t, U, T>
>;
template<class T, class U=T>
inline constexpr bool equivalent_v = equivalent<T, U>::value;

template<class> struct tag {};

template<class T>
using is_tag = is_template<T, tag>;
template<class T>
inline constexpr bool is_tag_v = is_tag<T>::value;

template<class ...> struct dto;

template<class T>
using is_dto = is_template<T, dto>;
template<class T>
inline constexpr bool is_dto_v = is_dto<T>::value;

template<class T>
using is_variant = typename is_template<T, std::variant>::value_t;
template<class T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template<class T>
using is_in_place_type = typename is_template<T, std::in_place_type_t>::value_t;
template<class T>
inline constexpr bool is_in_place_type_v = is_in_place_type<T>::value;

template<class T>
using is_tuple = typename is_template<T, std::tuple>::value_t;
template<class T>
inline constexpr bool is_tuple_v = is_tuple<T>::value;

template<class T>
using is_time_point = is_template<T, std::chrono::time_point>;
template<class T>
inline constexpr bool is_time_point_v = is_time_point<T>::value;

template<class T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

template<class T>
struct strip_reference_wrapper {
   using type = T;

   static constexpr T &apply(T &t) noexcept {
      return t;
   }
};

template<class T>
struct strip_reference_wrapper<std::reference_wrapper<T>> : strip_reference_wrapper<T> {
   static constexpr T &apply(std::reference_wrapper<T> t) noexcept {
      return t.get();
   }
};

template<class T>
using strip_reference_wrapper_t = typename strip_reference_wrapper<T>::type;

template<class T>
using is_reference_wrapper = is_template<T, std::reference_wrapper>;

template<class T>
inline constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

}
