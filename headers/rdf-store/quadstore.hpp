#ifndef __RDFSTORE_QUADSTORE__
#define __RDFSTORE_QUADSTORE__

#include <functional>
#include <vector>

#include <thesoup/types/indexedgraph.hpp>
#include <thesoup/types/types.hpp>
#include <rdf-store/models.hpp>

namespace rdfstore {

    namespace quadstore {

        class QuadStore {
        private:
            thesoup::types::IndexedPropertyDiGraph<rdfstore::models::Node, rdfstore::models::Predicate> graph {};

        public:
            thesoup::types::Result<rdfstore::models::Triple, rdfstore::models::Error<thesoup::types::IndexedPropertyDiGraphAttributes::ErrorCode>>
            insert_triple(
                    const std::size_t& namespace_id,
                    const rdfstore::models::Node& subj,
                    const rdfstore::models::Predicate& pred,
                    const rdfstore::models::Node& obj) noexcept;

            thesoup::types::Result<std::vector<rdfstore::models::Triple>, rdfstore::models::Error<thesoup::types::IndexedPropertyDiGraphAttributes::ErrorCode>>
            query(const rdfstore::models::QueryFilter& filter) noexcept;
        };
    }
}

#endif