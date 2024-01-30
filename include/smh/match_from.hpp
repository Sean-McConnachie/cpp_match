#ifndef MATCH_FROM_H
#define MATCH_FROM_H

#include <metalang99.h>

#ifndef DEFAULT
#define DEFAULT true
#endif // DEFAULT

#define MATCH_FROM_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_FROM_STATEMENT_ARITY 1

#define MATCH_FROM_CASE_IMPL(statement) v(statement)
#define MATCH_FROM_CASE_ARITY 1

#define MATCH_FROM_MISSING_RETURN std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1);

#define MATCH_FROM(args...) [&]() { \
    ML99_LIST_EVAL(ML99_listMap(v(MATCH_FROM_CASE), ML99_listFromTuples(v(MATCH_FROM_STATEMENT), v(args))))                \
    MATCH_FROM_MISSING_RETURN                                                                                            \
}();

#endif // MATCH_FROM_H