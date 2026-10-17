# Third-party license audit

Reviewed and updated 2026-10-14. The copied Base64 implementation has been
replaced, and the unused legacy components identified below have been removed
from this snapshot. The asset and math/snippet reviews are now recorded below.
The outstanding Freesound-named click was deleted at the owner's direction,
closing that provenance question. The recorded review scope and distribution
requirements still apply.

## Scope and method

Reviewed bundled license documents, source copyright/permission notices, and
comments identifying copied code. Compared them with the Sources and Resources
phases in the VastBuilder, VastDevKit, and difont macOS Xcode projects. Those
projects list 34, 209, and 58 source files respectively. Header-only code and
conditional compilation require separate consideration; these counts are not
an exhaustive binary dependency analysis.

The macOS source phases include Box2D, libpng, zlib's core, JsonCpp, Duktape,
difont, and FreeType. SDL is external. The removed legacy source files were
outside those source phases. Resource entries for obsolete Oculus/SSAO/fractal shaders and removed photos
were pruned; a Maps icon uses a former stock-photo resource entry. The
maintained target now has 90 resource entries.

Evidence comes from the actual retained files and the linked upstream sources,
not from applying today's license to an unidentified old version. Retained text
sources and SHA-256 hashes are recorded in [license-sources.json](license-sources.json).
Private working reports and the removal manifest are under `.context/`.

## Completed cleanup

