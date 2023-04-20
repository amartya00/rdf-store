#include <exception>
#include <variant>

#include <rdf-store/models.hpp>

namespace rdfstore {

    namespace models {

        template<>
        Node Node::init<LiteralTerm>(const LiteralTerm& term)  noexcept {
            return Node {
                .type = NodeType::LITERAL,
                .term = term
            };
        }

        template<>
        Node Node::init<IRITerm>(const IRITerm& term)  noexcept {
            return Node {
                    .type = NodeType::IRI,
                    .term = term
            };
        }

        template<>
        Node Node::init<BlankTerm>(const BlankTerm& term)  noexcept {
            return Node {
                    .type = NodeType::BLANK,
                    .term = term
            };
        }

        bool operator==(const LiteralTerm& left, const LiteralTerm& right) {
            return left.datatype_iri_id == right.datatype_iri_id && left.bytes == right.bytes;
        }

        bool operator==(const IRITerm& left, const IRITerm& right) {
            return left.iri == right.iri;
        }

        bool operator==(const BlankTerm& _left, const BlankTerm& _right) {
            (void)_left;
            (void)_right;
            return true;
        }

        bool operator==(const Node& left, const Node& right) {
            return left.type == right.type && left.type == right.type;
        }

        bool operator==(const Predicate& left, const Predicate& right) {
            return left.iri == right.iri;
        }

        bool operator==(const Triple& left, const Triple& right) {
            return left.namespace_id == right.namespace_id &&
                left.sub_id ==

        }
    }
}