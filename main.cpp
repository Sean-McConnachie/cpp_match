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