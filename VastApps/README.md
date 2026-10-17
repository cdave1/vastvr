# VastApps

Applications and shared resources for VastDevKit. Both projects now live in the
same repository; no workspace symlinks or additional Git checkouts are needed.

From the repository root:

```sh
./scripts/build-macos.sh Debug
open .build/DerivedData/Build/Products/Debug/VastBuilder.app
./scripts/build-macos.sh Debug VastBrowser
open .build/DerivedData/Build/Products/Debug/VastBrowser.app
```

See the [root README](../README.md) for dependencies, supported targets, and licensing.
VastBuilder and [VastBrowser](VastBrowser/README.md) are the current macOS apps;
the other apps are legacy projects.
