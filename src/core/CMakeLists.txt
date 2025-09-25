add_library(tinyvecdb_core STATIC
  tinyvecdb.cpp
)

target_include_directories(tinyvecdb_core
  PUBLIC
    ${TINYVECDB_PUBLIC_INCLUDE_DIR}
    ${TINYVECDB_GENERATED_INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(tinyvecdb_core PUBLIC cxx_std_20)

# target_link_libraries(tinyvecdb_core
#   PUBLIC
#     tinyvecdb_utils
#     tinyvecdb_kernels
#     tinyvecdb_vector_store
#     tinyvecdb_query_engine
#     tinyvecdb_index_registry
#     tinyvecdb_index_hnsw
#     tinyvecdb_persistence
# )

# Optional linkages
if(TINYVECDB_ENABLE_IVF)
  target_link_libraries(tinyvecdb_core PUBLIC tinyvecdb_index_ivf)
endif()
if(TINYVECDB_ENABLE_PQ)
  target_link_libraries(tinyvecdb_core PUBLIC tinyvecdb_index_pq)
endif()
