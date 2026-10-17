# Source release verification

Completed 2026-10-17 on Apple Silicon with Xcode 15.2 and the macOS 14.2 SDK.

## Sound-effect licensing

The project owner explicitly authorized MIT licensing for the 53 project audio
files. Complete terms are beside the [shared effects](../VastApps/resources/sfx/LICENSE.txt)
and the [archived PhysicsHotel recording](../VastApps/PhysicsHotel/resources/SOUND_EFFECTS_LICENSE.txt).
The audio bytes remain unchanged. The Freesound-named sample stays omitted.
The [asset inventory](assets.json) records the applicable MIT notice per file.

## Clean source copy

Verification uses a Git tree assembled through a temporary index, exported into
an empty directory. The working index and branch are unchanged. Only files
eligible for the source import are present: no previous build products, private
context/recovery material, local Xcode overrides, or original repositories.
The build creates fresh SDK, difont/FreeType, and builder products in that copy.
SDL2 remains the documented external Homebrew dependency.

The verified teapot keeps its upstream line endings: `.gitattributes` explicitly
prevents model text conversion, preserving the archive's recorded byte hashes.
All retained asset hashes and restored notice hashes are checked in the export.

Debug and Release builds pass, as do Base64, fonts, UBSan quaternion/ray checks,
GPU shader checks, and the builder integration check. Secret scanning of the
export reports no findings. The previous ASan runtime limitation remains as
recorded in the license audit.

## Builder interaction check

Run after a Debug build:

```sh
./scripts/test-builder-macos.sh
```

The check runs the real Builder, AppController, SDK, and Cocoa AppView in a
window, with a small test bootstrap retaining a pointer for scene assertions.
It sends Cocoa keyboard events through the view and normal input queue. Arrow
keys direct gaze, Space selects menu controls/paints/erases, and Tab toggles the
menu. A precise tracked-head pose is supplied for selecting a distant tiny
block. The check does not require desktop Accessibility or Screen Recording
permission and does not validate operating-system-wide event delivery.

It uses a private `.build/builder-check.*` resource/save directory. The normal
`saves/MySaveFile.json` scene in `VastApps` is never opened or changed by this
check. It verifies:

- Keyboard movement and painting two separate blocks.
- Selecting a hue through the real menu and applying it to new paint.
- Selecting Erase and deleting the first block through gaze/Space input.
- Saving through the menu, then restoring the remaining block in a second
  process with matching position/color and no saved brush preview.
- First writes, directory creation, UTF-8 names, and shorter overwrites.
- Starting/stopping the project sound, looping playback, and invalid-path errors.
- The final viewport matching the full Retina backing surface and scene pixels
  reaching all four window quadrants.
- Scene color/lighting/normal/depth attachments allocating one texel per backing
  pixel. A one-pixel checkerboard inserted into the scene must survive the real
  composite/presentation passes unchanged, catching upscaling and final blur.
  These checks repeat after the window changes size. Rendered frames are saved
  with the check.

These checks exposed existing prototype defects that were corrected: disabled
save writes, incorrect save parsing bounds, saving the transient brush preview,
the macOS sound stub, an invalid neutral headset rotation, duplicate/uninitialized
camera pitch updates, mono culling using a stereo projection, ray direction vs
endpoint confusion, hidden-menu input handling, inactive color selection, and
the final viewport left at an offscreen texture's smaller size. The application
window now fits the screen instead of forcing a minimum 3072×1920 point size.
The OpenGL view requests a Retina surface, and mono offscreen targets follow its
backing pixel size while projections/input retain view-point coordinates. The
mono presentation pass no longer blurs the completed scene, preserving text and
one-pixel detail. A 1280×800 point test window renders at 2560×1600 pixels on the
2× display, then at 1920×1200 pixels after resizing to 960×600 points.
Failed saves prevent New from discarding the current scene. macOS writes create
parent directories and atomically replace the complete file.

## Remaining scope limits

This is a source development snapshot. Other historical platforms/apps remain
unvalidated. Binary releases still need standalone resources, SDL packaging,
and complete bundled notices. CI and dependency upgrades remain follow-up work.

The legacy renderer still emits a core OpenGL diagnostic and unused shader-output
warnings. The on-screen keyboard and Undo history remain unfinished prototype
features; the keyboard check above covers physical-keyboard input. macOS sound
supports volume, looping, and stopping; the legacy pitch/pan arguments remain
unsupported. None of these are claims of a polished end-user app release.

## VastBrowser restoration

VastBrowser now builds with the current SDK project and Homebrew SDL2, in Debug
and Release. Its launcher registers the verified teapot, procedural shadows and
geometry, synthesized piano, and local-audio scenes. Removed models/music remain
omitted; the inactive SSAO scene is presented as Geometry. See the
[Browser instructions](../VastApps/VastBrowser/README.md) for controls and scope.

`scripts/test-browser-macos.sh` covers native keyboard scene selection, previous/
next wraparound, home navigation, movement, piano playback and release away from
a key, local/project audio playback and stopping, and Retina viewport coverage.
The shared renderer binds each mesh VAO explicitly and owns the VAO used for
index uploads, preventing draws/uploads from changing another renderer's state.
Picking compares actual surface distances after broad-phase rejection; a large
floor's bounding sphere no longer takes priority over a closer piano key.
