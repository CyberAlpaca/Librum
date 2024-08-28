#include <catch2/catch_test_macros.hpp>

#define CHECK_MSG(cond, msg) do { INFO(msg); CHECK(cond); } while((void)0, 0)
#define REQUIRE_MSG(cond, msg) do { INFO(msg); REQUIRE(cond); } while((void)0, 0)
#define REQUIRE_THAT_MSG(arg, matcher, msg) do { INFO(msg); REQUIRE_THAT(arg, matcher); } while((void)0, 0)
