#ifndef SMH_ENUM_DERIVE_H
#define SMH_ENUM_DERIVE_H

#include <metalang99.h>

#define ENUM_VARIANT_IMPL(variant) v(variant,)
#define ENUM_VARIANT_ARITY 1

#define ENUM_SWITCH_IMPL(name, variant) v(case name::variant: return #variant;)
#define ENUM_SWITCH_ARITY 2

#define ENUM_IF_IMPL(str, name, variant) v(if (str == #variant) { return name::variant; })
#define ENUM_IF_ARITY 3

#ifndef ENUM_INVALID_VARIANT
#define ENUM_INVALID_VARIANT(s) std::cerr << "Error: passed string: `" << #s << "` is an invalid variant." << std::endl; \
                                std::exit(1);
#endif // ENUM_INVALID_VARIANT

#define ENUM_DERIVE(name, args...)                                                      \
enum class name {                                                                       \
    ML99_EVAL(ML99_variadicsForEach(v(ENUM_VARIANT), v(args)))                          \
};                                                                                      \
inline std::string to_str(const name e) {                                               \
    switch (e) {                                                                        \
        ML99_EVAL(ML99_variadicsForEach(ML99_appl(v(ENUM_SWITCH), v(name)), v(args)))   \
        default: return "";                                                             \
    }                                                                                   \
}                                                                                       \
inline name from_str(const std::string &s) {                                            \
    ML99_EVAL(ML99_variadicsForEach(ML99_appl2(v(ENUM_IF), v(s), v(name)), v(args)))    \
    ENUM_INVALID_VARIANT(v(s))                                                          \
}

#include "../metalang99undef.h"

#endif // SMH_ENUM_DERIVE_H