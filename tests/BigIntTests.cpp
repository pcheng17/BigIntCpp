#include <doctest/doctest.h>
#include "yabi/BigInt.h"
#include <iostream>

using namespace yabi;

TEST_SUITE("BigInt tests")
{
    TEST_CASE("Construct BigInt")
    {
        SUBCASE("Large enough to be dynamic")
        {
            BigInt a((1 << 30) + 1);
            CHECK(!a.convertibleToU32());
        }
    }

    TEST_CASE("Copy constructor")
    {
        SUBCASE("Static to static")
        {
            BigInt a(42);
            BigInt b(a);

            CHECK(a == b);
            CHECK(a.convertibleToU32());
            CHECK(b.convertibleToU32());
        }

        SUBCASE("Dynamic to dynamic")
        {
            BigInt a((1 << 30));
            BigInt b(a);

            CHECK(a == b);
            CHECK(!a.convertibleToU32());
            CHECK(!b.convertibleToU32());
        }
    }

    TEST_CASE("Equality operator")
    {
        BigInt a(42);
        BigInt b(42);
        BigInt c(43);

        CHECK(a == b);
        CHECK(a != c);
        CHECK(b != c);
    }

    TEST_CASE("Addition operator")
    {
        BigInt actual, expected;

        SUBCASE("Static addition")
        {
            BigInt a(42);
            BigInt b(58);
            actual = a + b;
            expected = BigInt(100);
        }

        SUBCASE("Overflow into dynamic")
        {
            const uint32_t num = (1 << 30) - 1;
            BigInt a((1 << 30) - 1);
            BigInt b((1 << 30) - 1);
            REQUIRE(a.convertibleToU32());
            REQUIRE(b.convertibleToU32());

            actual = a + b;
            expected = BigInt(2 * num);
        }

        CHECK(actual == expected);
    }
}
