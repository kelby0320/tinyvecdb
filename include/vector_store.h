#pragma once

#include <cstdint>
#include <memory>
#include <span>

namespace tinyvecdb {

using VecId = std::uint64_t; // Move to VectorStore?
using RowId = std::uint64_t;

class IVectorStore {
  public:
    virtual ~IVectorStore() = default;

    virtual VecId insert(std::span<const float> vec,
                         std::span<const std::byte> metadata) = 0;
    virtual void remove(VecId id) = 0;
    virtual bool exists(VecId id) = 0;

  protected:
    std::uint32_t dim_;
};

std::unique_ptr<IVectorStore> create_vector_store(std::uint32_t dim);

} // namespace tinyvecdb