| Original issue | Resolution |
| --- | --- |
| Stack Overflow-derived Base64 implementation | Replaced both `VastDevKit/src/core/encoding/Base64.h` and `.cpp` with original MIT-licensed code implementing the byte grouping in [RFC 4648 section 4](https://www.rfc-editor.org/rfc/rfc4648.html#section-4). The old tables and implementation are absent. The public API and permissive decoding behavior are preserved: ASCII whitespace and padding are ignored, unpadded input is accepted, and invalid characters return an empty string. |
| Unestablished PowerVR grants and missing GIMPACT notices in Oolong2 | Removed the entire unused `VastDevKit/lib/Oolong2/` snapshot, including its nested Bullet, ENet, shader samples, and example artwork. Removed Oolong header-search references. This also removes the GIMPACT and San Angeles notice questions from this snapshot. |
| GPL-marked Wolf3D iOS adaptation | Removed `src/graphics/gl/ios/hdOpenGLES.h`, the already-stale Xcode reference to its absent implementation, and the unused PVRTC loader and old text-area files that depended on it. |
| Optional GPL Ada contribution | Removed `lib/zlib-1.2.7/contrib/ada/` and its catalog entry. Core compression sources remain unchanged. The vendored zlib README records this snapshot omission. |
| Oculus-derived Android samples and obsolete lens-distortion resources | Removed the three NativeActivity sample sources under `VastApps/Common/android/`, eleven Oculus-named shaders across shared and PhysicsHotel resources, and obsolete `DeviceDetector.cpp` / Oculus 0.4.4 adapter source/header containing embedded sample shaders. Pruned Xcode resources/references. Archived PhysicsHotel stereo previews now refer to their existing plain render-target shader; they no longer provide lens correction. |
| PGMidi custom-permission source | Removed `src/sound/ios/PGMidi/`, its NetworkMidi wrapper/header, the public umbrella include, and disabled sample hooks. Removed Xcode/Windows project references. The former MIDI integration is intentionally absent pending a future implementation with established terms. |

The cleanup removed 1,389 files (about 42.9 MB). The original repositories were
not edited. This public source snapshot has no old project Git history; omitted
files are not imported from that history. A local recovery archive is stored in
the gitignored `.context/` directory and is not part of the publishable tree.

The historical Android and iOS projects remain unvalidated and now omit these
integrations. Their presence does not imply platform support. Retained owner
integration code for separately installed SDKs is also unvalidated.

## Findings still requiring follow-up

| Finding | Retained files | Evidence and recommended action |
| --- | --- | --- |
| Standalone binary notices | Future distributable app or library packages | Include the applicable complete notices, font licenses, FreeType acknowledgment, and notices for the actual external SDL/SDL2-compat/SDL3 packages shipped. The current development app bundle is not a complete binary notice package. |

## Asset and math/snippet review

[The asset report](asset-review.md) and [hashed inventory](assets.json) cover
261 retained assets. The owner confirmed project-specific creation of sound
effects, and 53 recording files are unchanged. Fifty-eight reproducible original
images replace unknown menu/tool/launcher icons and textures. Unknown photos,
panoramas, rock/unnamed models, Sibenik, and unused shader samples were removed.
The teapot is now a byte-verified, freshly downloaded CC0 archive variant.
The Freesound-named click was removed at the owner's direction; sixteen callers
now reuse the unchanged project `blop_002.wav` recording.

[The snippet report](math-snippet-review.md) records the restored SoftSurfer,
Stefan Reinalter MIT, Oolong math zlib-style, and NVIDIA shader BSD grants. The
unverified two-vector quaternion, direction-based ray test, Windows GUID
formatting, and active shader blocks have original replacements. The quaternion
replacement fixes an out-of-bounds vector access. Standard matrix/interpolation
formula references were distinguished from copied code. Legacy platform
implementations remain unvalidated.

## License texts and attribution retained

- Restored util-linux's complete BSD-3-Clause terms at
  `VastDevKit/lib/Documentation/licenses/COPYING.BSD-3`, the location already
  referenced by `libuuid/COPYING`. The text matches the retained libuuid source
  permission notices. The upstream v2.25 tag records the license text's source;
  it does not identify the precise version of every imported libuuid source.
- Added `docs/licenses/John-Chapman-MIT.txt` for the OBJ parser section in
  `VastDevKit/src/graphics/mesh/MeshLoader.cpp`. Its existing notice credits
  John Chapman, 2013, and explicitly declares MIT, but referred to a missing
  `license.txt`. The added notice uses the standard MIT text and existing
  author/year; it is not claimed to be his recovered original file.
- Retained complete GPLv3 text in `docs/licenses/GPL-3.0.txt` for GLEW's separate
  `config/config.guess` helper. Its conditional Autoconf distribution exception
  remains in the helper. It is not part of the GLEW runtime library.
- The restored San Angeles and PGMidi statements were removed with their
  components. The additional GPLv2 document was removed after removal of the
  legacy GPLv2 code. Their historical provenance remains recorded separately
  in `license-sources.json`, rather than as notices for retained components.

## Other terms to preserve

The core libraries retain permissive notices: Box2D (zlib), difont and Duktape
(MIT), JsonCpp (public-domain/MIT alternatives), libpng (libpng license), and
FreeType (the chosen FreeType Project License). FreeType 2.4.4 is a reduced
source distribution; its README documents excluded font modules. Its FTL notice
requires source modifications to be documented and acknowledgment in binary
distribution documentation.

Retained legacy trees include iGLU under SGI Free Software License B 2.0/MIT,
JavaScriptCore API headers under BSD-2-Clause notices, CocosDenshion under MIT
with separately licensed Apple sample-derived files, and Reachability with both
Apple's sample-code terms and Donoho Design Group's BSD-style extension notice.
VFP/NEON math retains zlib-style notices. The iOS surface adaptations refer to
the owner's `iglview` project and declare MIT in source comments; its upstream
README contains no separate license document.

The [zlib FAQ](https://zlib.net/zlib_faq.html) confirms that optional contributions
have individual licenses. DotZLib retains its Boost 1.0 text; other retained
contributions keep their individual notices. GLEW also ships a GPLv2 document
in `doc/gpl.txt`; that license document does not make the GLEW runtime GPL
licensed. GPL and other third-party license texts do not relicense original
project code covered by the root MIT grant.

Carlito and Font Awesome retain OFL notices; Box2D's Droid Sans font retains
its Apache-2.0 text. Existing font hashes are in [dependencies.json](dependencies.json).

See [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md) for the current inventory.

## Cleanup validation

- Clean VastBuilder Debug build and Release build passed on Apple Silicon.
- Base64 standard vectors, binary data, permissive formatting, invalid-character
  behavior, and comparison with Python's codec passed for 77 binary inputs under
  UndefinedBehaviorSanitizer. The installed AddressSanitizer runtime aborted
  during startup before executing tests, so it was not used for this validation.
- Font loader/fallback/Unicode resizing/cache regression checks passed.
- An eight-second launch smoke test remained running with no font errors or
  missing/removed shader references. One preexisting OpenGL texture-binding
  diagnostic remains; this was not a complete interactive app test.
- Debug and Release bundles contain none of the removed Oculus shaders.
- Retained license hashes, documentation links, Xcode project syntax/object
  references, and removed-component source references passed verification.
- Gitleaks scanned the current tree, excluding private context/build output,
  and reported no findings.

After the asset/snippet changes, Debug/Release builds, UBSan quaternion/ray
checks, font regressions, and the 77 Base64 comparisons passed. Eight revised
GL3 fragments compiled, and GPU hue-picker/blur tests passed. The eight-second
builder launch had no shader or font failures; the existing GL diagnostic and
unused-output warnings remain. See the two review reports for the scope and
platform limitations.

The 2026-10-17 [source release verification](release-readiness.md) adds explicit
MIT terms for the owner's project recordings, clean-source verification, and
actual builder input/edit/save/reload coverage, including the Retina viewport
correction. Original project code changes do not add new third-party dependencies.

The VastBrowser macOS restoration uses those same audited SDK dependencies. Its
new piano tones are synthesized by original project code; local audio is chosen
by the user and is not bundled. All 261 retained asset hashes remain unchanged,
and the source secret scan reports no findings. The omitted iCracked model and
historical music remain omitted.
