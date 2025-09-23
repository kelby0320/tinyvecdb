#include "vector_store.h"

namespace tinyvecdb {

class VectorStore : public IVectorStore {
  public:
    VectorStore(std::uint32_t dim) { dim_ = dim; }

    VecId insert(std::span<const float> vec,
                 std::span<const std::byte> metadata) override {
        // Implementation of insert
        return 0; // Placeholder
    }

    void remove(VecId id) override {
        // Implementation of remove
    }

    bool exists(VecId id) override {
        // Implementation of exists
        return false; // Placeholder
    }
};

std::unique_ptr<IVectorStore> create_vector_store(std::uint32_t dim) {
    return std::make_unique<VectorStore>(dim);
}

} // namespace tinyvecdb