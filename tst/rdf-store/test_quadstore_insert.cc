#define CATCH_CONFIG_MAIN

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

constexpr std::string_view emp001_iri = "http://employees.com/emp001";
constexpr std::string_view has_id_iri = "http://employees.com/id/001";

SCENARIO("RDF store insertions.") {

    GIVEN("I have a red store.") {
        QuadStore qs;

        WHEN("I insert some elements into it.") {

            Node emp001 {Node::init(IRITerm {emp001_iri})};
            Predicate has_id {IRITerm {has_id_iri}};
            //Node emp001_id {LiteralTerm {}};
            REQUIRE(emp001_iri == has_id.iri.iri);
            REQUIRE(false);
        }
    }
}