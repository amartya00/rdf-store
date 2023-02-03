#pragma once

#include <thesoup/types/types.hpp>
#include <rdf-store/models.hpp>

/**
 * \namespace rdfstore
 *
 * \brief The root namespace.
 * */
namespace rdfstore {

    /**
     * \namespace rdfstore::utils
     *
     * \brief Sub namespace with utilities.
     * */
    namespace utils {
        enum class ErrorCode {
            INVALID_DESERIALIZATION_REQUEST,
            INVALID_LITERAL
        };

        template <typename T> std::size_t get_literal_datatype_iri_id();

        template <typename T, typename=void> struct LiteralGenerator {
            static rdfstore::models::LiteralTerm serialize(const T& term) noexcept;
            static thesoup::types::Result<T, ErrorCode> deserialize(const rdfstore::models::LiteralTerm& term) noexcept;
        };

        template <typename T>
        struct LiteralGenerator<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
            static rdfstore::models::LiteralTerm serialize(const T& term) {
                constexpr std::size_t num_bytes {sizeof(T)};
                std::vector<std::byte> bytes {num_bytes};

                const std::byte* start {reinterpret_cast<const std::byte*>(&term)};
                const std::byte* end {start + num_bytes};
                if (std::endian::native == std::endian::little) {
                    // Little endian: The MSB will be at the highest address
                    for (auto it = start; it < end; it++) {
                        bytes.push_back(*it);
                    }
                } else {
                    // Big endian: MSB will be at lowest address
                    for (auto it = end-1; it >= start; it--) {
                        bytes.push_back(*it);
                    }
                }

                return rdfstore::models::LiteralTerm {
                        .bytes = bytes,
                        .datatype_iri_id = get_literal_datatype_iri_id<T>()
                };
            }

            static thesoup::types::Result<T, ErrorCode> deserialize(const rdfstore::models::LiteralTerm& term) noexcept {
                std::size_t datatype_iri_id {get_literal_datatype_iri_id<T>()};
                if (datatype_iri_id != term.datatype_iri_id) {
                    return thesoup::types::Result<T, ErrorCode>::failure(ErrorCode::INVALID_DESERIALIZATION_REQUEST);
                } else if (sizeof (T) != term.bytes.size()) {
                    return thesoup::types::Result<T, ErrorCode>::failure(ErrorCode::INVALID_LITERAL);
                } else {
                    const T* it {reinterpret_cast<const T*>(&term.bytes[0])};
                    return thesoup::types::Result<T, ErrorCode>::success(*it);
                }
            }
        };
    }
}