//
// Created by user on 7/10/25.
//

#ifndef JST_TYPE_H
#define JST_TYPE_H
#include <cstdint>
#include <string>

namespace jstreamer {
using jst_bool = bool;
using jst_int = int;
using jst_int64 = int64_t;
using jst_string = std::string;
using jst_float = float;
using jst_double = double;
using jst_range = std::pair<jst_double, jst_double>; // [min, max]
}

#endif //JST_TYPE_H
