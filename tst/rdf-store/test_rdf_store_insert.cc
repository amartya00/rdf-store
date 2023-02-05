#define CATCH_CONFIG_MAIN

#include <variant>

#include <catch2/catch_all.hpp>
#include <rdf-store/models.hpp>
#include <rdf-store/quadstore.hpp>
#include <rdf-store/utils.hpp>

using rdfstore::quadstore::QuadStore;
using rdfstore::models::Triple;
using rdfstore::models::Node;
using rdfstore::models::IRITerm;
using rdfstore::models::LiteralTerm;
using rdfstore::models::NodeType;
using rdfstore::models::Predicate;
using rdfstore::utils::get_literal_datatype_iri_id;
using rdfstore::utils::LiteralGenerator;

constexpr std::string_view emp001_iri = "http://employees.com/emp001";
constexpr std::string_view has_id_iri = "http://employees.com/id/001";

template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned long>() {
    return 2;
}

SCENARIO("RDF store insertions.") {

    GIVEN("I have a red store.") {
        QuadStore qs;

        WHEN("I insert some elements into it.") {

            Node emp001 {Node::init(IRITerm {emp001_iri})};
            Predicate has_id {IRITerm {has_id_iri}};
            Node emp001_id {Node::init(LiteralGenerator<unsigned long>::serialize(100UL))};
            auto res{qs.insert_triple(1, emp001, has_id, emp001_id)};
            REQUIRE(res);
            REQUIRE(has_id_iri == has_id.iri.iri);
            REQUIRE(emp001_iri == std::get<IRITerm>(emp001.term).iri);
            REQUIRE(100UL == LiteralGenerator<unsigned long>::deserialize(std::get<LiteralTerm>(emp001_id.term)).unwrap());

        }
    }
}
