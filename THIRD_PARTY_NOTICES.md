# Third-party notices

Original project code, the explicitly identified project sound effects, and
generated replacement artwork are MIT licensed. Third-party code, fonts,
artwork, and other assets retain their individual licenses. Existing third-party copyright and license
notices are retained. This inventory was updated on 2026-10-14 after replacing
Base64 and removing the legacy bundles identified in [the license audit](docs/license-audit.md).
The asset and snippet reviews are recorded in `docs/asset-review.md` and
`docs/math-snippet-review.md`. The Freesound-named click was removed at the
owner's direction; its callers use an existing project recording.

## Fonts

- **Carlito Regular and Bold**: Copyright 2013 The Carlito Project Authors, with
  Reserved Font Name "Carlito". SIL Open Font License 1.1. Unmodified font files
  from [Google Fonts](https://github.com/google/fonts/tree/66928797f271fed5ee428544e7229685e2d90127/ofl/carlito).
  The license is in `VastDevKit/lib/carlito/OFL.txt` and
  `VastApps/resources/fonts/Carlito-OFL.txt`. The engine embeds the unmodified
  Regular font bytes; the generated C header is also subject to that font license.
- **Font Awesome 4.2.0**: Copyright Dave Gandy 2014. SIL Open Font License 1.1;
  see `VastApps/resources/fonts/FontAwesome-OFL.txt`. The retained TTF matches
  [the upstream v4.2.0 font](https://github.com/FortAwesome/Font-Awesome/tree/v4.2.0)
  byte for byte. This notice concerns the font, not Font Awesome's separately
  licensed CSS or documentation.
- **Droid Sans** (Box2D testbed): Digitized data copyright 2007 Google Corporation.
  The font metadata specifies Apache License 2.0. See
  `VastDevKit/lib/Box2D/Box2D/Build/Data/DroidSans-LICENSE.txt`.

The Carlito and Font Awesome license texts are included in VastBuilder's bundled
resources. File hashes and source revisions are in `docs/dependencies.json`.

## Engine dependencies

| Component | License/notice location |
| --- | --- |
| Box2D | `VastDevKit/lib/Box2D/Box2D/License.txt` (zlib-style license) |
| difont | `VastDevKit/lib/difont/LICENSE` (MIT) |
| FreeType 2.4.4, bundled with difont | `VastDevKit/lib/difont/lib/freetype2/FTL.txt`; the FreeType Project License option is used. The reduced distribution is documented in its `README.md`. |
| libpng 1.2.44 | `VastDevKit/lib/libpng/LICENSE`; the notices in `png.h` prevail in case of discrepancy. |
| zlib 1.2.7 | License in `VastDevKit/lib/zlib-1.2.7/zlib.h` |
| JsonCpp | License in `VastDevKit/lib/jsoncpp/json/json.h` (public-domain/MIT terms) |
| Duktape 1.0.2 | `VastDevKit/lib/duktape-1.0.2/LICENSE.txt` (MIT) |
| GLEW 1.13.0 | `VastDevKit/lib/glew-1.13.0/LICENSE.txt` and source notices |
| iGLU (two copies) | `VastDevKit/lib/iGLU-1.0.0/README`, `VastDevKit/lib/difont/lib/iGLU-1.0.0/README`, and individual source notices (SGI Free Software License B 2.0 / MIT) |
| JavaScriptCore API headers | BSD-2-Clause copyright and permission notices in the individual headers; these are headers, not a bundled WebKit engine. |
| libuuid | `VastDevKit/lib/libuuid/COPYING`, individual source notices, and the restored `VastDevKit/lib/Documentation/licenses/COPYING.BSD-3` (BSD-3-Clause). |
| SDL2 | External Homebrew dependency; not vendored. Consult the installed package's license, including SDL2-compat/SDL3 when applicable. |

This software includes FreeType, copyright David Turner, Robert Wilhelm, and
Werner Lemberg. The software is based in part on the work of the FreeType Team.
Retain its license and required acknowledgments in distributions.

The zlib core license does not cover every optional contribution. DotZLib's
Boost 1.0 terms are in `contrib/dotzlib/LICENSE_1_0.txt`; preserve the other
contributions' individual notices. This snapshot omits the optional Ada binding;
core compression sources remain unchanged.

GLEW's runtime has BSD-style, Mesa MIT, and Khronos notices. Its separate
`config/config.guess` helper is GPL-3.0-or-later with a conditional Autoconf
distribution exception. The exception remains in the helper; complete GPLv3
text is in `docs/licenses/GPL-3.0.txt`. The GPLv2 document shipped in GLEW's
`doc/` directory does not change the runtime library's license. These documents
do not relicense the project's original MIT code.

## Third-party code embedded in the engine

| Component | License/notice location |
| --- | --- |
| John Chapman's OBJ parser adaptation | Existing copyright 2013 John Chapman and MIT declaration in `VastDevKit/src/graphics/mesh/MeshLoader.cpp`; complete MIT terms added in `docs/licenses/John-Chapman-MIT.txt`. |
| SoftSurfer ray/triangle adaptations | Complete unrestricted-use notice in both adaptations and `docs/licenses/softSurfer.txt`; copyright 2001 softSurfer / 2012 Dan Sunday. |
| Stefan Reinalter quaternion/vector multiplication | MIT source-code policy; copyright 2012–2017 Stefan Reinalter. `docs/licenses/Stefan-Reinalter-MIT.txt`. |
| Oolong-derived matrix routines | Restored zlib-style notice, copyright 2007–2008 Wolfgang Engel, in `math/Matrix.cpp` and `docs/licenses/Oolong-Math-zlib.txt`. The surviving adaptation is marked as modified; the Oolong bundle remains omitted. |
| VFP and NEON math | Zlib-style notices in `VastDevKit/src/math/ios/vfp/` and `math/ios/neon/`; credit Wolfgang Engel, Matthias Grundmann, and Justin Saunders as specified per file. |
| CocosDenshion | MIT notices crediting Steve Oldmeadow, 2010, in `VastDevKit/src/sound/ios/CocosDenshion/`; `CDOpenALSupport.h` and `.mm` separately retain Apple's sample-code terms. |
| Reachability | Apple's sample-code terms and Donoho Design Group's BSD-style extension notice in `VastDevKit/src/network/ios/Reachability.h` and `.m`. |

The Base64 codec is now original project code under the root MIT license. It
implements the standard encoding's byte grouping and retains the historical
permissive decoder API; it contains no copied Stack Overflow implementation.

## Restored texts and distribution

[License source records](docs/license-sources.json) identify the provenance and
hashes of retained restored texts, and distinguish a restored upstream file from
an added standard license template. Removed-component records are historical
provenance and are not notices for components currently distributed here.

Keep applicable copyright and complete permission/disclaimer texts with
redistributed source. Binary releases need the applicable notices, font
licenses, and FreeType acknowledgment delivered with the software; the present
development app is not a complete binary notice package.

## Demo assets and shaders

- The fresh Utah teapot OBJ/material/texture files come from Morgan McGuire's
  Computer Graphics Archive, under CC0 1.0. The source record is in
  `VastApps/resources/models/teapot/SOURCE.md`; complete terms are in
  `docs/licenses/CC0-1.0.txt`. Martin Newell is credited, with archive variant
  modifications by Kenzie Lamar. The old unverified JSON variant was removed.
- The two ambientObscurance fragment adaptations retain NVIDIA's complete
  two-condition BSD notice, copyright 2011–2012 NVIDIA, crediting Morgan McGuire
  and Michael Mara. See `docs/licenses/NVIDIA-SAO-BSD.txt` and their source headers.
- New menu/tool/launcher artwork and noise/wood/gradient textures are original
  MIT replacements generated by `scripts/generate-demo-assets.py`. Their list
  is in `docs/generated-assets.json`; the original borrowed artwork is omitted.
- Upstream documentation/test/sample artwork retains the applicable package
  notices, as individually recorded in `docs/assets.json`. Those records state
  when the evidence is package-level rather than an asset-specific grant.
- The owner confirmed that sound effects were made specifically for this
  project and authorized MIT licensing on 2026-10-17. All 53 audio files remain
  unchanged. Complete terms are in `VastApps/resources/sfx/LICENSE.txt` and
  `VastApps/PhysicsHotel/resources/SOUND_EFFECTS_LICENSE.txt`. See
  `docs/asset-review.md` for the source confirmation.

Unknown photographs/skyboxes, unnamed/rock models, Sibenik, the Freesound-named
click recording, and unverified unused shader samples were omitted. The active
unverified snippets were replaced with original implementations. Full review details and validation
limits are in the asset and math/snippet reports.

Oolong2 (including its nested components), the old PGMidi integration,
GPL-marked iOS glue, optional zlib Ada binding, Oculus sample NativeActivity
sources, and obsolete Oculus sample shaders were removed from this snapshot.
They do not require notices in its current source distribution. See the
license audit for the removal record and the intentionally omitted integrations.
