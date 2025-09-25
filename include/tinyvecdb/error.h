#pragma once

#include <string>

namespace tinyvecdb {

enum class ErrorCode { Ok, Unknown };

struct Error {
    ErrorCode code;
    std::string message;
};

} // namespace tinyvecdb