# cpp_match

Bringing a little bit of Rust to C++

I suffered from a missing `break;` a few days ago... my initial solution was to create a loop that uses lambdas to delay
the execution of a particular check. The same principal applied to the function; use a lambda.

```c++
#include <iostream>
#include <functional>
#include <random>

#define CASE(statement) [&]() { return (statement); }
#define DEFAULT true

template<typename F>
struct s_MatchCase {
    std::function<bool()> condition;
    std::function<F()> function;
};

template<typename F, size_t N>
F match(const s_MatchCase<F> (&match_case)[N]) {
    for (const auto &[cond, func]: match_case) {
        if (cond()) {
            return func();
        }
    }
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    int x = std::uniform_int_distribution<>(0, 3)(gen);
    std::cout << "X: " << x << std::endl;

    auto r = match<int>({{CASE(x < 1),   []() { return 0; }},
                         {CASE(x == 1),  []() { return 1; }},
                         {CASE(DEFAULT), []() { return 999; }}});
    std::cout << "M: " << r << std::endl;

    return 0;
}
```

While this is an elegant solution, it is not at all optimal in performance terms. Ideally, a match statement would
simply
generate an `if`/`switch` statement which the compiler can optimize as if it were built into the language.

This brought me onto macros, then external preprocessors such as the

- [Boost Preprocessor]("https://www.boost.org/doc/libs/1_84_0/libs/preprocessor/doc/index.html")
- [m4 Text Preprocessor]("https://www.gnu.org/software/m4/manual/m4.html")
- [Metalang99 Preprocessor]("https://github.com/Hirrolot/metalang99")

I settled for the ML99 preprocessor.

**Note:**
The following examples use a random number between 0 and 3 inclusive.

### `MATCH` macro

```c
#define MATCH_CASE_0_IMPL(statement) v(statement)
#define MATCH_CASE_1_IMPL(statement) v(else statement)

#define MATCH_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_STATEMENT_ARITY 1

#define MATCH_CASE_IMPL(statement, i) ML99_boolMatchWithArgs(ML99_natNeq(v(0), v(i)), v(MATCH_CASE_), v(statement))
#define MATCH_CASE_ARITY 2

#define MATCH(args...) ML99_LIST_EVAL(ML99_listMapI(v(MATCH_CASE), ML99_listFromTuples(v(MATCH_STATEMENT), v(args))))
```

Usage:

```c
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
```

Generates:

```c
if (x == 0) {std::cout << "x = 0!" << std::endl;} else if (x == 1) {std::cout << "x = 1!" << std::endl;} else if (x == 2) {std::cout << "x = 2!" << std::endl;} else if (x == 3) {{ x++; std::cout << "x = 4!" << std::endl; }}
```

### `MATCH_TO` macro

```c
#define MATCH_TO_STATEMENT_IMPL(cond, then) v(if (cond) {then})
#define MATCH_TO_STATEMENT_ARITY 1

#define MATCH_TO_CASE_IMPL(statement) v(statement)
#define MATCH_TO_CASE_ARITY 1

#define MATCH_TO_MISSING_RETURN std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1);

#define MATCH_TO(args...) [&]() { \
    ML99_LIST_EVAL(ML99_listMap(v(MATCH_TO_CASE), ML99_listFromTuples(v(MATCH_TO_STATEMENT), v(args))))                \
    MATCH_TO_MISSING_RETURN                                                                                            \
}();
```

Usage:

```c++
auto r = MATCH_TO( // Requires C++ 11 as it uses a lambda function.
        (x == 0, return 0.0;),
        (x == 1, return 1.0;),
        (x == 2, return 2.0;),
        (x == 3, {
            x++;
            return 3.0;
        })
)
assert(x == 0 | x == 1 | x == 2);
```

Generates:
```c++
auto r = [&]() { if (x == 0) {return 0.0;} if (x == 1) {return 1.0;} if (x == 2) {return 2.0;} if (x == 3) {{ x--; return 3.0; }} std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1); }();
```

#### Seeing the output (using cmake)

```
# Change directory/file names where relevant
mkdir build && cd build
cmake ..
make main.cpp.i && mv CMakeFiles/cpp_match.dir/main.cpp.o main.cpp.i
# You can find the relevant functions at the bottom of this massive file.
```