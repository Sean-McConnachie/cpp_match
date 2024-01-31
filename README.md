# smh

Bringing a little bit of Rust to C++.

## Motivation

Recently, I experienced a few stupid bugs in my code that could have been avoided if I had used Rust instead of C++.
These bugs were caused by the fact that I'm not a perfect programmer and I make mistakes. 🐛s:

- [match](https://doc.rust-lang.org/rust-by-example/flow_control/match.html) Falling through a switch statement and
  using `MJPG` for 3 weeks instead of `H264`.
- [derive](https://doc.rust-lang.org/reference/attributes/derive.html) Forgetting to update fields in json
  configuration structs (there are two functions and one field that must be modified).

***

## Table of Contents

- [Info](#info)
- [Install](#install)
- [Features](#features)
    - [ENUM_DERIVE](#enum_derive)
    - [JSON_STRUCT](#json_struct)
    - [MATCH](#match)
    - [MATCH_FROM](#match_from)
- [Seeing the output](#seeing-the-output-using-cmake)
- [TODO](#todo)

***

## Info

For generating code, this project uses the [Metalang99 Preprocessor](https://github.com/Hirrolot/metalang99).

As most of my projects use the [nlohmann::json](https://github.com/nlohmann/json) library, this is what
the `JSON_STRUCT` macro is designed for.
***

## Install

This project can currently be added to a `CMake` project using `FetchContent`.

```cmake
include(FetchContent)
FetchContent_Declare(
        smh
        GIT_REPOSITORY https://github.com/Sean-McConnachie/smh.git
        GIT_TAG main
)
FetchContent_MakeAvailable(smh)
target_link_libraries(${PROJECT_NAME} PRIVATE smh)
```

***

## Features

#### Handling end of statement errors**

The [ENUM_DERIVE](#enum_derive) and [MATCH_FROM](#match_from) macros could reach an end of statement where none of the
variants were reached yet the function must still return or crash (gracefully-ish). Each macro has its own `#define`
for this case which is wrapped in an `#ifndef`. If you would like to handle the case better, look in the source file to
find the name of this define and define it before you import `smh`.

*This will need improving in the future.*
***

### ENUM_DERIVE

**Usage:**

```c
ENUM_DERIVE(Test,
            Var1, Var2)

Test t = from_str("Var1");
assert(t == Test::Var1);
std::string t_str = to_str(t);
assert(t_str == "Var1");
```

**Generates:**

```c
enum class Test { Var1, Var2, }; inline std::string to_str(Test e) { switch (e) { case Test::Var1: return "Var1"; case Test::Var2: return "Var2"; default: return ""; } } inline Test from_str(const std::string &s) { if (s == "Var1") { return Test::Var1; } if (s == "Var2") { return Test::Var2; } std::cerr << "Error: passed string: `" << "v(s)" << "` is an invalid variant." << std::endl; std::exit(1); }
```

***

**Note:**
The following examples use a random number between 0 and 3 inclusive.

### MATCH

**Usage:**

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

**Generates:**

```c
if (x == 0) {std::cout << "x = 0!" << std::endl;} else if (x == 1) {std::cout << "x = 1!" << std::endl;} else if (x == 2) {std::cout << "x = 2!" << std::endl;} else if (x == 3) {{ x++; std::cout << "x = 4!" << std::endl; }}
```

***

### MATCH_FROM

**Usage:**

```c++
auto r = MATCH_FROM( // Requires C++ 11 as it uses a lambda function.
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

**Generates:**

```c++
auto r = [&]() { if (x == 0) {return 0.0;} if (x == 1) {return 1.0;} if (x == 2) {return 2.0;} if (x == 3) {{ x--; return 3.0; }} std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1); }();
```

***

### JSON_STRUCT

**Usages:**

```c++
JSON_STRUCT(MyStruct,
            (int, x),
            (float, y),
            (std::string, z))

std::string json_str = R"({"x":1,"y":2.0,"z":"test"})";
json j = json::parse(json_str);
MyStruct ms = j;
assert(ms.x == 1);
assert(ms.y == 2.0);
assert(ms.z == "test");
json j2 = ms;
std::string j2_str = j2.dump();
assert(j2_str == json_str);
```

**Generates:**

```c++
struct MyStruct { int x; float y; std::string z; }; inline void from_json(const json &j, MyStruct &s) { j.at("x").get_to(s.x); j.at("y").get_to(s.y); j.at("z").get_to(s.z); } inline void to_json(json &j, const MyStruct &s) { j = { {"x", s.x}, {"y", s.y}, {"z", s.z}, }; }
```

#### Seeing the output (using cmake)

**Note:** I would like to make a small tool that makes this easier in the future.

```
# Change directory/file names where relevant
mkdir build && cd build
cmake ..
make main.cpp.i && mv CMakeFiles/<PROJECT>.dir/main.cpp.o main.cpp.i
# You can find the relevant functions at the bottom of this massive file.
```

***

## TODO

- [ ] Rewrite `MATCH` and `MATCH_FROM` to not use `ML99_list`s.
- [ ] Add tests.
- [ ] Make tool that outputs only required part of the preprocessor output.