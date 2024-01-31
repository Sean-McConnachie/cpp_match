# python generate_ml99_undefs.py --ml99-dir=cmake-build-debug/_deps/metalang99-src/include/ --output=include/metalang99undef.h

import os
import sys


def find_macros(fp: str) -> list[str]:
    with open(fp, "r") as f:
        lines = f.readlines()

    ml99_macros = []
    for line in lines:
        if line.startswith("#define ML99_"):
            macro_defn = line.split()[1]
            macro_name = macro_defn.split("(")[0]
            ml99_macros.append(f"{macro_name}")

    return ml99_macros


if __name__ == "__main__":
    ml99_dir = None
    output = None
    for arg in sys.argv[1:]:
        if arg.startswith("--ml99-dir="):
            ml99_dir = arg[len("--ml99-dir="):]
        elif arg.startswith("--output="):
            output = arg[len("--output="):]

    if ml99_dir is None:
        raise RuntimeError("missing --ml99-dir")
    if output is None:
        raise RuntimeError("missing --output")

    ml99_undefs = []

    # Get all the header files
    h_files = []
    for root, _, files in os.walk(ml99_dir):
        for file in files:
            if file.endswith(".h"):
                ml99_undefs.extend(find_macros(os.path.join(root, file)))

    # Generate the output
    with open(output, "w") as f:
        f.write("/* !!! THIS IS AN AUTO-generated file !!!\n"
                " * It's purpose is to prevent errors with other meta-object compilers such as the one found in the "
                "QT framework. */\n")

        f.write("#undef v\n")  # special case
        for macro in ml99_undefs:
            f.write(f"#undef {macro}\n")
