#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
build_dir="${1:-build}"
cmake -S . -B "$build_dir"
cmake --build "$build_dir" --parallel 4
"$build_dir/tests/training_main"

# Use the compiler and SDK selected by the successful project configuration.
cxx="$(sed -n 's/^CMAKE_CXX_COMPILER:FILEPATH=//p' "$build_dir/CMakeCache.txt")"
sdk="$(sed -n 's/^CMAKE_OSX_SYSROOT:PATH=//p' "$build_dir/CMakeCache.txt")"
compiler_args=(-std=c++17 -Wall -Wextra -Werror -I include)
if [[ -n "$sdk" ]]; then compiler_args+=(-isysroot "$sdk"); fi
scratch_dir="$(mktemp -d "${TMPDIR:-/tmp}/lab4-verify.XXXXXX")"
trap 'rm -rf "$scratch_dir"' EXIT
"$cxx" "${compiler_args[@]}" script/validate_lab4.cpp "$build_dir/src/libtraining.a" -o "$scratch_dir/reference"
"$scratch_dir/reference"
