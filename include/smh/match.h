#ifndef SMH_MATCH_H
#define SMH_MATCH_H

#include <metalang99.h>

#ifndef DEFAULT
#define DEFAULT true
#endif // DEFAULT

#define MATCH_CASE_0_IMPL(statement) v(statement)
#define MATCH_CASE_1_IMPL(statement) v(else statement)

#define MATCH_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_STATEMENT_ARITY 1

#define MATCH_CASE_IMPL(statement, i) ML99_boolMatchWithArgs(ML99_natNeq(v(0), v(i)), v(MATCH_CASE_), v(statement))
#define MATCH_CASE_ARITY 2

#define MATCH(args...) ML99_LIST_EVAL(ML99_listMapI(v(MATCH_CASE), ML99_listFromTuples(v(MATCH_STATEMENT), v(args))))

#endif // SMH_MATCH_H