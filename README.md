# VastVR

VastVR is a C++ engine used to develop virtual reality applications during
2015–2018. This repository preserves the VastDevKit engine alongside the tools,
applications, and experiments built around it in VastApps.

The engine and apps were originally maintained in separate repositories and are
now combined into this open-source snapshot. Recent restoration work makes the
SDK and selected macOS apps buildable and usable on current Macs, so the original
work can be explored today. Historical VR, mobile, and Windows targets remain
unvalidated; see [Status](#status) below.

- `VastDevKit/`: engine, source snapshots of dependencies, and legacy tools.
- `VastApps/`: applications and shared resources.
- `scripts/`: reproducible macOS build and embedded-font generation.

## Build the macOS apps

Install Xcode and SDL2, then build from the repository root:

```sh
brew install sdl2
./scripts/build-macos.sh Debug
open .build/DerivedData/Build/Products/Debug/VastBuilder.app
```

The script builds for the current Mac architecture with a macOS 14.2 deployment
target. It selects `/Applications/Xcode.app` when the system developer directory
points to Command Line Tools; set `DEVELOPER_DIR` for a different Xcode install.

```sh
./scripts/build-macos.sh Release
./scripts/build-macos.sh Debug VastDevKit
./scripts/build-macos.sh Debug VastBrowser
open .build/DerivedData/Build/Products/Debug/VastBrowser.app
./scripts/build-macos.sh Debug PhysicsHotel
open .build/DerivedData/Build/Products/Debug/PhysicsHotel.app
```

Box2D, difont, and FreeType are included as source files: no submodule setup,
external workspaces, or prebuilt engine library is needed. Xcode also works by
opening `VastApps/VastBuilder/osx/xcode/VastBuilder.xcodeproj` directly.

These are development builds: resources are loaded from this checkout and SDL2
is loaded from Homebrew. They are not standalone distributable app bundles.

## Status

The maintained starting point is VastBuilder, VastBrowser, PhysicsHotel, and VastDevKit on macOS. Other app,
Android, iOS, Windows, and tool projects are retained for future work and have not
been validated. Some legacy SDKs and uninitialized dependencies were deliberately
excluded; see [the import notes](docs/import.md).

[VastBrowser](VastApps/VastBrowser/README.md) launches the teapot, shadow, geometry,
piano, and local-audio demos. It uses the shared Retina renderer and current SDK.

[PhysicsHotel](VastApps/PhysicsHotel/README.md) is a physics playground for
dragging, bouncing, and stacking balls and boxes, with pause/gravity/reset controls.

Music is not bundled. UI fonts and the engine's embedded fallback font use
Carlito under SIL OFL 1.1. Font Awesome 4.2.0 remains under its original OFL license.
To regenerate the fallback after changing the licensed font source:

```sh
python3 scripts/generate-default-font.py
```

## Checks

After a Debug build, run the focused loader regression checks:

```sh
./scripts/test-fonts-macos.sh
```

This checks the new fonts, embedded fallback, Unicode character-map preservation,
and separate cache entries for different face sizes.

The independent Base64 regression checks do not require an engine build:

```sh
./scripts/test-base64.sh
```

This checks standard vectors, binary data, legacy decoder formatting, and invalid
input, and compares the codec against Python with UndefinedBehaviorSanitizer
enabled. Set `BASE64_SANITIZERS=address,undefined` to also use AddressSanitizer
when supported by the installed compiler/runtime.

Math and changed-shader checks are also available:

```sh
./scripts/test-math-macos.sh
./scripts/test-shaders-macos.sh
```

The math test requires the Debug SDK build. The shader test creates a Mac OpenGL
context, compiles changed fragments, and renders hue/blur boundary cases.
The builder integration check runs a private scene, exercises keyboard/menu
editing and sound, saves it, and reloads it in a second process:

```sh
./scripts/test-builder-macos.sh
```

After building VastBrowser in Debug, check its launcher, scenes, piano/audio
interaction, and Retina presentation with `./scripts/test-browser-macos.sh`.
After building PhysicsHotel in Debug, check its physics, mouse interaction,
keyboard controls, and resizing with `./scripts/test-physics-macos.sh`.

The builder check's save files and rendered frames stay under
`.build/builder-check.*`; it does not modify your working scene. Desktop controls use WASD to move, arrow keys to
look, Space to paint/select/erase, and Tab to toggle the menu. Saves are written
to `VastApps/saves/MySaveFile.json`, which is ignored by Git.

See [source release verification](docs/release-readiness.md) for the clean-source
checks, tested behavior, and remaining prototype limitations.

Original replacement icons/textures can be reproduced with
`python3 scripts/generate-demo-assets.py`.

## Licensing

Original VastDevKit/VastApps code is available under the [MIT license](LICENSE).
The project-created sound effects are also MIT licensed; see their
[complete license](VastApps/resources/sfx/LICENSE.txt).
Bundled third-party code and assets retain their own licenses; the root MIT
license does not relicense them. See [third-party notices](THIRD_PARTY_NOTICES.md).

The [asset review](docs/asset-review.md) and [math/snippet review](docs/math-snippet-review.md)
record preserved project recordings, original artwork/snippet replacements,
verified teapot resources, and restored notices. The unverified Freesound click
was removed; its callers use an existing project sound effect. Remaining work
is tracked in [the import notes](docs/import.md).
The [license audit](docs/license-audit.md) records the Base64 replacement and
removal of unused Oolong2, PGMidi, GPL-marked iOS glue, zlib Ada contributions,
and obsolete Oculus samples and shaders. Historical mobile/headset projects
omit these integrations and remain unvalidated.
