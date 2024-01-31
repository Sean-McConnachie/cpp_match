#ifndef SMH_JSON_STRUCT_H
#define SMH_JSON_STRUCT_H

#include <metalang99.h>

#ifdef v // Should always be true since we just included ML99
#undef v
#endif // v

#include <nlohmann/json.hpp>

#define v(...) (0v, __VA_ARGS__)

using json = nlohmann::json;

#define JSON_STRUCT_UNWRAP_CALL_IMPL(call, tuple) ML99_call(call, ML99_untuple(v(tuple)))
#define JSON_STRUCT_UNWRAP_CALL_ARITY 2

#define JSON_STRUCT_VAR_DECL_IMPL(type, name) v(type name;)
#define JSON_STRUCT_FROM_JSON_IMPL(type, name) v(j.at(#name).get_to(s.name);)
#define JSON_STRUCT_TO_JSON_IMPL(type, name) v({#name, s.name},)

#define JSON_STRUCT(name, args...)                                                                                     \
struct name {                                                                                                          \
    ML99_EVAL(ML99_variadicsForEach(ML99_appl(v(JSON_STRUCT_UNWRAP_CALL), v(JSON_STRUCT_VAR_DECL)), v(args)))          \
};                                                                                                                     \
inline void from_json(const json &j, name &s) {                                                                        \
    ML99_EVAL(ML99_variadicsForEach(ML99_appl(v(JSON_STRUCT_UNWRAP_CALL), v(JSON_STRUCT_FROM_JSON)), v(args)))         \
}                                                                                                                      \
inline void to_json(json &j, const name &s) {                                                                          \
    j = {                                                                                                              \
        ML99_EVAL(ML99_variadicsForEach(ML99_appl(v(JSON_STRUCT_UNWRAP_CALL), v(JSON_STRUCT_TO_JSON)), v(args)))       \
    };                                                                                                                 \
}

#endif //SMH_JSON_STRUCT_H