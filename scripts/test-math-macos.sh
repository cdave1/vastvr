#!/usr/bin/env bash
set -euo pipefail
repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
library="$repo_root/.build/DerivedData/Build/Products/Debug/libVastDevKit.a"
if [[ ! -f "$library" ]]; then
  echo "Build first: ./scripts/build-macos.sh Debug" >&2
  exit 1
fi
if [[ -z "${DEVELOPER_DIR:-}" ]] && [[ "$(xcode-select -p)" == /Library/Developer/CommandLineTools ]] && [[ -d /Applications/Xcode.app ]]; then
  export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
fi
kit="$repo_root/VastDevKit"
xcrun clang++ -std=c++11 -mmacosx-version-min=14.2 \
  -fsanitize=undefined -fno-sanitize-recover=all \
  -include "$kit/build/xcode/prefix.pch" \
  -I "$kit/include" -I "$kit/src" -I "$kit/lib/Box2D/Box2D" \
  "$repo_root/tests/math.cpp" "$kit/src/graphics/components/RayHitTester.cpp" "$library" \
  -framework Foundation -framework AppKit -framework OpenGL \
  -framework IOKit -framework CoreAudio \
  -L/opt/homebrew/lib -L/usr/local/lib -lSDL2 \
  -o "$repo_root/.build/math-tests"
"$repo_root/.build/math-tests"
