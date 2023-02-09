#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <rdf-store/utils.hpp>

using rdfstore::utils::get_literal_datatype_iri_id;
using rdfstore::utils::LiteralGenerator;
using rdfstore::models::LiteralTerm;
using UtilsErrorCode = rdfstore::utils::ErrorCode;

template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<int>() {
    return 1;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned int>() {
    return 2;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<long>() {
    return 3;
}
template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned long>() {
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
        unsigned int y {101U};

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

SCENARIO("Literal generator test for long integer ans unsigned long integer.") {

    GIVEN("I try to convert a long integer to a literal.") {
        long int x {-1023};
        unsigned long y {101UL};

        THEN("I should be able to do so.") {

            LiteralTerm term_long {LiteralGenerator<long int>::serialize(x)};
            LiteralTerm term_unsigned_long {LiteralGenerator<unsigned long>::serialize(y)};

            REQUIRE(get_literal_datatype_iri_id<long int>() == term_long.datatype_iri_id);
            REQUIRE(get_literal_datatype_iri_id<unsigned long>() == term_unsigned_long.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    long int xx {LiteralGenerator<long int>::deserialize(term_long).unwrap()};
                    unsigned long yy {LiteralGenerator<unsigned long>::deserialize(term_unsigned_long).unwrap()};

                    REQUIRE(x == xx);
                    REQUIRE(y == yy);
                }
            }
        }
    }
}

SCENARIO("Literal generator test for float and double.") {

    GIVEN("I try to convert a float to a literal.") {
        const float x {101.23765f};
        const double y {110.23};

        THEN("I should be able to do so.") {

            LiteralTerm term_float {LiteralGenerator<float>::serialize(x)};
            LiteralTerm term_double {LiteralGenerator<double>::serialize(y)};

            REQUIRE(get_literal_datatype_iri_id<float>() == term_float.datatype_iri_id);
            REQUIRE(get_literal_datatype_iri_id<double>() == term_double.datatype_iri_id);

            AND_WHEN("I deserialize the same.") {

                THEN("I should get back the original value.") {

                    float xx {LiteralGenerator<float>::deserialize(term_float).unwrap()};
                    double yy {LiteralGenerator<double>::deserialize(term_double).unwrap()};
                    REQUIRE(x == xx);
                    REQUIRE(y == yy);
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

SCENARIO("Failure scenarios for scalar types.") {

    GIVEN("I have a scalar term with incorrect byte buffer length.") {

        constexpr std::size_t bad_bytes {sizeof(unsigned long) + 1};

        std::vector<std::byte> bytes {};
        for (auto i = 0UL; i < bad_bytes; i++) {
            bytes.push_back({});
        }
        LiteralTerm bad_unsigned_long_term {
            .bytes {std::move(bytes)},
            .datatype_iri_id {get_literal_datatype_iri_id<unsigned long >()}
        };

        WHEN("I try to deserialize it.") {

            auto res {LiteralGenerator<unsigned long>::deserialize(bad_unsigned_long_term)};

            THEN("I should get an error.") {

                REQUIRE_FALSE(res);
                REQUIRE(UtilsErrorCode::INVALID_LITERAL == res.error());
            }
        }
    }

    AND_GIVEN("I have a literal term with an invalid datatype type IRI id.") {

        LiteralTerm term {LiteralGenerator<float>::serialize(1.2f)};
        LiteralTerm bad_term {
            .bytes {std::move(term.bytes)},
            .datatype_iri_id {get_literal_datatype_iri_id<int>()}
        };


        WHEN("I try to deserialize it.") {

            auto res {LiteralGenerator<float>::deserialize(bad_term)};

            THEN("I should get an error.") {

                REQUIRE_FALSE(res);
                REQUIRE(UtilsErrorCode::INVALID_DESERIALIZATION_REQUEST == res.error());
            }
        }
    }
}