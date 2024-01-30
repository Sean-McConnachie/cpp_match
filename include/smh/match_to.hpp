#ifndef MATCH_TO_H
#define MATCH_TO_H

#ifndef DEFAULT
#define DEFAULT true
#endif // DEFAULT

#define MATCH_TO_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_TO_STATEMENT_ARITY 1

#define MATCH_TO_CASE_IMPL(statement) v(statement)
#define MATCH_TO_CASE_ARITY 1

#define MATCH_TO_MISSING_RETURN std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1);

#define MATCH_TO(args...) [&]() { \
    ML99_LIST_EVAL(ML99_listMap(v(MATCH_TO_CASE), ML99_listFromTuples(v(MATCH_TO_STATEMENT), v(args))))                \
    MATCH_TO_MISSING_RETURN                                                                                            \
}();

#endif // MATCH_TO_H