#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

if [ ! -f build/build.ninja ]; then
    meson setup build
fi

meson compile -C build
exec ./build/raylib-game "$@"
