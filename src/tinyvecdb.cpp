#include "tinyvecdb.h"

namespace tinyvecdb {

struct TinyVecDb::Impl {
    explicit Impl(const TinyVecDbConfig &config) {
        // TODO
    }

    Impl() = default;
    ~Impl() = default;

    Impl(const Impl &) = delete;
    Impl &operator=(const Impl &) = delete;

    Impl(Impl &&) noexcept = default;
    Impl &operator=(Impl &&) noexcept = default;
};

std::unique_ptr<TinyVecDb> TinyVecDb::open(const TinyVecDbConfig &config) {
    return std::unique_ptr<TinyVecDb>(
        new TinyVecDb(std::make_unique<Impl>(config)));
}

} // namespace tinyvecdb