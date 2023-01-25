#include <optional>

#include <thesoup/types/types.hpp>
#include <thesoup/types/graph.hpp>
#include <rdf-store/quadstore.hpp>

using thesoup::types::Result;
using UnderlyingErrorCode = thesoup::types::IndexedPropertyDiGraphAttributes::ErrorCode;
using rdfstore::models::Error;
using rdfstore::models::ErrorCode;
using rdfstore::quadstore::QuadStore;
using rdfstore::models::Triple;
using rdfstore::models::Node;
using rdfstore::models::Predicate;
using rdfstore::models::QueryFilter;

Result<Triple, Error<UnderlyingErrorCode>>
QuadStore::insert_triple(const std::size_t& namespace_id, const Node& subj, const Predicate& pred, const Node& obj) noexcept {
    auto insert_sub_res {graph.insert_vertex(subj)};
    auto insert_obj_res {graph.insert_vertex(obj)};
    auto edge_type_id {graph.register_edge_type(pred).unwrap()};
    if (!insert_sub_res) {
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_SUBJ, insert_obj_res.error()});
    } else if (!insert_obj_res) {
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_OBJECT, insert_obj_res.error()});
    } else if (!register_edge_res) {
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_PRED, register_edge_res.error()});
    }

    auto insert_edge_res{
        graph.insert_edge(
                {insert_sub_res.unwrap(), register_edge_res.unwrap(), insert_obj_res.unwrap()})
    };

    auto insert_backward_edge_res{
            graph.insert_edge(
                    {register_edge_res.unwrap(), insert_obj_res.unwrap(), insert_sub_res.unwrap()})
    };
    if (insert_edge_res && insert_backward_edge_res) {
        return Result<Triple, Error<UnderlyingErrorCode>>::success(
                Triple {
                    namespace_id,
                    insert_sub_res.unwrap(),
                    register_edge_res.unwrap(),
                    insert_obj_res.unwrap()
                });
    } else if (!insert_edge_res){
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_TRIPLE, insert_edge_res.error()});
    } else {
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_BACK_EDGE, insert_backward_edge_res.error()});
    }
}

Result<std::vector<Triple>, Error<UnderlyingErrorCode>> QuadStore::query(const QueryFilter &filter) noexcept {
    if (filter.subject_id == std::nullopt && filter.object_id == std::nullopt) {
        return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::UNDER_SPECIFIED_QUERY_FILTER, std::nullopt});
    } else {}
}

