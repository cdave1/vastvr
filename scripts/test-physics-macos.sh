#!/usr/bin/env bash
set -euo pipefail
repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [[ -z "${DEVELOPER_DIR:-}" ]] && [[ "$(xcode-select -p)" == /Library/Developer/CommandLineTools ]] && [[ -d /Applications/Xcode.app ]]; then
  export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
fi
objects="$repo_root/.build/DerivedData/Build/Intermediates.noindex/PhysicsHotel.OSX.build/Debug/PhysicsHotel.build/Objects-normal/$(uname -m)"
library="$repo_root/.build/DerivedData/Build/Products/Debug/libVastDevKit.a"
if [[ ! -d "$objects" || ! -f "$library" ]]; then
  echo 'Build first: ./scripts/build-macos.sh Debug PhysicsHotel' >&2; exit 1
fi
kit="$repo_root/VastDevKit"
link_objects=()
for object in "$objects"/*.o; do
  case "$(basename "$object")" in main.o|Client.o|AppDelegate.o) continue ;; esac
  link_objects+=("$object")
done
xcrun clang++ -std=c++11 -Wno-deprecated-declarations -Wno-nonportable-include-path -mmacosx-version-min=14.2 \
  -include "$kit/build/xcode/prefix.pch" \
  -I "$kit/include" -I "$kit/src" -I "$kit/lib/Box2D/Box2D" \
  -I /opt/homebrew/include -I /usr/local/include \
  -I "$kit/lib/difont/src" -I "$kit/lib/difont/lib/freetype2/include" \
  -I "$repo_root/VastApps/Common" -I "$repo_root/VastApps/Common/common" -I "$repo_root/VastApps/Common/entitysystems" -I "$repo_root/VastApps/PhysicsHotel/src" -I "$repo_root/VastApps/PhysicsHotel/osx" \
  "$repo_root/tests/physics-macos.mm" "${link_objects[@]}" "$library" \
  -framework Foundation -framework AppKit -framework OpenGL -framework IOKit \
  -framework CoreAudio -framework AudioToolbox -framework AVFoundation -framework QuartzCore \
  -L/opt/homebrew/lib -L/usr/local/lib -lSDL2 -o "$repo_root/.build/physics-check"
check_dir="$(mktemp -d "$repo_root/.build/physics-check.XXXXXX")"
ln -s "$repo_root/VastApps/resources" "$check_dir/resources"
"$repo_root/.build/physics-check" "$check_dir" "$@"
printf 'PhysicsHotel check artifacts: %s\n' "$check_dir"
