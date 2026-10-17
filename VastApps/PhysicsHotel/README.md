# PhysicsHotel

A physics sandbox restored for macOS. Balls and boxes fall, bounce, stack, and
can be picked up with the mouse. It uses the existing Box2D source in VastDevKit
and the shared Retina renderer; no additional dependency or media download is
needed.

The historical source contains several unfinished experiments: a bounded 2D
physics scene with circular bodies, extruded star artwork, pointer joints, and
player/harvest-item scaffolding, followed by a static 3D rendering experiment.
There are no implemented levels, score, or win condition. The maintained desktop
scene reconstructs the physics playground with procedural balls and boxes and
adds explicit sandbox controls. It is not a complete historical game recreation.

From the repository root, with Xcode and Homebrew SDL2 installed:

```sh
./scripts/build-macos.sh Debug PhysicsHotel
open .build/DerivedData/Build/Products/Debug/PhysicsHotel.app
```

Use `Release` instead of `Debug` for an optimized build. The shared Xcode scheme
is `PhysicsHotel`, in `osx/xcode/PhysicsHotel.OSX.xcodeproj`.

| Control | Action |
| --- | --- |
| Left mouse drag | Pick up a shape and move it; release anywhere to let go |
| Space | Add a shape, up to 64 |
| Backspace | Remove the last shape |
| P | Pause or resume physics |
| G | Toggle gravity |
| R | Reset to the initial 16 shapes, running with gravity |
| Command-Q / close window | Quit |

The window resizes with correct pointer coordinates, aspect ratio, and native
Retina render targets. Leaving the window cancels an active drag when it loses
keyboard focus. Physics uses a fixed 60 Hz step with a bounded catch-up interval.
The release sound is an existing MIT-licensed project recording. Nothing is saved.

## Resource licenses

The source review found no outstanding provenance issue among the 12 retained
assets in `resources/`; their bytes match [the asset inventory](../../docs/assets.json).
The folder also contains the recording's complete license text.

| Resources | Provenance and license |
| --- | --- |
| `wood.png`, `noise.png` | Original MIT artwork from `scripts/generate-demo-assets.py` |
| Nine legacy shader files | Reviewed project shaders/original replacements, MIT; see [the snippet review](../../docs/math-snippet-review.md) |
| `blockSelect03.caf` | Project recording, retained under the owner's MIT grant in `resources/SOUND_EFFECTS_LICENSE.txt` |

The maintained app loads shared GL3 shaders, the unmodified Carlito Bold font
(SIL OFL 1.1), and the MIT project effect `resources/sfx/converted/blop_002.wav`.
Its balls, boxes, and walls are generated in code. It does not load the legacy
PhysicsHotel texture/shader/CAF folder or any removed photograph, model, or music.
Box2D retains the bundled version's zlib-style license. The SDK's other existing
dependency notices still apply; these resources and dependencies are not all
relicensed by the root MIT grant. See [third-party notices](../../THIRD_PARTY_NOTICES.md).
Source notices are included in the repository. Distributing standalone binaries
still requires the resource, SDL, and notice packaging documented below.

Run the native interaction check after a Debug build:

```sh
./scripts/test-physics-macos.sh
```

It checks gravity, collisions and bounds, mouse joints, release away, pause,
gravity toggle, adding/removing shapes and capacity limits, reset, resize picking,
Retina presentation, and visible geometry matching a physics body. Cocoa events
enter the actual view callbacks/input queue; this does not test system-wide event
delivery or require Accessibility/Screen Recording permission. Frames and private
test resources are written under `.build/physics-check.*`.

The active implementation is in `src/` and the current Mac window uses
`osx/PhysicsView`. The old `game/`, root `PhysicsGame.*`, unused Mac sources,
`_OLD/`, and iOS project are retained as historical reference and are not compiled
by the maintained scheme. Removed/unverified images and old headset integrations
stay excluded. As with the other apps, this is a development bundle loading
resources from the checkout and SDL2 from Homebrew. The shared renderer's existing
OpenGL startup diagnostic remains.
