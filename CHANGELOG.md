# Changelog

## [Unreleased]

### Build system
- Refactored `CMakePresets.json` to use shared `common`, `vcpkg`, and `clang` base presets and added vcpkg toolchain integration.
- Updated `CMakeLists.txt` for static-linked executables, added the `BUILD_STATIC_EXE` option, and switched to vcpkg-installed `libRocket`, `SDL2`, and `nomlib` discovery.
- Relaxed `cmake/doxygen.cmake` so Doxygen is optional at configure time instead of a fatal error.
- Expanded `vcpkg.json` with license, homepage, maintainers, `supports`, and explicit platform dependencies including `openal-soft`, `sdl2-image`, `sdl2-ttf`, and Wayland support on Linux.

### Added
- Added `Resources/SharedSupport/gamecontrollerdb.txt` SDL game controller database resource.
- Added `NOM_GCDB_RESOURCE_FILE` path handling in `src/CMakeLists.txt` and exposed it as `GCDB_RES_FILE` in `version.hpp`.
- Added `__builtin_trap` debug action bound to `F12` in debug builds.
- Added `TTCARDS_ENABLE_AUDIO_PLAYSTOP` compile definition.
- Added shutdown of the game controller subsystem in `Game::~Game`.
- Added mouse wheel and game-controller button bindings to `CardsMenuState`.

### Changed
- Converted `TTCARDS_VERSION_GIT_REV` from a preprocessor macro to a `std::string` constant in `version.hpp.in`.
- Reordered and cleaned up keyboard/controller input mappings in `CardsMenuState`.

### State machine
- Introduced a common `GameState` base class and modernized state creation in `Game::set_state()` from raw `new`/typedef pointer aliases to `std::make_unique`.
- Added `Game::state_machine()` accessor.

### Input handling
- Fixed startup crash caused by stale `nomlib` static-library ABI mismatch; debug and debug-dynamic presets now produce working executables.

### Filesystem
- Replaced deprecated `nom::File`/`nom::Path` usage with `nom::filesystem::path` and `nom::filesystem::exists`/`current_path` APIs, and improved resource-directory discovery relative to the executable.

### Documentation
- Added `AGENTS.md` project instructions for automated agents.
- Added `.omp/RULES.md` with the rule that `nomlib.git` is read-only.
- Recorded segfault investigation notes in `wiki/build-logs/segfault_findings.md`.
