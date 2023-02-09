#pragma once

#include <bit>
#include <iostream>

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
        struct LiteralGenerator<T, typename std::enable_if<std::is_scalar<T>::value>::type> {
            static_assert(std::endian::native == std::endian::little, "This will not work on Big Endian architecture.");
            static rdfstore::models::LiteralTerm serialize(const T& term) {
                constexpr std::size_t num_bytes {sizeof(T)};
                std::vector<std::byte> bytes {};
                bytes.reserve(num_bytes);

                const std::byte* start {reinterpret_cast<const std::byte*>(&term)};
                const std::byte* end {start + num_bytes};

                // Little endian: The MSB will be at the highest address
                for (auto it = start; it < end; it++) {
                    bytes.push_back(*it);
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

        template <typename T>
        struct LiteralGenerator<T, typename std::enable_if<std::is_same<T, std::string>::value>::type> {
            static rdfstore::models::LiteralTerm serialize(const std::string& term) noexcept  {
                const std::byte* start {reinterpret_cast<const std::byte*>(&term[0])};
                const std::byte* end {start + term.size()};

                std::vector<std::byte> bytes;
                bytes.reserve(term.size());
                std::copy(start, end, std::back_inserter(bytes));

                return rdfstore::models::LiteralTerm {
                        .bytes = bytes,
                        .datatype_iri_id = get_literal_datatype_iri_id<std::string>()
                };
            }

            static thesoup::types::Result<std::string, ErrorCode> deserialize(const rdfstore::models::LiteralTerm& term) noexcept {
                std::size_t datatype_iri_id {get_literal_datatype_iri_id<std::string>()};
                if (datatype_iri_id != term.datatype_iri_id) {
                    return thesoup::types::Result<std::string, ErrorCode>::failure(ErrorCode::INVALID_DESERIALIZATION_REQUEST);
                } else if (term.bytes.size() == 0) {
                    return thesoup::types::Result<std::string, ErrorCode>::failure(ErrorCode::INVALID_LITERAL);
                } else {
                    std::stringstream ss;
                    for (const auto& item: term.bytes) {
                        ss << static_cast<char>(item);
                    }
                    return thesoup::types::Result<std::string, ErrorCode>::success(ss.str());
                }
            }
        };

    }
}