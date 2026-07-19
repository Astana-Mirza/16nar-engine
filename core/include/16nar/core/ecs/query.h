/// @file
/// @brief File with Query class definitions.
#ifndef _16NAR_CORE_ECS_QUERY_H
#define _16NAR_CORE_ECS_QUERY_H

#include <16nar/core/ecs/defs.h>

#include <16nar/platform/strings/static_name.h>
#include <16nar/core/ecs/ecs_storage.h>

namespace _16nar::ecs
{

/// @brief Query into ECS storage.
/// @details Queries may check existence of components and get components data.
/// 
class NARENGINE_CORE_API Query
{
public:
     class Result;

     /// @brief Iterator in query results.
     class NARENGINE_CORE_API Iterator
     {
     public:
          /// @brief Constructor.
          /// @param[in] result result of the query.
          explicit Iterator( Result *result = nullptr ) noexcept;

          /// @brief Dereference operator.
          /// @return result of the query.
          Result& operator*() const noexcept;

          /// @brief Pre-increment operator.
          /// @return current object.
          Iterator& operator++() noexcept;

          /// @brief Equality comparison operator.
          /// @param[in] other right operand.
          /// @return true if operands are equal, false otherwise.
          bool operator==( const Iterator& other ) const noexcept;

          /// @brief Inequality comparison operator.
          /// @param[in] other right operand.
          /// @return true if operands are not equal, false otherwise.
          bool operator!=( const Iterator& other ) const noexcept;

     private:
          Result *result_;    ///< result of the query.
     };

     /// @brief Proxy class to get result of the query.
     /// @details It may be used as Java-like iterator, but in that case
     /// it should not be mixed with @b Iterator which is a regular C++ iterator.
     class NARENGINE_CORE_API Result
     {
     public:
          friend class Query;

          /// @brief Constructor.
          /// @param[in] query performed query.
          explicit Result( Query& query ) noexcept;

          /// @brief Get iterator of the query result.
          /// @return iterator of the query result.
          Iterator begin() noexcept;

          /// @brief Get iterator of the query result end.
          /// @return iterator of the query result end.
          Iterator end() noexcept;

          /// @brief Get constant component data from query result.
          /// @tparam T type of component.
          /// @param[in] index index of query element.
          /// @return constant data of the component from query result if it exists, nullptr otherwise.
          template < typename T >
          const T *get_component( std::size_t index ) const noexcept
          {
               return reinterpret_cast< const T * >( get_component_impl( index ) );
          }

          /// @brief Get component data from query result.
          /// @tparam T type of component.
          /// @param[in] index index of query element.
          /// @return data of the component from query result if it exists, nullptr otherwise.
          template < typename T >
          T *get_component( std::size_t index ) noexcept
          {
               return reinterpret_cast< T * >( const_cast< std::byte * >( get_component_impl( index ) ) );
          }

          /// @brief Check if component exists in query result.
          /// @param[in] index index of query element.
          /// @return true if the component exists, false otherwise.
          bool has_component( std::size_t index ) const noexcept;

          /// @brief Get identifier of current matched entity.
          /// @return identifier of current matched entitiy if any, empty identifier otherwise.
          EntityId get_entity_id() const noexcept;

          /// @brief Advance query to the next result.
          /// @return iterator of the query result.
          Iterator next() noexcept;

     private:
          /// @brief Reset the result to its default state.
          void reset() noexcept;

          /// @brief Get raw component data from query result.
          /// @param[in] index index of query element.
          /// @return raw data of the component from query result if it exists, nullptr otherwise.
          const std::byte *get_component_impl( std::size_t index ) const noexcept;

     private:
          Query& query_;                ///< executed query.
          std::uint64_t page_block_;    ///< second-order bitmasks of the query.
          BlockMask page_;              ///< bitmasks of the query.
          EcsId block_index_;           ///< index of current page block.
          EcsId page_index_;            ///< index of current page.
          EcsId local_index_;           ///< index of current entity on the page.
     };

     /// @brief Constructor.
     /// @param[in] storage queried ECS storage.
     /// @param[in] resource memory resource for query elements allocations.
     Query( EcsStorage& storage, std::pmr::memory_resource& resource );

     /// @brief Add condition requiring component presence.
     /// @param[in] type name of component type.
     /// @return current object.
     Query& has( strings::StaticName type );

     /// @brief Add condition requiring component absence.
     /// @param[in] type name of component type.
     /// @return current object.
     Query& has_not( strings::StaticName type );

     /// @brief Add condition requiring component data.
     /// @param[in] type name of component type.
     /// @param[out] index index of current query element which can be used to retrieve data.
     /// @return current object.
     Query& get( strings::StaticName type, std::size_t& index );

     /// @brief Add condition not requiring component data but extracting it, if any.
     /// @param[in] type name of component type.
     /// @param[out] index index of current query element which can be used to retrieve data.
     /// @return current object.
     Query& get_optional( strings::StaticName type, std::size_t& index );

     /// @brief Add condition not requiring component but checking if it exists.
     /// @param[in] type name of component type.
     /// @param[out] index index of current query element which can be used to check for component presence.
     /// @return current object.
     Query& has_optional( strings::StaticName type, std::size_t& index );

     /// @brief Check if the query is valid.
     /// @return true if the query is valid, false otherwise.
     bool valid() const noexcept;

     /// @brief Check if the query finished its execution.
     /// @return true if the query finished its execution, false otherwise.
     bool finished() const noexcept;

     /// @brief Perform the query from the beginning.
     /// @return result of the query.
     Result& execute() noexcept;

     /// @brief Get result of the query.
     /// @return result of the query.
     Result& get_result() noexcept;

     /// @brief Get constant result of the query.
     /// @return constant result of the query.
     const Result& get_result() const noexcept;

     /// @brief Reset the query to its default state.
     void reset();

private:
     /// @brief One element of a ECS query.
     struct QueryElement
     {
          /// @brief Flags of the query which modify its behavior.
          enum Flags : std::uint8_t
          {
               SearchModifier = 1 << 0, ///< component must be checked in the query.
               Wanted         = 1 << 1, ///< during check, component must exist.
               DataWanted     = 1 << 2  ///< component data must be extracted.
          };

          ComponentDescription *desc;   ///< component type description.
          std::byte *data;              ///< data of current component, may be nullptr.
          std::uint8_t flags;           ///< flags of the query element.
          bool found;                   ///< true if the component is found, false otherwise.
     };

     /// @brief Get component description.
     /// @param[in] type name of component type.
     /// @return component description.
     ComponentDescription *get_description( strings::StaticName type );

     /// @brief Advance query to the next result.
     void advance() noexcept;

     /// @brief Fill result data with found entity's components.
     void fill_result();

     /// @brief Get identifier of current matched entity.
     /// @return identifier of current matched entitiy if any, empty identifier otherwise.
     EntityId get_entity_id() const noexcept;

private:
     std::pmr::vector< QueryElement > elements_;  ///< elements of the query.
     Result result_;                              ///< state of the query result.
     EcsStorage& storage_;                        ///< queried ECS storage.
     bool valid_;                                 ///< validity of the query.
     bool finished_;                              ///< flag of query finish.
};

} // namespace _16nar::ecs

#endif // #ifndef _16NAR_CORE_ECS_QUERY_H
