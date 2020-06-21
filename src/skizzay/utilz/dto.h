#pragma once

#include "skizzay/utilz/traits.h"
#include "skizzay/utilz/type_sequence.h"
#include <type_traits>
#include <utility>

namespace skizzay::utilz {

namespace details_ {

template<class> struct tags_holder;

template<class ...Tags>
struct tags_holder<utilz::type_sequence<Tags...>> {
   using tag_types = utilz::type_sequence<Tags...>;
};

template<class> struct data_holder;

template<class ...Data>
struct data_holder<utilz::type_sequence<Data...>> : Data ... {
   using data_types = utilz::type_sequence<Data...>;
   constexpr data_holder(Data ...data) noexcept(
         std::conjunction_v<std::is_nothrow_move_constructible<Data>...>
       )
       : Data{std::move(data)}... {}
};

template<class T> using is_data = std::negation<is_tag<T>>;
template<class ...Ts> using data_holder_t = data_holder<utilz::filter_t<utilz::type_sequence<Ts...>, is_data>>;
template<class ...Ts> using tags_holder_t = tags_holder<utilz::filter_t<utilz::type_sequence<Ts...>, is_tag>>;

}

template <class... TagsAndData>
struct dto : details_::tags_holder_t<TagsAndData...>,
             details_::data_holder_t<TagsAndData...> {
   using tag_types = typename details_::tags_holder_t<TagsAndData...>::tag_types;
   using data_types = typename details_::data_holder_t<TagsAndData...>::data_types;
   using details_::data_holder_t<TagsAndData...>::data_holder;
};

template<class ...DataOnly>
dto(DataOnly ...) -> dto<remove_cref_t<DataOnly>...>;

}
