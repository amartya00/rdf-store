#define CATCH_CONFIG_MAIN

#include <set>
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

using GenStringLiteral = rdfstore::utils::LiteralGenerator<std::string>;
using GenLongLiteral = rdfstore::utils::LiteralGenerator<unsigned long>;

template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<unsigned long>() {
    return 2;
}

template <>
std::size_t rdfstore::utils::get_literal_datatype_iri_id<std::string>() {
    return 3;
}


SCENARIO("RDF store insertions.") {

    GIVEN("I have a red store and some triples.") {
        QuadStore qs;

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

        WHEN("I insert some elements into it.") {

            auto t1 { qs.insert_triple(0, emp001, has_name, amartya).unwrap() };
            auto t2 { qs.insert_triple(0, emp001, department, engineering).unwrap() };
            auto t3 { qs.insert_triple(0, emp001, salary, high_salary).unwrap() };

            auto t4 { qs.insert_triple(0, emp002, has_name, bob).unwrap() };
            auto t5 { qs.insert_triple(0, emp002, department, sales).unwrap() };
            auto t6 { qs.insert_triple(0, emp002, salary, mid_salary).unwrap() };

            auto t7 { qs.insert_triple(0, emp003, has_name, hal).unwrap() };
            auto t8 { qs.insert_triple(0, emp003, department, engineering).unwrap() };
            auto t9 { qs.insert_triple(0, emp003, salary, low_salary).unwrap() };

            auto t10 { qs.insert_triple(0, sales, has_population, sales_population).unwrap() };

            THEN("The returned triples should have consistent node IDs and edge IDs.") {

                REQUIRE(t1.sub_id == t2.sub_id);
                REQUIRE(t2.sub_id == t3.sub_id);

                REQUIRE(t4.sub_id == t5.sub_id);
                REQUIRE(t5.sub_id == t6.sub_id);

                REQUIRE(t7.sub_id == t8.sub_id);
                REQUIRE(t8.sub_id == t9.sub_id);

                REQUIRE_FALSE(t1.sub_id == t4.sub_id);
                REQUIRE_FALSE(t1.sub_id == t7.sub_id);
                REQUIRE_FALSE(t4.sub_id == t7.sub_id);

                REQUIRE(t1.pred_id == t4.pred_id);
                REQUIRE(t4.pred_id == t7.pred_id);

                REQUIRE(t2.pred_id == t5.pred_id);
                REQUIRE(t5.pred_id == t8.pred_id);

                REQUIRE(t3.pred_id == t6.pred_id);
                REQUIRE(t6.pred_id == t9.pred_id);

                REQUIRE_FALSE(t1.pred_id == t2.pred_id);
                REQUIRE_FALSE(t2.pred_id == t3.pred_id);
                REQUIRE_FALSE(t3.pred_id == t1.pred_id);

                REQUIRE(t5.obj_id == t10.sub_id); // Sales for both

                std::set<std::size_t> object_ids {
                    t1.obj_id,
                    t2.obj_id,
                    t3.obj_id,
                    t4.obj_id,
                    t5.obj_id,
                    t6.obj_id,
                    t7.obj_id,
                    t8.obj_id,
                    t9.obj_id,
                    t10.obj_id};

                REQUIRE(9 == object_ids.size()); // 3 names + 2 departments + 3 salaries + 1 population
            }
        }
    }
}
