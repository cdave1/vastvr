#!/usr/bin/env bash
set -euo pipefail
repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [[ -z "${DEVELOPER_DIR:-}" ]] && [[ "$(xcode-select -p)" == /Library/Developer/CommandLineTools ]] && [[ -d /Applications/Xcode.app ]]; then
  export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
fi
mkdir -p "$repo_root/.build"
xcrun clang++ -std=c++11 -Wno-deprecated-declarations \
  "$repo_root/tests/shaders-macos.mm" -framework AppKit -framework OpenGL \
  -o "$repo_root/.build/shader-tests"
cd "$repo_root"
exec "$repo_root/.build/shader-tests"
