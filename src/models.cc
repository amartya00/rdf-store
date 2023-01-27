#include <rdf-store/models.hpp>

namespace rdfstore {

    namespace models {

        bool operator==(const LiteralNode& left, const LiteralNode& right) {
            return left.datatype_iri_id == right.datatype_iri_id && left.bytes == right.bytes;
        }

        bool operator==(const IRINode& left, const IRINode& right) {
            return left.iri == right.iri;
        }

        bool operator==(const BlankNode& _left, const BlankNode& _right) {
            (void)_left;
            (void)_right;
            return true;
        }

        bool operator==(const Node& left, const Node& right) {
            return left.type == right.type && left.term == right.term;
        }

        bool operator==(const Predicate& left, const Predicate& right) {
            return left.iri == right.iri;
        }

    }
}