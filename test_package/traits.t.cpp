#include "skizzay/utilz/traits.h"
#if __has_include(<catch/catch.hpp>)
#  include <catch/catch.hpp>
#elif __has_include(<catch.hpp>)
#  include <catch.hpp>
#endif
#include <array>
#include <chrono>
#include <memory>
#include <vector>

using namespace skizzay::utilz;

struct empty {};

struct tuple_child : std::tuple<int, double> {};


TEST_CASE( "is_template", "[unit][traits]" ) {
   using my_dto = dto<struct something, empty>;
   REQUIRE( is_template_v<my_dto, dto> );
   REQUIRE_FALSE( is_template_v<int, dto> );
   REQUIRE( is_dto_v<my_dto> );
   REQUIRE( is_template_v<std::chrono::system_clock::time_point, std::chrono::time_point> );
   REQUIRE_FALSE( is_template_v<nonesuch, dto> );
   REQUIRE_FALSE( is_template_v<nonesuch, std::variant> );
   REQUIRE_FALSE( is_template_v<nonesuch, std::tuple> );
   REQUIRE( inherits_from_template_v<tuple_child, std::tuple> );
   REQUIRE_FALSE( is_template_v<tuple_child, std::tuple> );
   REQUIRE_FALSE( inherits_from_template_v<tuple_child, std::variant> );
}


TEST_CASE( "pointers", "[unit][traits]" ) {
   using shared = std::shared_ptr<int>;
   using unique = std::unique_ptr<int>;
   using raw = int *;

   REQUIRE( std::is_pointer_v<raw> );
   REQUIRE( is_pointer_like_v<raw> );
   REQUIRE( std::is_lvalue_reference_v<decltype(to_reference(std::declval<dereference_t<raw>>()))> );

   REQUIRE_FALSE( std::is_pointer_v<shared> );
   REQUIRE( is_pointer_like_v<shared> );
   REQUIRE( std::is_lvalue_reference_v<decltype(to_reference(std::declval<dereference_t<shared>>()))> );

   REQUIRE_FALSE( std::is_pointer_v<unique> );
   REQUIRE( is_pointer_like_v<unique> );
   REQUIRE( std::is_lvalue_reference_v<decltype(to_reference(std::declval<dereference_t<unique>>()))> );
}


TEST_CASE( "ranges", "[unit][traits]" ){
   using array = std::array<int, 1>;

   REQUIRE_FALSE( is_range_v<int> );
   REQUIRE( is_detected_v<begin_t, array> );
   REQUIRE( is_pointer_like_v<detected_t<begin_t, array>> );
   REQUIRE( std::is_same_v<std::remove_reference_t<detected_t<begin_t, array>>,
                           std::remove_reference_t<detected_t<increment_t, std::add_lvalue_reference_t<detected_t<begin_t, array>>>>> );
   REQUIRE( is_detected_convertible_v<bool, begin_not_equal_to_end, array> );
   REQUIRE( is_range_v<array> );
   REQUIRE( is_range_v<std::vector<int>> );
}
