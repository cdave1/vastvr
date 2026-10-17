# Asset provenance review

Reviewed 2026-10-14. [assets.json](assets.json) records paths, SHA-256 hashes,
source/license evidence, and changes for 261 retained asset files. The inventory
includes raster/vector artwork, fonts, audio, model materials, and shaders in
both imported projects. Dependency sources and embedded code are covered by the
[license audit](license-audit.md) and [snippet review](math-snippet-review.md).

## Project recordings

The owner confirmed on 2026-10-14: “All of the sound effects were all created
specifically for this project.” This confirms the origin of the `vast_menu_sesh*`,
`blop*`, `warp`, `undo`, and `blockSelect03` recordings. Their 53 files, including
converted copies and the archived PhysicsHotel copy, are retained unchanged.
No individual recording author or commission contract was supplied; the
inventory records the owner's confirmation without inventing that information.
On 2026-10-17, the owner explicitly authorized releasing these 53 audio files
under MIT. The complete grant is in [the sound-effect license](../VastApps/resources/sfx/LICENSE.txt),
with a local copy for [the archived PhysicsHotel recording](../VastApps/PhysicsHotel/resources/SOUND_EFFECTS_LICENSE.txt).

The additional `48610__nirmatara__click2-short.wav` sample was deleted at the
owner's direction on 2026-10-14. It had been identified by filename as
[nirmatara's Freesound recording](https://freesound.org/people/nirmatara/sounds/48610/),
with no separate permission established. All sixteen former callers in Builder,
DashboardMenu, Keyboard, and the archived 4x app now use the existing
owner-confirmed `resources/sfx/converted/blop_002.wav` effect. No new recording
was introduced. This closes the outstanding click-sample provenance question.

## Replacement artwork

Unidentified project icons and textures were replaced with 58 original images
produced by [generate-demo-assets.py](../scripts/generate-demo-assets.py).
The generator uses geometric shapes and deterministic random data, with no
imported pixels, photographs, logos, or external icon outlines. The output files
carry MIT/copyright/source metadata. [generated-assets.json](generated-assets.json)
identifies every generated image, including:

- Shared menu/tool icons, gradients, wood, and noise/normal textures.
- A Maps icon at the path the dashboard already expected.
- Foxtrot/Tango tool images, Android launcher/web icons, and original replacements
  for the Windows difont sample icons.
- The archived PhysicsHotel wood/noise textures.

To reproduce them from the repository root:

```sh
python3 scripts/generate-demo-assets.py
```

The visual style is deliberately simple. This changes the demo's appearance;
it does not reproduce the unidentified original art. Bad paint/noise resource
paths were corrected to point at their shared resource directories.

## Omitted photographs and models

Stock-style photos, the unidentified `PANO_20150108_111723` panorama, downloaded
skyboxes/character imagery, unnamed `models`/`untitled` models, and rock models
were removed from this public snapshot because no author or grant was supplied.
Their removal does not assert that the owner lacks rights: they can be restored
from the private originals if their provenance is later confirmed.

Sibenik was removed as well. The [graphics archive's metadata](https://casual-effects.com/g3d/data10/research/model/sibenik/info.js)
identifies its variant as CC BY-NC and names Marko Dabrovic, Kenzie Lamar, and
Morgan McGuire. No separate grant was established for the local variant.

The old teapot JSON/material/texture variant could not be verified. It was
replaced with the unmodified OBJ, material, and texture from Morgan McGuire's
[CC0 teapot archive](https://casual-effects.com/g3d/data10/common/model/teapot/info.js).
The archive credits Martin Newell, with the variant produced by Kenzie Lamar;
its update date is 2011-07-27. All three files match the freshly downloaded ZIP
byte for byte. See the model's [source record](../VastApps/resources/models/teapot/SOURCE.md)
and [complete CC0 terms](licenses/CC0-1.0.txt). The builder's existing empty model
selection remains empty; this does not enable automatic teapot loading.

## Shaders and vendor resources

The [snippet review](math-snippet-review.md) documents shader attribution and
original replacements. NVIDIA's ambient-obscurance implementation is retained
with its recovered BSD notice. Unverified retired SSAO/fractal samples were
removed; active borrowed blocks were replaced. Archived demo shaders now use
simple original equivalents.

Upstream Box2D documentation illustrations, libpng test/documentation images,
GLEW documentation branding, and difont shader sample resources remain with their
package notices. The inventory distinguishes that package-level evidence from
an individually recovered asset grant; no separate asset-specific terms were
found. Those upstream images were not relabeled as original MIT project art.
Carlito, Font Awesome, and Droid Sans retain their established font licenses.

## Recovery and verification

The original repositories were not changed. Private recovery material is under
`.context/`: the asset archive, additional original photos, and original snippet
copies. It is ignored and excluded from the public tree. `assets.json` records
old hashes for 69 removed assets, 58 replaced assets, and 16 modified shaders.
Notice-only files and other source changes are documented separately.

Debug and Release VastBuilder builds, math/font/Base64 regressions, GPU shader
checks, and an eight-second launch check passed on Apple Silicon. GPU tests
compile eight changed GL3 fragments and render color-picker interpolation and
blur boundary cases. The existing OpenGL texture-binding diagnostic and unused
shader-output warnings remain. Historical app/platform projects and changed
legacy GL/PhysicsHotel shaders are unvalidated. These are development builds,
not self-contained binary releases with a complete notice package.

A subsequent [source release check](release-readiness.md) on 2026-10-17
authorized explicit MIT terms for the project recordings and exercised editing,
colors, sound, persistence, and full-window Retina rendering in the builder.
