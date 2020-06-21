#include "skizzay/utilz/dto.h"

#if __has_include(<catch/catch.hpp>)
#  include <catch/catch.hpp>
#elif __has_include(<catch.hpp>)
#  include <catch.hpp>
#endif

namespace {

struct nothing {};
struct something {
   int int_field = {};
};

}

TEST_CASE( "metadata", "[unit][dto]" ) {
   using namespace skizzay::utilz;
   using target_type = dto<nothing, something, tag<struct made_up_tag>>;

   SECTION( "result is base of data types" ) {
      REQUIRE( std::is_base_of_v<nothing, target_type> );
      REQUIRE( std::is_base_of_v<something, target_type> );
      REQUIRE( !std::is_base_of_v<tag<struct made_up_tag>, target_type> );
   }

   SECTION( "data_types is type sequence of only data types" ) {
      using expected_type = type_sequence<nothing, something>;
      using actual_type = target_type::data_types;
      REQUIRE( std::is_same_v<expected_type, actual_type> );
   }

   SECTION( "tag_types is type sequence of only tag types" ) {
      using expected_type = type_sequence<tag<struct made_up_tag>>;
      using actual_type = target_type::tag_types;
      REQUIRE( std::is_same_v<expected_type, actual_type> );
   }
}
