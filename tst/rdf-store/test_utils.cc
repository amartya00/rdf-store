#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rdf-store/utils.hpp>

using rdfstore::models::LiteralTerm;
using rdfstore::utils::LiteralGenerator;

using rdfstore::utils::get_literal_datatype_iri_id;
using rdfstore::utils::LiteralGenerator;
using rdfstore::models::LiteralTerm;


template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<int>() {
    return 1;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned int >() {
    return 2;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<long>() {
    return 3;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned long >() {
    return 4;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<float>() {
    return 5;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<double>() {
    return 6;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<std::string>() {
    return 7;
}

SCENARIO("Literal generator test for integers and unsigned integers.") {

    GIVEN("I try to convert an integer / unsigned integer to a literal.") {
        int x {-1023};
        unsigned int y {101};

        THEN("I should be able to do so.") {

            LiteralTerm term_int {LiteralGenerator<int>::serialize(x)};
            LiteralTerm term_unsigned_int {LiteralGenerator<unsigned int>::serialize(y)};

            REQUIRE(get_literal_datatype_iri_id<int>() == term_int.datatype_iri_id);
            REQUIRE(get_literal_datatype_iri_id<unsigned int>() == term_unsigned_int.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    int xx {LiteralGenerator<int>::deserialize(term_int).unwrap()};
                    unsigned int yy {LiteralGenerator<unsigned int>::deserialize(term_unsigned_int).unwrap()};
                    REQUIRE(x == xx);
                    REQUIRE(y == yy);
                }
            }
        }
    }
}

SCENARIO("Literal generator test for Long Integer.") {

    GIVEN("I try to convert a long integer to a literal.") {
        long int x {1023};

        THEN("I should be able to do so.") {

            LiteralTerm term {LiteralGenerator<long int>::serialize(x)};
            REQUIRE(get_literal_datatype_iri_id<long int>() == term.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    long int xx {LiteralGenerator<long int>::deserialize(term).unwrap()};
                    REQUIRE(x == xx);
                }
            }
        }
    }
}

SCENARIO("Literal generator test for Float.") {

    GIVEN("I try to convert a float to a literal.") {
        float x {101.23765f};

        THEN("I should be able to do so.") {

            LiteralTerm term {LiteralGenerator<float>::serialize(x)};
            REQUIRE(get_literal_datatype_iri_id<float>() == term.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    auto res {LiteralGenerator<float>::deserialize(term)};
                    REQUIRE(res);
                    REQUIRE(x == res.unwrap());
                }
            }
        }
    }
}

SCENARIO("Literal generator for string.") {

    GIVEN("I want to convert a string to a literal.") {

        std::string test_str {"This is a test string"};

        THEN("I should be able to do so.") {

            LiteralTerm term {LiteralGenerator<std::string>::serialize(test_str)};
            REQUIRE(get_literal_datatype_iri_id<std::string>() == term.datatype_iri_id);

            AND_WHEN("I try to deserialize the same.") {

                THEN("I should be able to do so.") {

                    auto res {LiteralGenerator<std::string>::deserialize(term).unwrap()};
                    REQUIRE(test_str == res);
                }
            }
        }
    }
}