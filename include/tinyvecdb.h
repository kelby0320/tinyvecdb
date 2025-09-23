#pragma once

#include "vector_store.h"

#include <cstdint>
#include <memory>
#include <span>
#include <string>

namespace tinyvecdb {

struct QueryResult;
struct Filter;

struct TinyVecDbConfig {
    // TODO
};

class TinyVecDb {
  public:
    static std::unique_ptr<TinyVecDb> open(const TinyVecDbConfig &config);

    bool create_collection(std::string collection);
    bool drop_collection(std::string collection);

    VecId insert(std::string collection, std::span<const float> vec,
                 std::span<const std::byte> metadata = {});
    void remove(std::string collection, VecId id);
    QueryResult query(std::string collection, std::span<const float> vec,
                      const Filter &filter);

    ~TinyVecDb();
    TinyVecDb(TinyVecDb &&) noexcept;
    TinyVecDb &operator=(TinyVecDb &&) noexcept;

    TinyVecDb(const TinyVecDb &) = delete;
    TinyVecDb &operator=(const TinyVecDb &) = delete;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;

    explicit TinyVecDb(std::unique_ptr<Impl> impl);
};

} // namespace tinyvecdb