#include "skizzay/utilz/type_sequence.h"
#if __has_include(<catch/catch.hpp>)
#  include <catch/catch.hpp>
#elif __has_include(<catch.hpp>)
#  include <catch.hpp>
#endif

using namespace skizzay::utilz;

struct A {};
struct B {};
struct C {};
struct D {};

template<class T, class U>
struct same_ : std::is_same<T, U> {};

template<class T, std::size_t I, class U>
struct same_<T, type_sequence<size_constant<I>, U>> : same_<T, U> {};

template<class T>
struct matches {
   template<class U>
   using apply = same_<T, U>;
};
template<class T>
using matches_A = apply_f<matches<A>, T>;

template<class T>
using to_D = D;

TEST_CASE( "all/any/none of", "[unit][type_sequence]" ) {
   REQUIRE( all_of_v<type_sequence<A, A>, matches_A> );
   REQUIRE_FALSE( all_of_v<type_sequence<A, B>, matches_A> );
   REQUIRE( any_of_v<type_sequence<A, B>, matches_A> );
   REQUIRE_FALSE( any_of_v<type_sequence<B, B>, matches_A> );
   REQUIRE( none_of_v<type_sequence<B, B>, matches_A> );
   REQUIRE_FALSE( none_of_v<type_sequence<A, B>, matches_A> );
}


TEST_CASE( "count/length/empty", "[unit][type_sequence]" ) {
   using Types = type_sequence<A, A, B, C>;
   REQUIRE( 4 == length_v<Types> );
   REQUIRE_FALSE( empty_v<Types> );
   REQUIRE( empty_v<type_sequence<>> );
   REQUIRE( 2 == count_of_v<Types, matches_A> );
}


TEST_CASE( "access", "[unit][type_sequence]" ) {
   using Types = type_sequence<A, A, B, C>;
   REQUIRE( 0 == first_index_of_v<Types, A> );
   REQUIRE( std::is_same_v< std::index_sequence<0, 1>, indices_of<Types, A> > );
   REQUIRE_FALSE( empty_v<Types> );
   REQUIRE( empty_v<type_sequence<>> );
   REQUIRE( 2 == count_of_v<Types, matches_A> );

   REQUIRE( std::is_same_v<C, back_t<Types>> );
   REQUIRE( std::is_same_v<A, front_t<Types>> );
   REQUIRE( std::is_same_v<B, at_t<Types, 2>> );
}


TEST_CASE( "combining", "[unit][type_sequence]" ) {
   REQUIRE( std::is_same_v< type_sequence<A, B>, concat_t<type_sequence<>, type_sequence<A>, type_sequence<B>, type_sequence<>> > );
   REQUIRE( std::is_same_v< type_sequence<A, B>, concat_t< type_sequence<A>, type_sequence<>,type_sequence<B>, type_sequence<>> > );
   REQUIRE( std::is_same_v< type_sequence<A, B>, concat_t<type_sequence<>, type_sequence<A>, type_sequence<B>> > );
   REQUIRE( std::is_same_v< type_sequence<A, B>, concat_t<type_sequence<A>, type_sequence<B>> > );

   REQUIRE( std::is_same_v< std::index_sequence<5, 8>, concat_t<std::index_sequence<>, std::index_sequence<5>, std::index_sequence<8>, std::index_sequence<>> > );
   REQUIRE( std::is_same_v< std::index_sequence<5, 8>, concat_t< std::index_sequence<5>, std::index_sequence<>,std::index_sequence<8>, std::index_sequence<>> > );
   REQUIRE( std::is_same_v< std::index_sequence<5, 8>, concat_t<std::index_sequence<>, std::index_sequence<5>, std::index_sequence<8>> > );
   REQUIRE( std::is_same_v< std::index_sequence<5, 8>, concat_t<std::index_sequence<5>, std::index_sequence<8>> > );

   using Types = type_sequence<A, C>;
   REQUIRE( std::is_same_v< type_sequence<type_sequence<size_constant<0>, A>, type_sequence<size_constant<1>, C>>,
                            indexed_t<Types> > );
   REQUIRE( std::is_same_v< type_sequence<C, A>, reverse_t<Types> > );
}


TEST_CASE( "filtering", "[unit][type_sequence]" ) {
   using Types = type_sequence<C, A, B>;

   REQUIRE( std::is_same_v< type_sequence<A>, filter_t<Types, matches_A> > );
   REQUIRE( std::is_same_v< std::index_sequence<1>, to_filtered_indices_t<Types, matches_A> > );

   REQUIRE( std::is_same_v< type_sequence<C, A>, take_t<Types, 2> > );
   REQUIRE( std::is_same_v< type_sequence<B>, skip_t<Types, 2> > );
   REQUIRE( std::is_same_v< type_sequence<A>, take_t<skip_t<Types, 1>, 1> > );
}


TEST_CASE( "transformations", "[unit][type_sequence]" ) {
   using Types = type_sequence<C, A, B>;

   REQUIRE( std::is_same_v<std::tuple<C, A, B>, as_tuple_t<Types>> );
   REQUIRE( std::is_same_v<std::variant<C, A, B>, as_variant_t<Types>> );
   REQUIRE( std::is_same_v<Types, as_type_sequence_t<Types>> );
   REQUIRE( std::is_same_v<type_sequence<D, D, D>, map_t<Types, to_D>> );
}
