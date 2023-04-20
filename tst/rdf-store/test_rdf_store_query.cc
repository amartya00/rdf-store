#define CATCH_CONFIG_MAIN

#include <algorithm>

#include <catch2/catch_all.hpp>
#include <rdf-store/quadstore.hpp>
#include <rdf-store/models.hpp>
#include <rdf-store/utils.hpp>


using rdfstore::quadstore::QuadStore;
using rdfstore::models::LiteralTerm;
using rdfstore::models::IRITerm;
using rdfstore::models::Node;
using rdfstore::models::Predicate;
using rdfstore::models::QueryFilter;

using GenStringLiteral = rdfstore::utils::LiteralGenerator<std::string>;
using GenLongLiteral = rdfstore::utils::LiteralGenerator<unsigned long>;

SCENARIO("Query by subject.") {

    GIVEN("I have a rdf store and have some nodes and edges to insert.") {

        QuadStore qs {};

        Predicate has_name {IRITerm {"iri://properties/name"}};
        Predicate department {IRITerm {"iri://properties/department"}};
        Predicate salary {IRITerm {"iri://properties/salary"}};
        Predicate has_population {IRITerm {"iri://properties/population"}};

        Node emp001 {Node::init(IRITerm {"iri://database/employees/001"})};
        Node emp002 {Node::init(IRITerm {"iri://database/employees/002"})};
        Node emp003 {Node::init(IRITerm {"iri://database/employees/003"})};

        Node amartya {Node::init(GenStringLiteral::serialize("Amartya Datta Gupta"))};
        Node bob {Node::init(GenStringLiteral::serialize("Bob Bob"))};
        Node hal {Node::init(GenStringLiteral::serialize("Hal Lah"))};

        Node engineering {Node::init(IRITerm {"iri://database/departments/engineering"})};
        Node sales {Node::init(IRITerm {"iri://database/departments/sales"})};

        Node high_salary {Node::init(GenLongLiteral::serialize(100000UL))};
        Node mid_salary {Node::init(GenLongLiteral::serialize(80000UL))};
        Node low_salary {Node::init(GenLongLiteral::serialize(30000UL))};

        Node sales_population {Node::init(GenLongLiteral::serialize(80UL))};

        WHEN("I insert them.") {

            auto t1 { qs.insert_triple(0, emp001, has_name, amartya).unwrap() };
            auto t2 { qs.insert_triple(0, emp001, department, engineering).unwrap() };
            auto t3 { qs.insert_triple(0, emp001, salary, high_salary).unwrap() };

            auto t4 { qs.insert_triple(0, emp002, has_name, bob).unwrap() };
            auto t5 { qs.insert_triple(0, emp002, department, sales).unwrap() };
            auto t6 { qs.insert_triple(0, emp002, salary, mid_salary).unwrap() };

            auto t7 { qs.insert_triple(0, emp003, has_name, hal).unwrap() };
            auto t8 { qs.insert_triple(0, emp003, department, engineering).unwrap() };
            auto t9 { qs.insert_triple(0, emp003, salary, low_salary).unwrap() };



            AND_WHEN("I query by subject only.") {

                auto triples1 {
                    qs.query(QueryFilter{
                        .namespace_id=0,
                        .subject_id=t1.sub_id,
                        .predicate_id=std::nullopt,
                        .object_id=std::nullopt
                }).unwrap()};

                THEN("I should get back the correct triples.") {

                    REQUIRE(std::find(triples1.begin(), triples1.end(), t1) != triples1.end());
                }
            }
        }
    }
}