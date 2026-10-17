# VastBrowser on macOS

VastBrowser is the VR demo launcher. Build it from the repository root with the
same Xcode and Homebrew SDL2 setup as VastBuilder:

```sh
./scripts/build-macos.sh Debug VastBrowser
open .build/DerivedData/Build/Products/Debug/VastBrowser.app
./scripts/build-macos.sh Release VastBrowser
```

The shared Xcode scheme also builds directly from `osx/xcode/VastBrowser.xcodeproj`.
These development builds read resources from this checkout and SDL2 from
Homebrew; they are not standalone distribution bundles.

## Controls and demos

Arrow keys look around; the center cross selects what you are looking at.
Press Space to select a card or interact with a scene. WASD moves the camera.
Tab returns to the launcher; 1 and 2 cycle backward/forward, including home.
Command-Q quits the application.

- **Teapots:** the verified CC0 Utah teapot model in a small scene.
- **Shadows:** procedural floor and blocks using the existing shadow pass.
- **Geometry:** procedural blocks; select one with Space to change its color.
- **Piano:** 88 keys with original synthesized tones, including raised black
  keys. Look at a key and press Space. It plays one note at a time and stops
  queued audio when you leave the scene; no sampled instrument is bundled.
- **Music & audio:** choose your own local audio file, audition a project sound,
  or stop playback. Audio loops until stopped or you leave this scene. The app
  does not copy selected files into the repository. Failed loads retain any
  current playback and show an error.

No music or previously excluded models have been restored. The iCracked scene
is omitted from the launcher because its original model was not retained. The
historical SSAO scene is a geometry preview: its ambient-occlusion pass remains
disabled, and it is labeled Geometry accordingly.

## Verification

After a Debug build:

```sh
./scripts/test-browser-macos.sh
```

The check creates its own Cocoa AppView and uses the same registration/startup
as the app. It sends app-local keyboard events through the normal input queue
to select every scene, return home, cycle across the first/last scene, move the
camera, play piano notes, and start/stop audio. Small piano targets use precise
tracked-head poses before normal Space events. A private audio file with spaces
and a UTF-8 name exercises the playback handler used after file selection.
The operating-system file picker itself is not automated by this check.

Rendered frames verify Retina viewport coverage and visible teapot geometry,
including after resizing. Frames and the private audio copy stay under
`.build/browser-check.*`. These checks require a logged-in Mac desktop and
audio output; they do not request Accessibility or Screen Recording access.

The app shares prototype engine limits recorded in
[source release verification](../../docs/release-readiness.md). Historical
platform/headset targets remain unvalidated.
