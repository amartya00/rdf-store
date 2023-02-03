#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rdf-store/utils.hpp>
#include <unordered_map>

#include <iostream>

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

SCENARIO("Literal generator test for Integer.") {

    GIVEN("I try to convert an integer to a literal.") {
        int x {1023};

        THEN("I should be able to do so.") {

            LiteralTerm term {LiteralGenerator<int>::serialize(x)};
            REQUIRE(get_literal_datatype_iri_id<int>() == term.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    int xx {LiteralGenerator<int>::deserialize(term).unwrap()};
                    REQUIRE(x == xx);
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

                    float xx {LiteralGenerator<float>::deserialize(term).unwrap()};
                    std::cout << "-----------------------------" << xx << "\n";
                    REQUIRE(false);
                }
            }
        }
    }
}