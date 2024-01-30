#include <metalang99.h>
#include <iostream>
#include <random>
#include <cassert>

#define DEFAULT true

#define MATCH_CASE_0_IMPL(statement) v(statement)
#define MATCH_CASE_1_IMPL(statement) v(else statement)

#define MATCH_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_STATEMENT_ARITY 1

#define MATCH_CASE_IMPL(statement, i) ML99_boolMatchWithArgs(ML99_natNeq(v(0), v(i)), v(MATCH_CASE_), v(statement))
#define MATCH_CASE_ARITY 2

#define MATCH(args...) ML99_LIST_EVAL(ML99_listMapI(v(MATCH_CASE), ML99_listFromTuples(v(MATCH_STATEMENT), v(args))))


#define MATCH_TO_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_TO_STATEMENT_ARITY 1

#define MATCH_TO_CASE_IMPL(statement) v(statement)
#define MATCH_TO_CASE_ARITY 1

#define MATCH_TO_MISSING_RETURN std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1);

#define MATCH_TO(args...) [&]() { \
    ML99_LIST_EVAL(ML99_listMap(v(MATCH_TO_CASE), ML99_listFromTuples(v(MATCH_TO_STATEMENT), v(args))))                \
    MATCH_TO_MISSING_RETURN                                                                                            \
}();

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int x = std::uniform_int_distribution<>(0, 3)(gen);
    std::cout << "MATCH example;\t\tx: " << x << std::endl;

    MATCH(
            (x == 0, std::cout << "x = 0!" << std::endl;),
            (x == 1, std::cout << "x = 1!" << std::endl;),
            (x == 2, std::cout << "x = 2!" << std::endl;),
            (x == 3, {
                    x++;
                    std::cout << "x = 4!" << std::endl;
            })
    )

    assert(x == 0 | x == 1 | x == 2 | x == 4);
    x = std::uniform_int_distribution<>(0, 3)(gen);
    std::cout << "MATCH_TO example;\tx: " << x << std::endl;

    auto r = MATCH_TO( // Requires C++ 11 as it uses a lambda.
            (x == 0, return 0.0;),
            (x == 1, return 1.0;),
            (x == 2, return 2.0;),
            (x == 3, {
                    x--;
                    return 3.0;
            })
    )

    assert(x == 0 | x == 1 | x == 2);
    std::cout << "r = " << r << std::endl;
}