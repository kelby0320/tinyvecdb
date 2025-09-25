#pragma once

#include <cstdint>
#include <span>

namespace tinyvecdb {

using VectorId = uint64_t;
using CollectionId = uint32_t;
using NamespaceId = uint32_t;
using Dimension = uint32_t;
using VectorData = std::span<const float>;

enum class MetricType { Cosine, L2, Dot };

} // namespace tinyvecdb