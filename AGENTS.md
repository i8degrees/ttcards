# Project instructions

ttcards is a video game remake of Triple Triad, a side-game of the
Final Fantasy VIII console game released in 1998 by Square for the
PlayStation (PS1).

- https://github.com/i8degrees/ttcards.git

## Build and test

- Any time a JSON file is updated, ensure that it lints correctly by using
the `json5-validator` tool or a JSON3 with comments parsing enabled.

### Dependencies

Ensure the following build dependencies are installed:

- Linux
    - `cmake`, `git`, `clang++`, `vcpkg`
- Windows
    - `cmake.exe`, `git.exe`, `vcpkg.exe`, `cl.exe`
- macOS
    - `cmake`, `git`, `clang++`, `vcpkg`, `brew`

Optional, only when building documentation:

- `doxygen`, `graphviz`

vcpkg should be bootstrapped locally and `VCPKG_ROOT` set so that CMake can
locate the toolchain file at
`$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake`. ttcards dependencies are
managed by vcpkg; see `vcpkg.json` and `vcpkg-configuration.json`. Do
not modify `vcpkg_installed/` directly.

### Configure and build

Generate build files and compile from the project root using CMake presets:

Debug build:

```sh
cmake -S . --preset debug
cmake --build --preset debug
```

Release build:

```sh
cmake -S . --preset release
cmake --build --preset release
```

Build output lands under `build/<presetName>` because the presets set
`binaryDir` to `${sourceDir}/build/${presetName}`.

Temporary debug build:

```sh
cmake -S . --preset debug -B /tmp/ttcards
```

Temporary release build:

```sh
cmake -S . --preset release -B /tmp/ttcards
```

If a custom configuration path is needed:
```sh
# json configuration files
TTCARDS_DIR=$HOME/.config/ttcards
```

### Documentation

To generate the Doxygen HTML documentation, enable the `DOCS` option:

```sh
cmake -S . --preset debug -DDOCS=ON
cmake --build --preset debug
```

## Unit Tests

- No official unit tests exist in this project. nomlib's API is extensively
tested in its repo.

## Conventions

- All existing source files must be valid C++14 and must be able to compile
using the `clang` toolchain on Linux, macOS, and Windows.

- New source files are permitted to use C++17 (without extensions) **only
if** the build is updated to compile as C++17 on all supported platforms
triplets. Until then, the effective standard remains C++14 because
`cmake/StandardProjectSettings.cmake` sets `CMAKE_CXX_STANDARD 14` and
`CMAKE_CXX_STANDARD_REQUIRED ON`.

- Windows 10 and 11 use Microsoft Visual C++ Build Tools 2022 with the
`clang-cl` dialect. Consult `CMakePresets.json` for the general build
environment.

- ttcards has a wiki at https://github.com/i8degrees/ttcards/wiki of which
is also available as a submodule called `wiki.git` in the repository root.
This path is intended to be a collection of thoughts, ideas, and
experiments that have happened during the life of this project. It should
be considered read-only except by explicit consult for me to review.

- ttcards dependencies should always be handled by `vcpkg`. See also:
`vcpkg.json` and `vcpkg-configuration.json`. The `vcpkg_installed`
directory should never be modified because it is managed by vcpkg.

- Keep public API changes backward compatible.

- Never edit files under the following paths:
`tmp`, `Resources`, `Documentation`, `extra`, `build`, `build.*`

- The `Resources` path is append-only. Do not delete, overwrite, or modify
existing files there; add new files only.

- Prefer CMake presets over the helper scripts in `bin/` for normal builds.
Those scripts exist for historical or platform-specific workflows and
should only be relied on when explicitly requested.

- `compile_commands.json` and `build/` contents are generated. Do not
hand-edit or commit them.

- Do not use ssh key for GPG signing when using `git commit` or `git push` from the agent.

