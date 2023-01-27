#include <optional>

#include <thesoup/types/types.hpp>
#include <thesoup/types/graph.hpp>
#include <rdf-store/quadstore.hpp>

using thesoup::types::Result;
using thesoup::types::Neighbour;
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
    auto register_edge_res {graph.register_edge_type(pred)};

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

    if (insert_edge_res) {
        return Result<Triple, Error<UnderlyingErrorCode>>::success(
                Triple {
                    namespace_id,
                    insert_sub_res.unwrap(),
                    register_edge_res.unwrap(),
                    insert_obj_res.unwrap()
                });
    } else {
        return Result<Triple, Error<UnderlyingErrorCode>>::failure({ErrorCode::FAILURE_TO_INSERT_TRIPLE, insert_edge_res.error()});
    }
}


Result<std::vector<Triple>, Error<UnderlyingErrorCode>> QuadStore::query(const QueryFilter &filter) noexcept {
    if (filter.subject_id == std::nullopt && filter.object_id == std::nullopt) {
        // (NULL, _, NULL)
        return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::UNDER_SPECIFIED_QUERY_FILTER, std::nullopt});
    } else if (filter.subject_id != std::nullopt && filter.predicate_id != std::nullopt && filter.object_id != std::nullopt) {
        // (sub, pred, obj)
        auto neighbours_res {graph.get_neighbours(*filter.subject_id, *filter.predicate_id)};
        if (!neighbours_res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure(
                    {ErrorCode::TRIPLE_NOT_FOUND, neighbours_res.error()});
        } else {
            const std::vector<std::size_t>& neighbours {neighbours_res.unwrap()};
            auto it {std::find(neighbours.begin(), neighbours.end(), *filter.object_id)};
            if (it == neighbours.end()) {
                return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure(
                        {ErrorCode::TRIPLE_NOT_FOUND, std::nullopt});
            } else {
                return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(
                        {{filter.namespace_id, *filter.subject_id, *filter.predicate_id, *filter.object_id}});
            }
        }
    } else if(filter.subject_id != std::nullopt && filter.predicate_id == std::nullopt && filter.object_id == std::nullopt) {
        // (subj, NULL, NULL)
        auto neighbours_res {graph.get_neighbours(*filter.subject_id)};
        if (!neighbours_res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure(
                    {ErrorCode::SUBJECT_NOT_FOUND, neighbours_res.error()});
        } else {
            std::size_t num_neighbours {neighbours_res.unwrap().size()};
            std::vector<Triple> triples {};
            if (num_neighbours > 0) {
                triples.reserve(num_neighbours);
                for (const auto& item : neighbours_res.unwrap()) {
                    triples.push_back({filter.namespace_id, *filter.subject_id, item.edge, item.vertex});
                }
            }
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(triples);
        }
    } else if (filter.subject_id != std::nullopt && filter.predicate_id != std::nullopt) {
        // (sub, pred, NULL)
        auto neighbours_res {graph.get_neighbours(*filter.subject_id, *filter.predicate_id)};
        if (!neighbours_res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::SUBJECT_NOT_FOUND, neighbours_res.error()});
        } else {
            std::vector<Triple> triples;
            if (neighbours_res.unwrap().size() > 0) {
                triples.reserve(neighbours_res.unwrap().size());
                for (const auto& item : neighbours_res.unwrap()) {
                    triples.push_back({filter.namespace_id, *filter.subject_id, *filter.predicate_id, item});
                }
            }
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(triples);
        }
    } else if(filter.predicate_id != std::nullopt && filter.object_id != std::nullopt) {
        // (NULL, pred, obj)
        auto back_neighbours_res {graph.get_incoming_edges(*filter.object_id, *filter.predicate_id)};
        if (!back_neighbours_res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::OBJECT_NOT_FOUND, back_neighbours_res.error()});
        } else {
            std::vector<Triple> triples;
            if (back_neighbours_res.unwrap().size() > 0) {
                triples.reserve(back_neighbours_res.unwrap().size());
                for (const auto& item : back_neighbours_res.unwrap()) {
                    triples.push_back({filter.namespace_id, item, *filter.predicate_id, *filter.object_id});
                }
            }
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(triples);
        }
    } else if (filter.subject_id != std::nullopt && filter.object_id != std::nullopt) {
        // (sub, NULL, obj)
        auto res {graph.get_neighbours(*filter.subject_id)};
        if (!res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::SUBJECT_NOT_FOUND, res.error()});
        } else {
            std::vector<Triple> triples;
            for (const auto& item: res.unwrap()) {
                if (item.vertex == *filter.object_id) {
                    triples.push_back({filter.namespace_id, *filter.subject_id, item.edge, *filter.object_id});
                }
            }
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(triples);
        }
    } else {
        // (NULL, NULL, obj)
        auto back_neighbours_res {graph.get_incoming_edges(*filter.object_id)};
        if (!back_neighbours_res) {
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::failure({ErrorCode::OBJECT_NOT_FOUND, back_neighbours_res.error()});
        } else {
            std::vector<Triple> triples;
            if (back_neighbours_res.unwrap().size() > 0) {
                triples.reserve(back_neighbours_res.unwrap().size());
                for (const auto& item : back_neighbours_res.unwrap()) {
                    triples.push_back({filter.namespace_id, item.vertex, item.edge, *filter.object_id});
                }
            }
            return Result<std::vector<Triple>, Error<UnderlyingErrorCode>>::success(triples);
        }
    }
}
