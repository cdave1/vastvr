# Initial source import

This repository contains copies of the existing VastDevKit and VastApps working
trees, including local build fixes and excluding files already deleted by the
owner. The original repositories and their Git histories were not modified or
imported. Source commit identifiers below are provenance references only; their
commit graphs are not present in this repository.

## Snapshot revisions

| Source | Base commit |
| --- | --- |
| VastDevKit | `26536ef65872476c1896b137fd7d567b2218089c` plus working-tree changes |
| VastApps | `bd30345ed448be87cb8b5a4b306a500d344c1a89` plus working-tree changes |
| Box2D | `6a69ddbbd59b21c0d6699c43143b4114f7f92e21` |
| difont | `93b77671ca93856f6a98bc40e0d0fbeed03dd39f` |

## Import changes

- Original project code is MIT licensed under David Petrie's name. The owner's
  former-company copyright notices were replaced; third-party notices remain.
- Box2D and difont were materialized as ordinary source directories, without Git
  metadata or submodule pointers.
- Deleted source-tree files remain absent. No old history was used to restore them.
- Music resources were excluded and legacy sample playlists now start empty.
- DIN, Helvetica, and Diavlo fonts were removed, including the Diavlo bytes embedded
  in the engine and the difont sample. Carlito replaces text fonts and the embedded fallback; Font Awesome
  was retained with its license.
- The noncommercial `canyon_iq` and `circle_pattern` shaders were removed. Places
  referring to the circle shader now use an original, plain vertex-color shader.
- Font loading defaults to Unicode for the new TrueType fonts, resizing preserves
  the selected character map, and cache keys use the integer face size correctly.
- Xcode resource entries were pruned for removed media and the owner's deleted
  images/skyboxes; resource licenses were added to the build.
- Unused `curl-ios` archives and the old Oculus SDK bundles were excluded. The
  previously deleted `curl-android` tree remains absent.
- Git metadata, Conductor state, IDE caches, compiled binaries, and external
  filesystem symlinks were excluded.
- Uninitialized legacy submodules (`e2fsprogs` and `PhysicsHotel/_OLD/skeleton`)
  were not reconstructed; their obsolete gitlink configuration was not imported.
- VastBuilder uses the sibling VastDevKit directory. The macOS build script uses
  a common deployment target and builds the native architecture.

## Incremental follow-up work

The [third-party license audit](license-audit.md) records the 2026-10-14 review,
restored notices, cleanup, and remaining issues. The Base64 adaptation has been
replaced with original MIT code. Unused Oolong2, PGMidi and its MIDI integration,
GPL-marked iOS glue and dependent obsolete loaders/UI, the optional zlib Ada
binding, Oculus Android samples, and obsolete Oculus shaders/adapters have been
removed. Project references and header-search paths were pruned. Historical
PhysicsHotel stereo previews use their plain render-target shader, without lens
correction. Mobile/headset integrations remain unvalidated.

1. Keep the completed [asset inventory](asset-review.md) current when adding
   new resources. It records the owner's project-recording confirmation,
   replacements and removals, including the omitted Freesound-named click.
2. Keep the restored math/shader notices with source and future binary releases.
   [The completed snippet review](math-snippet-review.md) documents the recovered
   grants, original replacements, quaternion correction, and validation.
3. Add CI for the macOS baseline. The reproducible builder interaction and
   save/reload check is in `scripts/test-builder-macos.sh`; the demo-launcher
   check is in `scripts/test-browser-macos.sh`.
4. Upgrade dependencies one at a time with a passing build and smoke test after each
   update. Box2D, FreeType/difont, libpng, zlib, and SDL are initial candidates.
5. Remove remaining historical machine-specific paths from legacy targets as each
   target is restored. Apps other than VastBuilder/VastBrowser, and mobile/Windows
   projects, are currently unverified.
6. Prepare self-contained resources and SDL packaging if distributing app binaries.

The snapshot is a development baseline. The specifically identified click
recording has been removed. Historical targets and standalone binary packaging
remain separate follow-up work.

## Import validation

- VastBuilder Debug and Release builds passed on Apple Silicon.
- The loader regression test passed for Carlito, the embedded fallback, Unicode
  resizing, and separate font-size cache entries.
- A launch smoke test stayed running for eight seconds with no font errors. The
  legacy renderer still emits an OpenGL texture-binding diagnostic and shader
  interface warnings; this was not a full interactive application test.
- Secret scanning found only two filename-based false positives for PowerVR text
  shader `.pfx` files, with no credential findings.
- No old Git metadata or external filesystem links were copied.

After the license cleanup, clean Debug and Release builds, Base64 codec checks,
font regression tests, and an eight-second launch smoke test passed. No removed
shader references or font errors appeared; the preexisting OpenGL diagnostic
remains. The current secret scan has no findings after removing the old PowerVR
sample files. Details are in [the license audit](license-audit.md#cleanup-validation).

The asset/math review added original demo artwork, a verified CC0 teapot,
restored math/shader notices, original snippet replacements, and focused
quaternion/ray/GPU tests. The maintained Mac build and regression checks passed;
archived models/shader effects removed from this snapshot are recorded in the
asset inventory. Private originals remain available outside the public tree.

The three source-release checks completed on 2026-10-17: explicit MIT sound
licenses, an isolated Git export build/regression run, and automated native
builder interaction/save/reload checks. Corrections and remaining prototype
limits are recorded in [source release verification](release-readiness.md).

VastBrowser was restored on 2026-10-17 using the current SDK project and shared
Retina renderer. Debug/Release builds and native launcher, scene, piano, and
audio checks pass; the Builder regression check also passes. No removed media
was reintroduced. See the [Browser instructions](../VastApps/VastBrowser/README.md).
