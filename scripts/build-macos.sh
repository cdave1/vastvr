#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
configuration="${1:-Debug}"
build_target="${2:-VastBuilder}"
case "$configuration" in Debug|Release) ;; *) echo "Usage: $0 [Debug|Release] [VastBuilder|VastBrowser|VastDevKit]" >&2; exit 2 ;; esac
case "$build_target" in
  VastBuilder) project="$repo_root/VastApps/VastBuilder/osx/xcode/VastBuilder.xcodeproj" ;;
  VastBrowser) project="$repo_root/VastApps/VastBrowser/osx/xcode/VastBrowser.xcodeproj" ;;
  VastDevKit) project="$repo_root/VastDevKit/build/xcode/VastDevKit.xcodeproj" ;;
  *) echo "Unknown target: $build_target" >&2; exit 2 ;;
esac

if [[ -z "${DEVELOPER_DIR:-}" ]] && [[ "$(xcode-select -p)" == /Library/Developer/CommandLineTools ]] && [[ -d /Applications/Xcode.app ]]; then
  export DEVELOPER_DIR=/Applications/Xcode.app/Contents/Developer
fi

exec xcodebuild -project "$project" -scheme "$build_target" \
  -configuration "$configuration" -destination "platform=macOS,arch=$(uname -m)" \
  -derivedDataPath "$repo_root/.build/DerivedData" \
  ONLY_ACTIVE_ARCH=YES MACOSX_DEPLOYMENT_TARGET=14.2 build
