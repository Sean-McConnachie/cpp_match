# smh

Bringing a little bit of Rust to C++.

## Motivation

Recently, I experienced a few stupid bugs in my code that could have been avoided if I had used Rust instead of C++.
These bugs were caused by the fact that I'm not a perfect programmer and I make mistakes. 🐛s:

- [match]("https://doc.rust-lang.org/rust-by-example/flow_control/match.html") Falling through a switch statement and
  using `MJPG` for 3 weeks instead of `H264`.
- [derive]("https://doc.rust-lang.org/reference/attributes/derive.html") Forgetting to update fields in json
  configuration structs (there are two functions and one field that must be modified).

## Table of Contents

- [Info](#info)
- [Install](#install)
- [Features](#features)
    - [MATCH](#match)
    - [MATCH_TO](#match_to)
    - [JSON_STRUCT](#json_struct)
- [Seeing the output](#seeing-the-output-using-cmake)

## Info

For generating code, this project uses the [Metalang99 Preprocessor]("https://github.com/Hirrolot/metalang99").

As most of my projects use the [nlohmann::json]("https://github.com/nlohmann/json") library, this is what
the `JSON_STRUCT` macro is designed for.

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

## Features

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

### MATCH_TO

**Usage:**

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

**Generates:**

```c++
auto r = [&]() { if (x == 0) {return 0.0;} if (x == 1) {return 1.0;} if (x == 2) {return 2.0;} if (x == 3) {{ x--; return 3.0; }} std::cerr << "Error: Missing return statement!\n" << std::endl; exit(1); }();
```

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