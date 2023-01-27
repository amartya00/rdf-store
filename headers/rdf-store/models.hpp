#ifndef __RDFSTORE_MODELS__
#define __RDFSTORE_MODELS__

#include <vector>
#include <variant>
#include <optional>

#include <thesoup/types/types.hpp>

/**
 * \namespace rdfstore
 *
 * \brief The root namespace.
 * */
namespace rdfstore {

    /**
     * \namespace rdfstore::models
     *
     * \brief Sub namespace with all models related to the RDF store.
     * */
    namespace models {
        /**
         * \brief A literal node
         *
         * A structure representing a literal RDF node. The description can be found here: https://www.w3.org/TR/2014/REC-rdf11-concepts-20140225/#section-Graph-Literal
         * According to the documentation, it has 2 components:
         *   - A lexical form. This is supposed to be a unicode string, but internally, we will just use a byte vector.
         *     That allows some storage optimizations.
         *   - A datatype IRI. This is an official IRI pointing to the details of the datatype. Example: http://www.w3.org/2001/XMLSchema#string.
         *     However, internally we will use an identifier (std::size_t) to represent the datatype. The ID -> datatype IRI
         *     mapping will me maintained elsewhere. If you are using this struct in isolation, please make sure to create a
         *     datatype -> ID mapping.
         */
        struct LiteralNode {
            std::vector<std::byte> bytes; /**< The byte vector containing the raw data. */
            std::size_t datatype_iri_id; /**< The ID of the datatype IRI. */
        };

        /**
         * \brief An IRI node
         *
         * A structure representing an IRI node. The description can be found here: https://www.w3.org/TR/2014/REC-rdf11-concepts-20140225/#section-IRIs .
         * As per the requirements, this node only needs a IRI parameter that is a unicode string. We will yse a plain
         * std::string to store it and implement the conversions and equality operations elsewhere.
         *
         * If you are using this structure in isolation, please remember to implement equality operations and unicode -> ASCII
         * conversions.
         *
         */
        struct IRINode {
            std::string iri; /**< The ASCII encoded IRI. */
        };

        /**
         * \brief A blank node
         *
         * This structure represents a blank node. Detailed description here: https://www.w3.org/TR/2014/REC-rdf11-concepts-20140225/#section-blank-nodes
         */
        struct BlankNode {};

        /**
         * \brief The enum representing a node type.
         *
         * This enum represents a node type. This will used in the rdfstore::models::Node class in conjunction with a
         * variant to save the appropriate data.
         */
        enum class NodeType {
            IRI,
            LITERAL,
            BLANK
        };

        /**
         * \brief A RDF node class
         *
         * A RDF node is a component of a RDF graph. For a description of what it means, please see the specification here:
         * https://www.w3.org/TR/2014/REC-rdf11-concepts-20140225/#section-triples .
         */
        struct Node {
            NodeType type;
            std::variant<BlankNode, LiteralNode, IRINode> term; /**< The data; can be blank, IRI or literal. */

        };

        /**
         * \brief A predicate (edge) type.
         *
         * This structure represents a predicate type. In a RDF graph, each "triple" looks like (subject) ---[predicate]---> (object).
         * The predicate is essentially like an edge, connecting 2 "nodes": a subject and an object.
         *
         * As per the specification here: https://www.w3.org/TR/2014/REC-rdf11-concepts-20140225/#section-triples , a predicate
         * can only be an IRI type, so that is the only member here.
         */
        struct Predicate {
           IRINode iri;
        };

        //!\cond NO_DOC

        //!\endcond


        /**
         * \brief A triple
         *
         * This structure represents a triple. In our implementation, namespaces, nodes and edges are registered as IDs
         * and then the graph is composed of an adjacency list of those IDs. Hence this struct just has 4 IDs (std::size_t).
         * The decoupling of the node/edge data and the graph allows for increased efficiency during parse operations.
         */
        struct Triple {
            std::size_t namespace_id; /**< Namespace ID. */
            std::size_t sub_id; /**< Subject ID. */
            std::size_t pred_id; /**< Predicate ID. */
            std::size_t obj_id; /**< Object ID. */
        };

        /**
         * \brief A filter struct for  querying the graph.
         *
         * This struct is used to filter triples in the RDF graph. We optionally pass the subject, predicate and object,
         * and the filtering function will filter based on how much of the triple is provided. Note that the namespace
         * has to be provided.
         */
        struct QueryFilter {
            std::size_t namespace_id;
            std::optional<std::size_t> subject_id;
            std::optional<std::size_t> predicate_id;
            std::optional<std::size_t> object_id;
        };

        /**
         * \brief ErrorCode enum representing errors that can occur in the store.
         *
         * This enum represents all possible errors that can happen in the rdf store.
         */
        enum ErrorCode {
            NON_EXISTENT_SUBJECT,
            NON_EXISTENT_PREDICATE,
            NON_EXISTENT_OBJECT,
            UNKNOWN_ERROR,

            FAILURE_TO_INSERT_SUBJ,
            FAILURE_TO_INSERT_PRED,
            FAILURE_TO_INSERT_OBJECT,
            FAILURE_TO_INSERT_TRIPLE,
            UNDER_SPECIFIED_QUERY_FILTER,

            TRIPLE_NOT_FOUND,
            SUBJECT_NOT_FOUND,
            OBJECT_NOT_FOUND
        };

        /**
         * \brief An error struct
         *
         * This struct is used to return an error code with some underlying error information. The underlying error is
         * optional as errors might not always have bubbled up from underneath.
         *
         * @tparam T The underlying error type.
         */
        template <typename T> struct Error {
            ErrorCode code;
            std::optional<T> underlying_error;
        };

        //!\cond NO_DOC
        bool operator==(const LiteralNode& left, const LiteralNode& right);
        bool operator==(const IRINode& left, const IRINode& right);
        bool operator==(const BlankNode& _left, const BlankNode& _right);
        bool operator==(const Node& left, const Node& right);
        bool operator==(const Predicate& left, const Predicate& right);
        //!\endcond
    }
}

//!\cond NO_DOC
namespace std {
    template<> struct hash<rdfstore::models::Node> {
        std::size_t operator()(const rdfstore::models::Node& node) {
            if (node.type == rdfstore::models::NodeType::IRI) {
                const rdfstore::models::IRINode& iri{std::get<rdfstore::models::IRINode>(node.term)};
                const auto hash_value{std::hash<std::string>()(iri.iri)};
                return hash_value == 0 ? 1 : hash_value;
            } else if (node.type == rdfstore::models::NodeType::LITERAL) {
                const rdfstore::models::LiteralNode& literal {std::get<rdfstore::models::LiteralNode>(node.term)};
                std::size_t acc_hash {std::hash<std::byte>()(literal.bytes[0])};
                for (std::size_t i = 1; i < literal.bytes.size(); i++) {
                    std::size_t tmp_hash {std::hash<std::byte>()(literal.bytes[i])};
                    acc_hash = acc_hash ^ (tmp_hash + (acc_hash<<6) + (acc_hash>>2) + 0x9e3779b9);
                }
                return acc_hash;
            } else {
                return 0;
            }
        }
    };
}

//!\endcond

#endif