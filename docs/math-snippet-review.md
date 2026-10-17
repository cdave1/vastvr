# Math and shader snippet review

Reviewed 2026-10-14. The review follows explicit source references and compares
identifiable implementations with the original source/permission notices.
Mathematical formulas are distinguished from copied implementations. Source
records and hashes for restored notices are in [license-sources.json](license-sources.json).

## Retained implementations with recovered grants

| Implementation | Evidence and action |
| --- | --- |
| Both endpoint-based `RayIntersectsTriangle` adaptations | The [archived original author page](https://web.archive.org/web/20140701000000id_/http://geomalgorithms.com/a06-_intersect-2.html) contains matching `intersect3D_RayTriangle` code and an explicit unrestricted-use notice, copyright 2001 softSurfer / 2012 Dan Sunday. The full notice is now beside each adaptation in `Geometry.h` and `RayHitTester.cpp` and in [softSurfer.txt](licenses/softSurfer.txt). The [SPDX record](https://spdx.org/licenses/softSurfer.html) corroborates these terms. Intersection behavior and the component API's historical normal orientation are retained. The out-of-line public definition no longer incorrectly uses `inline`. |
| Quaternion/vector multiplication | [Stefan Reinalter's original article](https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/) matches the optimized routine. His [source-code policy](https://blog.molecular-matters.com/source-code-license/) explicitly licenses blog snippets under MIT, copyright 2012–2017 Stefan Reinalter. Attribution and a pointer to the [complete notice](licenses/Stefan-Reinalter-MIT.txt) were restored. The implementation is unchanged. |
| Matrix routines adapted from Oolong | The surviving `Matrix::Inverse` code and distinctive comments match `Math/Matrix.cpp` in the original imported Oolong snapshot. That specific source has Wolfgang Engel's 2007–2008 zlib-style grant. Its complete notice was restored to `Matrix.cpp` and [Oolong-Math-zlib.txt](licenses/Oolong-Math-zlib.txt), with the adaptation marked as modified. This recovers attribution for retained math without restoring the omitted Oolong bundle or asserting grants for its other components. |
| Ambient-obscurance shader | `tapLocation` and `sampleAO` match `SAO_AO.pix` in the authors' [NVIDIA-hosted demo source archive](https://research.nvidia.com/sites/default/files/pubs/2012-06_Scalable-Ambient-Obscurance/SAO-demo.zip), linked from [the research publication page](https://research.nvidia.com/publication/2012-06_scalable-ambient-obscurance). The source carries a two-condition BSD grant, copyright 2011–2012 NVIDIA, crediting Morgan McGuire and Michael Mara. The complete [source notice](licenses/NVIDIA-SAO-BSD.txt) is now in both ambientObscurance fragments. Project adaptations are identified. The publication's separate paper copyright is not substituted for the shader's source grant. |

## Original replacements and removals

| Previous code | Resolution and behavior |
| --- | --- |
| Lolengine-derived `Quaternion::FromTwoVectors` and copied reference block | Replaced with an original axis-angle construction. It normalizes finite inputs using double intermediates, selects a perpendicular axis for opposite directions, and returns identity for zero/nonfinite inputs. This also fixes the former `u[3]` access beyond the three-component vector and non-unit rotation results. |
| Wikipedia-derived Möller–Trumbore `RayHitsTriangle` | Replaced with an original plane intersection followed by edge-half-space tests. Both windings, edge/vertex hits, and the direction-parameter convention are retained. Backward/parallel/degenerate rays are rejected. The separate licensed SoftSurfer APIs remain. |
| Stack Overflow GUID-to-string snippets in both BuilderSerialization copies | Replaced with direct standard hexadecimal formatting of GUID fields. The non-Windows branch is unchanged. Windows projects remain unvalidated. |
| Galaxy/fractal demo shaders | Replaced all three galaxy fragments with original grid-based stars and a background gradient, preserving caller interfaces. Unused `work` variants and the retired shader directory were removed. Visual appearance changes. |
| Active hue-picker snippet credited to Shadertoy | Replaced with original interpolation across six RGB hue sectors. Opacity and normal outputs are preserved. |
| IQ-credited geometry edge fade | Replaced with an original bounded smooth border fade. Removed its unused copied counterpart from the texture fragment. |
| Forum-derived post-blur helpers | Replaced with original bounded box averaging; sizes are clamped to 1–16, with no division by zero for a zero-size uniform. The maintained shader still averages the shadow input. It now preserves a constant input instead of adding the center twice. The unused bilateral/gaussian code is absent. |
| Shader-demo `nrand3` and copied shadow sample table | Replaced helper copies with an original deterministic arithmetic mixer and generated ring/quadrant sample offsets. Shadow filtering texture changes are expected. GL3 world unprojection is expressed directly through inverse matrices and homogeneous division. |
| Unverified SSAO and archived tutorial fragments | Removed unused SSAO variants. PhysicsHotel's tutorial-derived lighting/deformation/outline helpers now use simple original color, texture, and scrolling-texture equivalents. The archived app remains unvalidated. |
| Unused copied reference blocks | Removed commented quaternion and matrix examples, including the implementation inside the already-empty `ToQuaternion` placeholder. This does not implement that placeholder. |

## Formula references

`Matrix.cpp`'s quaternion-to-matrix conversion expresses the standard quadratic
rotation equations described by [Euclideanspace](https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm).
Its determinant and `InverseLong` routines express the determinant/cofactor
identities referenced by the [Hiroshima mathematics page](http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html).
The latter page is currently unavailable to the review tools; no new license
grant is inferred from it. The local expressions were assessed as formulas,
with no reproduced article prose. Formula-reference links remain as attribution.

`Functions.h` implements the usual linear interpolation and cubic smoothstep
polynomials. The Microsoft reference in `Matrix.h` describes an orthographic
projection convention. These references explain mathematics/API conventions;
they are not treated as separate licenses for the project's implementations.
This is a source-provenance assessment, not a legal opinion on every possible
similar expression elsewhere.

The existing VFP/NEON math retains its complete per-file zlib-style notices.
The John Chapman OBJ parser retains the MIT attribution restored in the earlier
license audit. The original Base64 replacement remains independently tested.

## Validation

- Quaternion quarter turns, non-unit/zero/nonfinite/large inputs, opposite axes,
  and 100 arbitrary direction pairs pass alignment/unit-length checks.
- Ray tests pass for both windings, edges, vertices, misses, parallel/degenerate
  triangles, direction scaling, and both retained SoftSurfer APIs. The test
  compiles the ray implementation and quaternion headers with UBSan.
- Eight changed GL3 fragments compile on the Mac OpenGL driver. Rendered hue
  interpolation and constant-input blur tests pass, including sizes 0 and 100.
- SDK and builder Debug/Release builds pass; fonts and 77 independent Base64
  comparisons pass. Launch remains running for eight seconds with no shader
  or font failures, with the preexisting GL diagnostic still present.
- No validation claim is made for Windows UUID formatting or historical mobile,
  legacy GL, tool, or PhysicsHotel targets.

Run the focused checks after a Debug build:

```sh
./scripts/test-math-macos.sh
./scripts/test-shaders-macos.sh
```

The later builder integration check caught a separate adapter bug: the mesh and
polygon ray wrappers passed a direction to the endpoint-based SoftSurfer API.
They now pass origin + direction as the endpoint and use a three-axis bounding
radius. The licensed endpoint routines themselves remain unchanged. Actual
non-origin scene selection/erasure is covered by the builder integration check.
