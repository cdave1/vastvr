#include "MeshLoader.h"
#include "json/json.h"

#include <Vast/VastCore.h>
#include <Vast/VastGraphics.h>
#include <difont/difont.h>

#include "graphics/GL/GraphicsInterface.h"
#include "core/filesystem/FileSystem.h"
#include "Mesh.h"
#include "MeshData.h"
#include "MeshMaterial.h"

static const uint32_t MISSING_MODEL_ID = 0;

hd::graphics::MeshLoader *s_meshLoaderInstance = NULL;

hd::graphics::MeshLoader * hd::graphics::MeshLoader::Instance() {
    if (!s_meshLoaderInstance) {
        s_meshLoaderInstance = new hd::graphics::MeshLoader();
    }
    return s_meshLoaderInstance;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetFontMesh(const hd::ui::Font *font,
                                                               const std::string text) {
    return FontMesh (font, text);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetFontMesh(const hd::ui::Font *font,
                                                               const hd::ui::FontLayout *layout,
                                                               const std::string text) {
    return FontMesh(font, layout, text);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetMeshFromFile(const std::string &path) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::File;
    prototype.meshFilePath = path;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetCylinder(const float radius,
                                                               const float length,
                                                               const int segments) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Cylinder;
    prototype.cylinderRadius = radius;
    prototype.cylinderLength = length;
    prototype.cylinderSegments = segments;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetQuad(const Vec3 &quad) {
    float x = (0.5f * quad[0]);
    float y = (0.5f * quad[1]);
    float z = (0.5f * quad[2]);
    AABB aabb(Vec3(-x, -y, -z), Vec3(x, y, z));

    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Quad;
    prototype.quadAABB = aabb;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetQuad(const AABB &aabb) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Quad;
    prototype.quadAABB = aabb;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetCube(const AABB &aabb) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Cube;
    prototype.cubeAABB = aabb;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetCube(const Size3f &cube) {
    float x = (0.5f * cube[0]);
    float y = (0.5f * cube[1]);
    float z = (0.5f * cube[2]);
    AABB aabb(Vec3(-x, -y, -z), Vec3(x, y, z));

    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Cube;
    prototype.cubeAABB = aabb;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetSphere(const float radius, const float segments) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::Sphere;
    prototype.sphereRadius = radius;
    prototype.sphereSegments = segments;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetInverseSphere(const float radius, const float segments) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::InverseSphere;
    prototype.sphereRadius = radius;
    prototype.sphereSegments = segments;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetSimpleCube(const Size3f &cube) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::SimpleCube;
    prototype.simpleCubeSize = cube;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetGapGrid(const Size3f &size, const Size3f &gap) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::GapGrid;
    prototype.gapGridSize = size;
    prototype.gapGridNodeSize = gap;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetRepeatingGrid(const AABB &aabb,
                                                                    const Size3f &cube,
                                                                    const Size3f &gap) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::RepeatingGrid;
    prototype.repeatingGridAABB = aabb;
    prototype.repeatingGridCubeSize = cube;
    prototype.repeatingGridGapSize = gap;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetBoxWithCurvedCorners(const Size3f &size,
                                                                           const float cornerRadius) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::BoxWithCurvedCorners;
    prototype.boxWithCurvedCornersSize = size;
    prototype.boxWithCurvedCornersCornerRadius = cornerRadius;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetCurvedQuad(const float curvedQuadArc,
                                                                 const float curvedQuadRadius,
                                                                 const float curvedQuadLength,
                                                                 const uint32_t curvedQuadSegments) {
    MeshPrototype prototype;
    prototype.meshMethod = MeshMethod::CurvedQuad;
    prototype.curvedQuadArc = curvedQuadArc;
    prototype.curvedQuadRadius = curvedQuadRadius;
    prototype.curvedQuadLength = curvedQuadLength;
    prototype.curvedQuadSegments = curvedQuadSegments;
    return FromMeshPrototype(prototype);
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GetMesh(const MeshPrototype &prototype) {
    return FromMeshPrototype(prototype);
}


bool hd::graphics::MeshLoader::DestroyMeshData(hd::graphics::MeshData *meshData) {
    DeleteSafely(meshData);
    return true;
}


#pragma mark - Private functions

hd::graphics::MeshLoader::MeshLoader() {
    m_meshSets = new hd::Vector<hd::graphics::MeshData *>();
}


hd::graphics::MeshLoader::~MeshLoader() {
}


hd::graphics::MeshData * hd::graphics::MeshLoader::FromMeshPrototype(const MeshPrototype &prototype) {
    hd::graphics::MeshData *meshData = nullptr;

    for (unsigned i = 0; i < m_meshSets->Size(); i++) {
        hd::graphics::MeshData *meshSet = m_meshSets->At(i);

        if (meshSet->GetPrototype() == prototype) {
            hd::log::i("Returned a meshset from cache with id: %d\n", meshSet->GetId());
            return meshSet;
        }
    }

    switch (prototype.meshMethod) {
        case MeshMethod::BoxWithCurvedCorners:
            meshData = BoxWithCurvedCorners(prototype.boxWithCurvedCornersSize, prototype.boxWithCurvedCornersCornerRadius);
            break;
        case MeshMethod::Cube:
            meshData = Cube(prototype.cubeAABB);
            break;
        case MeshMethod::Cylinder:
            meshData = Cylinder(prototype.cylinderRadius, prototype.cylinderLength, prototype.cylinderSegments);
            break;
        case MeshMethod::File:
            meshData = MeshFile(prototype.meshFilePath);
            break;
        case MeshMethod::Font:
            //meshData =
            break;
        case MeshMethod::Quad:
            meshData = Quad(prototype.quadAABB);
            break;
        case MeshMethod::RepeatingGrid:
            meshData = RepeatingGrid(prototype.repeatingGridAABB, prototype.repeatingGridCubeSize, prototype.repeatingGridGapSize);
            break;
        case MeshMethod::SimpleCube:
            meshData = SimpleCube(prototype.simpleCubeSize);
            break;
        case MeshMethod::Sphere:
            meshData = Sphere(prototype.sphereRadius, prototype.sphereSegments);
            break;
        case MeshMethod::GapGrid:
            meshData = GapGrid(prototype.gapGridSize, prototype.gapGridNodeSize);
            break;
        case MeshMethod::InverseSphere:
            meshData = Sphere(prototype.sphereRadius, prototype.sphereSegments);
            meshData->GetMeshes()->At(0)->FlipWinding();
            meshData->GetMeshes()->At(0)->GenerateNormals();
            break;
        case MeshMethod::CurvedQuad:
            meshData = CurvedQuad(prototype.curvedQuadArc,
                                  prototype.curvedQuadRadius,
                                  prototype.curvedQuadLength,
                                  prototype.curvedQuadSegments);
        case MeshMethod::NullMethod:
        default:
            break;
    }

    if (meshData) {
        meshData->GenerateExtents();

        m_meshSets->Add(meshData);
        meshData->SetPrototype(prototype);
        meshData->UploadMeshData();
        return meshData;
    } else {
        return nullptr;
    }
}


hd::graphics::MeshData * hd::graphics::MeshLoader::FontMesh(const hd::ui::Font *font,
                                                            const std::string text) {
    if (!font) {
        hd::log::e("Can't render a null font.\n");
        return NULL;
    }

    difont::Font *ftglFont = font->GetFont();

    if (ftglFont) {
        difont::FontMeshSet::Begin();
        ftglFont->Render(text.c_str());
    } else {
        return NULL;
    }

    // Get the FTFontMesh data here
    unsigned int fontMeshCount = difont::FontMeshSet::MeshCount();
    difont::FontMesh *meshes = difont::FontMeshSet::GetMeshes();

    if (fontMeshCount == 0) {
        return NULL;
    }

    unsigned int totalVertices = 0;
    unsigned int currentTextureId = 0;

    hd::graphics::MeshData *meshSet = new hd::graphics::MeshData(m_meshSets->GetSize());
    hd::graphics::Mesh *mesh = new hd::graphics::Mesh();
    meshSet->AddMesh(mesh);

    for (uint32_t i = 0; i < fontMeshCount; ++i) {
        difont::FontMesh fontMesh = meshes[i];
        unsigned int vertexCount = fontMesh.GetVertexCount();

		for (uint32_t j = 0; j < vertexCount; ++j) {
            difont::FontVertex fontVertex = fontMesh.vertices[j];

            Vertex vertex;
            vertex.origin.Set(fontVertex.position[0],
                              fontVertex.position[1],
                              fontVertex.position[2]);
            vertex.texCoord.Set(fontVertex.texCoord[0],
                                fontVertex.texCoord[1]);

            mesh->AddVertex(vertex);
        }

        if (fontMesh.primitive == GL_TRIANGLES) {
			for (uint32_t j = 0; j < vertexCount - 2; j += 3) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_TRIANGLE_STRIP) {
			for (uint32_t j = 0; j < (vertexCount - 2); j++) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_TRIANGLE_FAN) {
			for (uint32_t j = 0; j < (vertexCount - 2); j++) {
                int i1 = totalVertices;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_QUADS) {
			for (uint32_t j = 0; j < vertexCount - 3; j += 4) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);
                int i4 = totalVertices + (j + 3);

                Size4 quad(i1, i2, i3, i4);
                mesh->AddQuad(quad);
            }
        }
        totalVertices += vertexCount;
        /*
         if (fontMesh.textureId > 0 && fontMesh.textureId != currentTextureId) {
         currentTextureId = fontMesh.textureId;
         hd::graphics::MeshMaterial *material = new hd::graphics::MeshMaterial();
         material->SetColorTexture(fontMesh.textureId);
         mesh->SetMeshMaterial(material);
         } else {

         }*/
    }

    mesh->GenerateExtents();
    mesh->GenerateNormals();

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::FontMesh(const hd::ui::Font *font,
                                                            const hd::ui::FontLayout *layout,
                                                            const std::string text) {
    if (!font || !layout) {
        hd::log::e("Can't render a null font and layout.\n");
        return NULL;
    }

//    layout->Update();
    difont::Font *ftglFont = font->GetFont();
    difont::SimpleLayout *ftglLayout = const_cast<difont::SimpleLayout *>(layout->GetLayout());

    if (ftglFont && ftglLayout) {
        difont::FontMeshSet::Begin();
        ftglLayout->SetFont(ftglFont);
        ftglLayout->Render(text.c_str(), -1, difont::Point(), difont::RENDER_FRONT);
    } else {
        return NULL;
    }


    // Get the FTFontMesh data here
    int fontMeshCount = difont::FontMeshSet::MeshCount();
    difont::FontMesh *meshes = difont::FontMeshSet::GetMeshes();

    if (fontMeshCount == 0) {
        return NULL;
    }

    unsigned int totalVertices = 0;
    hd::graphics::MeshData *meshSet = new hd::graphics::MeshData(m_meshSets->GetSize());
    hd::graphics::Mesh *mesh = new hd::graphics::Mesh();
    meshSet->AddMesh(mesh);

	for (int i = 0; i < fontMeshCount; ++i) {
        difont::FontMesh fontMesh = meshes[i];
        unsigned int vertexCount = fontMesh.GetVertexCount();

		for (uint32_t j = 0; j < vertexCount; ++j) {
            difont::FontVertex fontVertex = fontMesh.vertices[j];

            Vertex vertex;
            vertex.origin.Set(fontVertex.position[0],
                              fontVertex.position[1],
                              fontVertex.position[2]);
            vertex.color.Set(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
            vertex.texCoord.Set(fontVertex.texCoord[0],
                                fontVertex.texCoord[1]);

            mesh->AddVertex(vertex);
        }

        if (fontMesh.primitive == GL_TRIANGLES) {
			for (uint32_t j = 0; j < vertexCount - 2; j += 3) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_TRIANGLE_STRIP) {
			for (uint32_t j = 0; j < (vertexCount - 2); j++) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_TRIANGLE_FAN) {
			for (uint32_t j = 0; j < (vertexCount - 2); j++) {
                int i1 = totalVertices;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);

                Size3 triangle(i1, i2, i3);
                mesh->AddTriangle(triangle);
            }
        } else if (fontMesh.primitive == GL_QUADS) {
			for (uint32_t j = 0; j < vertexCount - 3; j += 4) {
                int i1 = totalVertices + j;
                int i2 = totalVertices + (j + 1);
                int i3 = totalVertices + (j + 2);
                int i4 = totalVertices + (j + 3);

                Size4 quad(i1, i2, i3, i4);
                mesh->AddQuad(quad);
            }
        }
        totalVertices += vertexCount;
    }

    mesh->GenerateExtents();
    mesh->GenerateNormals();

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::MeshFile(const std::string &path) {
    if (path.empty()) {
        return NULL;
    }

    hd::graphics::MeshData *meshSet = NULL;
    std::string extension = path.substr(path.length() - 4, 4);
    if (extension == "json") {
        meshSet = LoadFromJson(path);
    } else {
        meshSet = LoadFromObj(path);
    }

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::Cylinder(const float radius,
                                                            const float length,
                                                            const int segments) {
    const float k_increment = 2.0f * hd_pi / segments;
    float theta = 0.0f;

    Mesh *circle = new(std::nothrow) Mesh();
    Mesh *side = new(std::nothrow) Mesh();

    Vertex c0, c1;
    c0.origin = Vec3(0, 0, -0.5f * length);
    c0.texCoord = Vec2(0.5f, 0.5f);

    c1.origin = Vec3(0, 0, 0.5f * length);
    c1.texCoord = Vec2(0.5f, 0.5f);

    circle->AddVertex(c0);
    circle->AddVertex(c1);

    for (int i = 0; i < segments; ++i) {
        Vertex v0, v1, v2, v3;

        // Front triangle
        v0.origin   = Vec3(radius * cosf(theta),
                           radius * sinf(theta),
                           -0.5f * length);
        v0.texCoord = Vec2(cosf(theta) * 0.5f + 0.5f,
                           1.0f - (sinf(theta) * 0.5f + 0.5f));

        v1.origin = Vec3(radius * cosf(theta + k_increment),
                         radius * sinf(theta + k_increment),
                         -0.5f * length);
        v1.texCoord = Vec2(cosf(theta + k_increment) * 0.5f + 0.5f,
                           1.0f - (sinf(theta + k_increment) * 0.5f + 0.5f));

        circle->AddVertex(v0);
        circle->AddVertex(v1);

        Size3 front(0, i * 4 + 2, i * 4 + 3);
        circle->AddTriangle(front);

        // Back triangle
        v2.origin = v0.origin + Vec3(0, 0, length);
        v2.texCoord = v0.texCoord;

        v3.origin = v1.origin + Vec3(0, 0, length);
        v3.texCoord = v1.texCoord;

        circle->AddVertex(v3);
        circle->AddVertex(v2);

        Size3 back(1, i * 4 + 4, i * 4 + 5);
        circle->AddTriangle(back);

        theta += k_increment;
    }

    theta = 0.0f;
    for (int i = 0; i < segments; ++i) {
        Vertex a, b;

        a.origin   = Vec3(radius * cosf(theta),
                          radius * sinf(theta),
                          -0.5f * length);
        a.texCoord = Vec2(cosf(theta) * 0.5f + 0.5f, 0.0f);

        b.origin = a.origin + Vec3(0, 0, length);
        b.texCoord = Vec2(cosf(theta) * 0.5f + 0.5f, 1.0f);

        side->AddVertex(a);
        side->AddVertex(b);

        Size4 quad((i * 2),
                   (i * 2 + 1),
                   (i * 2 + 3) % (segments * 2),
                   (i * 2 + 2) % (segments * 2));
        side->AddQuad(quad);

        theta -= k_increment;
    }

    circle->GenerateExtents();
    side->GenerateExtents();

//    circle->FlipWinding();
    side->FlipWinding();


    circle->GenerateNormals();
    //circle->FlipNormals();
    side->GenerateSmoothNormals();
/*
    circle->FlipWinding();
    circle->FlipNormals();
    side->FlipWinding();
    side->FlipNormals();
*/
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(circle);
    meshSet->AddMesh(side);

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::Quad(const AABB &aabb) {
    Mesh *quadMesh = new (std::nothrow) Mesh();
    Vertex t0, t1, t2, t3;

    t0.origin.Set(aabb.lower[0], aabb.lower[1], 0.0f);
    t1.origin.Set(aabb.upper[0], aabb.lower[1], 0.0f);
    t2.origin.Set(aabb.upper[0], aabb.upper[1], 0.0f);
    t3.origin.Set(aabb.lower[0], aabb.upper[1], 0.0f);

    t0.texCoord.Set(0, 0);
    t1.texCoord.Set(1, 0);
    t2.texCoord.Set(1, 1);
    t3.texCoord.Set(0, 1);

    quadMesh->AddVertex(t0);
    quadMesh->AddVertex(t1);
    quadMesh->AddVertex(t2);
    quadMesh->AddVertex(t3);

    quadMesh->AddQuad(Size4(0, 1, 2, 3));
    quadMesh->GenerateExtents();
    quadMesh->GenerateNormals();

    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(quadMesh);
    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::Cube(const AABB &aabb) {
    // b = bottom, t = top, s = south, w = west, e = east, n = north
    Mesh *cubeMesh = new (std::nothrow) Mesh();

    Vec3 bsw(aabb.lower[0], aabb.lower[1], aabb.lower[2]);
    Vec3 bse(aabb.upper[0], aabb.lower[1], aabb.lower[2]);
    Vec3 bne(aabb.upper[0], aabb.lower[1], aabb.upper[2]);
    Vec3 bnw(aabb.lower[0], aabb.lower[1], aabb.upper[2]);

    Vec3 tsw(aabb.lower[0], aabb.upper[1], aabb.lower[2]);
    Vec3 tse(aabb.upper[0], aabb.upper[1], aabb.lower[2]);
    Vec3 tne(aabb.upper[0], aabb.upper[1], aabb.upper[2]);
    Vec3 tnw(aabb.lower[0], aabb.upper[1], aabb.upper[2]);

    const int UNIQUE_VERTEX_COUNT = 24;
    Vec3 sides[UNIQUE_VERTEX_COUNT] = {
        tsw, tse, bse, bsw, // south
        tse, tne, bne, bse, // east
        tne, tnw, bnw, bne, // north
        tnw, tsw, bsw, bnw, // west
        tnw, tne, tse, tsw, // top
        bsw, bse, bne, bnw // bottom
    };

    for (int idx = 0; idx < UNIQUE_VERTEX_COUNT; idx += 4) {
        Vec3 s1 = sides[idx + 0];
        Vec3 s2 = sides[idx + 1];
        Vec3 s3 = sides[idx + 2];
        Vec3 s4 = sides[idx + 3];

        Vertex v1, v2, v3, v4;
        v1.origin.Set(s1);
        v2.origin.Set(s2);
        v3.origin.Set(s3);
        v4.origin.Set(s4);

        v1.texCoord.Set(0, 1);
        v2.texCoord.Set(1, 1);
        v3.texCoord.Set(1, 0);
        v4.texCoord.Set(0, 0);

        cubeMesh->AddVertex(v1);
        cubeMesh->AddVertex(v2);
        cubeMesh->AddVertex(v3);
        cubeMesh->AddVertex(v4);

        cubeMesh->AddQuad(Size4(idx + 0, idx + 1, idx + 2, idx + 3));
    }

    cubeMesh->GenerateExtents();
    cubeMesh->GenerateNormals();
    cubeMesh->FlipWinding();
    cubeMesh->FlipNormals();
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(cubeMesh);
    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::Sphere(const float radius, const float segments) {
    Mesh *mesh = new (std::nothrow) Mesh();

    const float TEXTURE_REPEAT = 1.0f;

    const unsigned int CIRCLE_SEGMENTS = 16;
    const unsigned int SPHERE_SEGMENTS = 32;

    const float circle_increment = hd_pi / CIRCLE_SEGMENTS;
    const float sphere_increment = hd_2_pi / SPHERE_SEGMENTS;

    const float circle_wrap_increment = TEXTURE_REPEAT / CIRCLE_SEGMENTS;
    const float sphere_wrap_increment = TEXTURE_REPEAT / SPHERE_SEGMENTS;

    float sphere_theta = 0.0f;
    float circle_theta = -hd_half_pi;

    float sphere_texture_wrap = 0.0f;

    Matrix rot1, rot2;
    unsigned idx = 0;

    for (int i = 0; i < SPHERE_SEGMENTS; ++i) {
        // Create our sphere rotations as rotations around the y (up) axis.
        Matrix::RotationXYZOrigin(rot1, Vec3(0.0f, sphere_theta, 0.0f));
        Matrix::RotationXYZOrigin(rot2, Vec3(0.0f, sphere_theta + sphere_increment, 0.0f));

        circle_theta = -hd_half_pi;
        float circle_texture_wrap = 0.0f;

        for (int j = 0; j < CIRCLE_SEGMENTS; ++j) {
            Vertex v1, v2, v;
            v.origin = radius * Vec3(cosf(circle_theta), sinf(circle_theta), 0);
            Matrix::Vec3Multiply(v1.origin, v.origin, rot1);

            v.origin = radius * Vec3(cosf(circle_theta + circle_increment), sinf(circle_theta + circle_increment), 0);
            Matrix::Vec3Multiply(v2.origin, v.origin, rot1);

            v1.texCoord.Set(1.0f - sphere_texture_wrap, TEXTURE_REPEAT - circle_texture_wrap);
            v2.texCoord.Set(1.0f - sphere_texture_wrap, TEXTURE_REPEAT - (circle_texture_wrap + circle_wrap_increment));

            mesh->AddVertex(v1);

            if (i < SPHERE_SEGMENTS) {
                Size4 quad;
                quad[0] = idx;
                quad[1] = (idx + 1);
                quad[2] = (idx + 1 + CIRCLE_SEGMENTS) % (SPHERE_SEGMENTS * (CIRCLE_SEGMENTS + 1));
                quad[3] = (idx + CIRCLE_SEGMENTS) % (SPHERE_SEGMENTS * (CIRCLE_SEGMENTS + 1));
                mesh->AddQuad(quad);
            }

            if (j == CIRCLE_SEGMENTS - 1) {
                mesh->AddVertex(v2);
                idx++;
            }

            idx++;

            circle_texture_wrap += circle_wrap_increment;
            circle_theta += circle_increment;
        }

        sphere_texture_wrap += sphere_wrap_increment;
        sphere_theta += sphere_increment;
    }

    mesh->GenerateExtents();
    mesh->GenerateNormals();
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(mesh);
    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::IcoSphere(const float radius) {
	return nullptr;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::SimpleCube(const Size3f &cube) {
    Mesh *cubeMesh = new (std::nothrow) Mesh();
    Vertex t0, t1, t2, t3, t4, t5, t6, t7;

    float x = (0.5f * cube[0]);
    float y = (0.5f * cube[1]);
    float z = (0.5f * cube[2]);

    t0.origin.Set(-x, -y, -z);
    t1.origin.Set(x,  -y, -z);
    t2.origin.Set(x,  -y, z);
    t3.origin.Set(-x, -y, z);

    t4.origin.Set(-x, y, -z);
    t5.origin.Set(x,  y, -z);
    t6.origin.Set(x,  y, z);
    t7.origin.Set(-x, y, z);

    cubeMesh->AddVertex(t0);
    cubeMesh->AddVertex(t1);
    cubeMesh->AddVertex(t2);
    cubeMesh->AddVertex(t3);
    cubeMesh->AddVertex(t4);
    cubeMesh->AddVertex(t5);
    cubeMesh->AddVertex(t6);
    cubeMesh->AddVertex(t7);

    unsigned idx = 0;

    cubeMesh->AddQuad(Size4(idx + 0, idx + 1, idx + 2, idx + 3));
    cubeMesh->AddQuad(Size4(idx + 4, idx + 5, idx + 6, idx + 7));
    cubeMesh->AddQuad(Size4(idx + 3, idx + 7, idx + 4, idx + 0));
    cubeMesh->AddQuad(Size4(idx + 0, idx + 4, idx + 5, idx + 1));
    cubeMesh->AddQuad(Size4(idx + 1, idx + 5, idx + 6, idx + 2));
    cubeMesh->AddQuad(Size4(idx + 2, idx + 6, idx + 7, idx + 3));

    cubeMesh->GenerateExtents();
    cubeMesh->GenerateNormals();
    cubeMesh->FlipWinding();
    cubeMesh->FlipNormals();
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(cubeMesh);
    return meshSet;
}


void hd::graphics::MeshLoader::AddGapGridNode(hd::graphics::Mesh *grid,
                                              const Size3f &gap,
                                              const Vec3 &center,
                                              uint32_t *idx) {
    const uint32_t VERTEX_COUNT = 6;
    Vertex v[VERTEX_COUNT];

    // TODO: Refactor me
    float x = center[0];
    float y = center[1];
    float z = center[2];

#if 0
    float len = gap[0];
    v[0].origin.Set(x, y, z);
    v[1].origin.Set(x + len, y, z);
    v[2].origin.Set(x      , y, z);
    v[3].origin.Set(x      , y + len, z);
    v[4].origin.Set(x      , y,       z);
    v[5].origin.Set(x      , y,       z + len);
#else
    float len = gap[0] * 0.05f;
    v[0].origin.Set(x - len, y, z);
    v[1].origin.Set(x + len, y, z);
    v[2].origin.Set(x      , y - len, z);
    v[3].origin.Set(x      , y + len, z);
    v[4].origin.Set(x      , y,       z - len);
    v[5].origin.Set(x      , y,       z + len);
#endif
    for (int i = 0; i < VERTEX_COUNT; ++i) {
        grid->AddVertex(v[i]);
    }

    Size2 xx(*idx + 0, *idx + 1);
    Size2 yy(*idx + 2, *idx + 3);
    Size2 zz(*idx + 4, *idx + 5);

    grid->AddLine(xx);
    grid->AddLine(yy);
    grid->AddLine(zz);

    *idx += VERTEX_COUNT;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::GapGrid(const Size3f &size, const Size3f &gap) {
    uint32_t idx = 0;

    if (gap[0] <= 0.0f || gap[1] <= 0.0f || gap[2] <= 0.0f) {
        return nullptr;
    }

    Mesh *grid = new (std::nothrow) Mesh();
    Vec3 offset = gap * 0.5f;
	Size3f nodes(size[0]/gap[0], size[1]/gap[1], size[2]/gap[2]);

    // Ensure the grid is centered on 0,0,0 with an offset
    for (uint32_t x = 0; x < nodes[0]; x++) {
		for (uint32_t y = 0; y < nodes[1]; y++) {
			for (uint32_t z = 0; z < nodes[2]; z++) {
                Vec3 center(x * gap[0] - ((nodes[0] / 2) * gap[0]),
                            y * gap[1] - ((nodes[1] / 2) * gap[1]),
                            z * gap[2] - ((nodes[2] / 2) * gap[2]));
                center += offset;
                AddGapGridNode(grid, gap, center, &idx);
            }
        }
    }

    grid->GenerateExtents();
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(grid);
    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::RepeatingGrid(const AABB &aabb,
                                                                 const Size3f &cube,
                                                                 const Size3f &gap) {
    int i = 0;
    Mesh *grid = new (std::nothrow) Mesh();

    for (float x = aabb.lower[0]; x < aabb.upper[0]; x += cube[0] + gap[0]) {
        for (float z = aabb.lower[2]; z < aabb.upper[2]; z += cube[2] + gap[2]) {
            Vertex t0, t1, t2, t3, t4, t5, t6, t7;

            float y = 0.0f + (0.5f * cube[1]);

            t0.origin.Set(x, y, z);
            t1.origin.Set(x + cube[0], y, z);
            t2.origin.Set(x + cube[0], y, z + cube[2]);
            t3.origin.Set(x          , y, z + cube[2]);

            t4.origin.Set(x, -y, z);
            t5.origin.Set(x + cube[0], -y, z);
            t6.origin.Set(x + cube[0], -y, z + cube[2]);
            t7.origin.Set(x          , -y, z + cube[2]);

            if (i % 2 == 0) {
                Color4f grey(0.8f, 0.8f, 0.8f, 1.0f);
                t0.color = t1.color = t2.color = t3.color = t4.color = t5.color = t6.color = t7.color = grey;
            }

            grid->AddVertex(t0);
            grid->AddVertex(t1);
            grid->AddVertex(t2);
            grid->AddVertex(t3);
            grid->AddVertex(t4);
            grid->AddVertex(t5);
            grid->AddVertex(t6);
            grid->AddVertex(t7);

            int idx = i * 8;

            grid->AddQuad(Size4(idx + 0, idx + 1, idx + 2, idx + 3));
            grid->AddQuad(Size4(idx + 4, idx + 5, idx + 6, idx + 7));
            grid->AddQuad(Size4(idx + 3, idx + 7, idx + 4, idx + 0));
            grid->AddQuad(Size4(idx + 0, idx + 4, idx + 5, idx + 1));
            grid->AddQuad(Size4(idx + 1, idx + 5, idx + 6, idx + 2));
            grid->AddQuad(Size4(idx + 2, idx + 6, idx + 7, idx + 3));

            i++;
        }
    }

    grid->GenerateExtents();
    grid->GenerateNormals();
    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(grid);
    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::BoxWithCurvedCorners(const Size3f &size,
                                                                        const float cornerRadius) {
    Mesh *front = new(std::nothrow) Mesh();
    Mesh *back = new(std::nothrow) Mesh();
    Mesh *edge = new(std::nothrow) Mesh();

    const float width = size[0] * 0.5f;
    const float height = size[1] * 0.5f;
    const float depth = size[2] * 0.5f;

    const unsigned int SEGMENTS = 9; // TODO: should be a function parm
    const unsigned int VERTEX_COUNT = 4 * (SEGMENTS + 1) + 1;
    const unsigned int EDGE_COUNT = (4 * SEGMENTS + 4);

    if (cornerRadius <= 0.0f) {
        front->AddVertex(Vertex(-width, -height));
        front->AddVertex(Vertex(width, -height));
        front->AddVertex(Vertex(width, height));
        front->AddVertex(Vertex(-width, height));
        front->AddVertex(Vertex(-width, -height));
    } else {
        Vertex frontVertex, backVertex;

        frontVertex.origin.Set(0.0f, 0.0f, depth);
        frontVertex.texCoord.Set(0.5f, 0.5f);

        backVertex.origin.Set(0.0f, 0.0f, -depth);
        backVertex.texCoord.Set(0.5f, 0.5f);

        front->AddVertex(frontVertex);
        back->AddVertex(backVertex);

        Vec2 BOTTOM_LEFT(-1, -1);
        Vec2 BOTTOM_RIGHT(1, -1);
        Vec2 TOP_RIGHT(1, 1);
        Vec2 TOP_LEFT(-1, 1);

        Vec2 CORNERS[4] = { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_RIGHT, TOP_LEFT };

        for (unsigned k = 0; k < 4; ++k) {
            Vec2 corner = CORNERS[k];
            for (size_t i = 0; i < (SEGMENTS+1); ++i) {
                Vertex frontVertex, backVertex;

                float rads = (float(i) / float(SEGMENTS) * hd_half_pi);
                float x, y;

                if (corner == BOTTOM_LEFT || corner == TOP_RIGHT) {
                    x = (corner[0] * (cornerRadius * cos(rads))) + (corner[0] * (width - cornerRadius));
                    y = (corner[1] * (cornerRadius * sin(rads))) + (corner[1] * (height - cornerRadius));
                } else {
                    x = (corner[0] * (cornerRadius * sin(rads))) + (corner[0] * (width - cornerRadius));
                    y = (corner[1] * (cornerRadius * cos(rads))) + (corner[1] * (height - cornerRadius));
                }

                Vec2 texcoord((x + width) / size[0], (y + height) / size[1]);

                frontVertex.origin.Set(x, y, depth);
                frontVertex.texCoord.Set(texcoord);

                backVertex.origin.Set(x, y, -depth);
                backVertex.texCoord.Set(texcoord);

                size_t idx_1 = (i + 1) + (k * (1 + SEGMENTS));
                size_t idx_2 = hd::max((idx_1 + 1) % VERTEX_COUNT, size_t(1));

                Size3 frontTriangle(0, idx_1, idx_2);
                front->AddVertex(frontVertex);
                front->AddTriangle(frontTriangle);

                Size3 backTriangle(0, idx_1, idx_2);
                back->AddVertex(backVertex);
                back->AddTriangle(backTriangle);

                edge->AddVertex(frontVertex);
                edge->AddVertex(backVertex);

                idx_1 = i + (k * (1 + SEGMENTS));

                Size4 quad((idx_1 * 2),
                           (idx_1 * 2 + 1),
                           (idx_1 * 2 + 3) % (2 * EDGE_COUNT),
                           (idx_1 * 2 + 2) % (2 * EDGE_COUNT));
                edge->AddQuad(quad);
            }
        }
    }

    front->GenerateExtents();
    back->GenerateExtents();
    edge->GenerateExtents();

    front->FlipWinding();
    back->FlipWinding();
    
    front->GenerateNormals();
//    front->FlipNormals();
    back->GenerateNormals();
//    back->FlipNormals();
    edge->GenerateSmoothNormals();

    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(front);
    meshSet->AddMesh(back);
    meshSet->AddMesh(edge);

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::CurvedQuad(const float curvedQuadArc,
                                                              const float curvedQuadRadius,
                                                              const float curvedQuadLength,
                                                              const uint32_t curvedQuadSegments) {
    const float k_increment = curvedQuadArc / curvedQuadSegments;
    float theta = 0.0f;

    Mesh *curvedQuad = new(std::nothrow) Mesh();

    Vertex c0, c1;
    c0.origin = Vec3(0, 0, -0.5f * curvedQuadLength);
    c0.texCoord = Vec2(0.5f, 0.5f);

    c1.origin = Vec3(0, 0, 0.5f * curvedQuadLength);
    c1.texCoord = Vec2(0.5f, 0.5f);

    theta = 0.0f;
	for (uint32_t i = 0; i < curvedQuadSegments; ++i) {
        Vertex a, b;

        a.origin   = Vec3(curvedQuadRadius * cosf(theta),
                          curvedQuadRadius * sinf(theta),
                          -0.5f * curvedQuadLength);
        a.texCoord = Vec2(cosf(theta) * 0.5f + 0.5f, 0.0f);

        b.origin = a.origin + Vec3(0, 0, curvedQuadLength);
        b.texCoord = Vec2(cosf(theta) * 0.5f + 0.5f, 1.0f);

        curvedQuad->AddVertex(a);
        curvedQuad->AddVertex(b);

        if (i < curvedQuadSegments - 1) {
            Size4 quad((i * 2),
                       (i * 2 + 1),
                       (i * 2 + 3) % (curvedQuadSegments * 2),
                       (i * 2 + 2) % (curvedQuadSegments * 2));
            curvedQuad->AddQuad(quad);
        }

        theta -= k_increment;
    }

    curvedQuad->GenerateExtents();
    curvedQuad->GenerateSmoothNormals();
    curvedQuad->FlipNormals();

    MeshData *meshSet = new MeshData(m_meshSets->Size());
    meshSet->AddMesh(curvedQuad);
    
    return meshSet;
}


inline bool nextLine(const char *buffer, size_t *i, size_t limit) {
    assert(buffer);

    do {
        if (*i == limit)
            return false;
        ++(*i);
    } while (buffer[*i] != '\n');
    ++(*i);

    return true;
}


// Obj parser via John Chapman
//
// /*******************************************************************************
// Copyright (C) 2013 John Chapman
//
// This software is distributed freely under the terms of the MIT License.
// See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
// *******************************************************************************/
hd::graphics::MeshData * hd::graphics::MeshLoader::LoadFromObj(const std::string &path) {
    hd::log::i("Loading mesh from \"%s\"...", path.c_str());

    hd::File *modelFile = NULL;
    if (NULL == (modelFile = hd::FileSystem::OpenFile(path.c_str()))) {
        hd::log::i("[ShaderProgram]: Could not open (%s) for reading. Couldn't find file\n", path.c_str());
        return NULL;
    }

    char *fileBuffer = NULL;
    int fileLength = hd::FileSystem::GetFileSize(modelFile);
    if (NULL == (fileBuffer = (char *)calloc(1, sizeof(char) * fileLength))) {
        hd::log::e("CALLOC!\n");
        return NULL;
    }

    hd::FileSystem::ReadFile(fileBuffer, fileLength, 1, modelFile);
    hd::FileSystem::CloseFile(modelFile);

    //	allocate buffers for vertices, texcoords, normals:
    Vec3 *vertexBuffer = 0, *normalBuffer = 0;
    Vec2 *texcoordBuffer = 0;
    size_t vertexBufferSize = 0, normalBufferSize = 0, texcoordBufferSize = 0;

    size_t i = 0;
    do {
        switch (fileBuffer[i]) {
            case 'V': // vertex data
            case 'v':
                switch (fileBuffer[i + 1]) {
                    case ' ': // vertex position
                        ++vertexBufferSize;
                        break;

                    case 'N': // vertex normal
                    case 'n':
                        ++normalBufferSize;
                        break;

                    case 'T': // vertex texcoord
                    case 't':
                        ++texcoordBufferSize;
                        break;

                    default:
                        break;
                };
                break;

            default:
                break;
        };
    } while (nextLine(fileBuffer, &i, fileLength));

    vertexBuffer = new(std::nothrow) Vec3[vertexBufferSize];
    assert(vertexBuffer);
    normalBuffer = new(std::nothrow) Vec3[normalBufferSize];
    assert(normalBuffer);
    texcoordBuffer = new(std::nothrow) Vec2[texcoordBufferSize];

    hd::log::i("\t%u vertices, %u normals, %u texcoords", vertexBufferSize, normalBufferSize, texcoordBufferSize);

    //	parse/process data:
    Mesh *result = new(std::nothrow) Mesh();
    assert(result);

    //	tmp list for expanding smoothing groups, used to prevent duplicate vertices
    //	within a smoothing group. realIndex is used to track the actual vertex
    //	index which may be different to what's in the OBJ file!
    struct TmpVertexT { size_t smoothGroup; int vt; int realIndex; };
    TmpVertexT *tmpVertexList = new(std::nothrow) TmpVertexT[vertexBufferSize];
    assert(tmpVertexList);
    for (size_t i = 0; i < vertexBufferSize; ++i) {
        tmpVertexList[i].smoothGroup = 0;
        tmpVertexList[i].vt = 0;
        tmpVertexList[i].realIndex = i;
    }

    i = 0; vertexBufferSize = 0; normalBufferSize = 0; texcoordBufferSize = 0;
    size_t thisSmoothGroup = 0;

    char linebuf[128];

    do {
#ifdef WIN32
        strncpy_s(linebuf, &fileBuffer[i], 128);
#else
        strncpy(linebuf, &fileBuffer[i], 128);
#endif
        switch (linebuf[0]) {
            case 'V': // vertex data
            case 'v':
                switch (linebuf[1]) {
                    case ' ': { // vertex position
                        float x, y, z;
                        if (sscanf(&linebuf[2], "%f %f %f", &x, &y, &z) != 3) {
                            hd::log::e("ERROR READING OBJ FILE\n");
                            return 0;
                        }
                        //	flip z, we want the model to be in the correct left-handed space
                        z = -z;
                        vertexBuffer[vertexBufferSize] = Vec3(x, y, z);
                        ++vertexBufferSize;
                        break;
                    }

                    case 'N': // vertex normal
                    case 'n': {
                        float x, y, z;
                        if (sscanf(&linebuf[2], "%f %f %f", &x, &y, &z) != 3) {
                            hd::log::e("ERROR READING OBJ FILE\n");
                            return 0;
                        }
                        //	flip normal z, too!
                        z = -z;
                        normalBuffer[normalBufferSize] = Vec3(x, y, z);
                        ++normalBufferSize;
                        break;
                    }

                    case 'T': // vertex texcoord
                    case 't':{
                        float u, v;
                        if (sscanf(&linebuf[2], "%f %f", &u, &v) != 2) {
                            hd::log::e("ERROR READING OBJ FILE\n");
                            return 0;
                        }

                        texcoordBuffer[texcoordBufferSize] = Vec2(u, v);
                        ++texcoordBufferSize;
                        break;
                    }

                    default:
                        break;
                };
                break;

            case 'S': // smoothing group
            case 's':
                ++thisSmoothGroup;
                break;

            case 'F': // face
            case 'f': {
                assert(vertexBufferSize); // must at least have some vertices

                const bool checkFaceForQuad = true;

                if (checkFaceForQuad) {

                    struct OBJVertexT { uint32_t v, vt, vn; };
                    OBJVertexT tmpFace[4];
                    OBJVertexT &a = tmpFace[0];
                    OBJVertexT &b = tmpFace[1];
                    OBJVertexT &c = tmpFace[2];
                    OBJVertexT &d = tmpFace[3];

                    if (sscanf(&linebuf[2], "%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u", &a.v, &a.vt, &a.vn, &b.v, &b.vt, &b.vn, &c.v, &c.vt, &c.vn, &d.v, &d.vt, &d.vn) == 12
                        || sscanf(&linebuf[2], "%u//%u %u//%u %u//%u %u//%u", &a.v, &a.vn, &b.v, &b.vn, &c.v, &c.vn, &d.v, &d.vn) == 8
                        || sscanf(&linebuf[2], "%u/%u %u/%u %u/%u %u/%u", &a.v, &a.vt, &b.v, &b.vt, &c.v, &c.vt, &d.v, &d.vt) == 8
                        || sscanf(&linebuf[2], "%u %u %u %u", &a.v, &b.v, &c.v, &d.v) == 4) {

                        // It's a quad face.
                        Size4 quad;
                        for (size_t j = 0; j < 4; ++j) {
                            //	correct indices:
                            --tmpFace[j].v;
                            --tmpFace[j].vt;
                            --tmpFace[j].vn;

                            if (tmpVertexList[tmpFace[j].v].smoothGroup == thisSmoothGroup
                                && tmpVertexList[tmpFace[j].v].vt == tmpFace[j].vt) {
                                quad[j] = tmpVertexList[tmpFace[j].v].realIndex;
                            } else {
                                tmpVertexList[tmpFace[j].v].smoothGroup = thisSmoothGroup;
                                tmpVertexList[tmpFace[j].v].vt = tmpFace[j].vt;
                                tmpVertexList[tmpFace[j].v].realIndex = result->CountVertices();
                                quad[j] = result->CountVertices();

                                Vertex ver;
                                ver.origin = vertexBuffer[tmpFace[j].v];
                                if (texcoordBufferSize) {
                                    ver.texCoord = texcoordBuffer[tmpFace[j].vt];
                                }

                                if (normalBufferSize) {
                                    ver.normal = normalBuffer[tmpFace[j].vn];
                                }

                                result->AddVertex(ver);

                            }
                        }

                        result->AddQuad(quad);

                    } else if (sscanf(&linebuf[2], "%u/%u/%u %u/%u/%u %u/%u/%u", &a.v, &a.vt, &a.vn, &b.v, &b.vt, &b.vn, &c.v, &c.vt, &c.vn) == 9
                               || sscanf(&linebuf[2], "%u//%u %u//%u %u//%u", &a.v, &a.vn, &b.v, &b.vn, &c.v, &c.vn) == 6
                               || sscanf(&linebuf[2], "%u/%u %u/%u %u/%u", &a.v, &a.vt, &b.v, &b.vt, &c.v, &c.vt) == 6
                               || sscanf(&linebuf[2], "%u %u %u", &a.v, &b.v, &c.v) == 3) {

                        // It's a triangle face.
                        Size3 tri;
                        for (size_t j = 0; j < 3; ++j) {
                            //	correct indices:
                            --tmpFace[j].v;
                            --tmpFace[j].vt;
                            --tmpFace[j].vn;

                            if (tmpVertexList[tmpFace[j].v].smoothGroup == thisSmoothGroup
                                && tmpVertexList[tmpFace[j].v].vt == tmpFace[j].vt) {
                                tri[j] = tmpVertexList[tmpFace[j].v].realIndex;
                            } else {
                                tmpVertexList[tmpFace[j].v].smoothGroup = thisSmoothGroup;
                                tmpVertexList[tmpFace[j].v].vt = tmpFace[j].vt;
                                tmpVertexList[tmpFace[j].v].realIndex = result->CountVertices();
                                tri[j] = result->CountVertices();

                                Vertex ver;
                                ver.origin = vertexBuffer[tmpFace[j].v];
                                if (texcoordBufferSize) {
                                    ver.texCoord = texcoordBuffer[tmpFace[j].vt];
                                }

                                if (normalBufferSize) {
                                    ver.normal = normalBuffer[tmpFace[j].vn];
                                }

                                result->AddVertex(ver);

                            }
                        }

                        result->AddTriangle(tri);
                    }

                } else {

                    //	read in indices:
                    struct OBJVertexT { uint32_t v, vt, vn; };
                    OBJVertexT tmpTri[3];
                    OBJVertexT &a = tmpTri[0];
                    OBJVertexT &b = tmpTri[1];
                    OBJVertexT &c = tmpTri[2];
                    OBJVertexT &d = tmpTri[3];

                    if (sscanf(&linebuf[2], "%u/%u/%u %u/%u/%u %u/%u/%u", &a.v, &a.vt, &a.vn, &b.v, &b.vt, &b.vn, &c.v, &c.vt, &c.vn) != 9)
                        if (sscanf(&linebuf[2], "%u//%u %u//%u %u//%u", &a.v, &a.vn, &b.v, &b.vn, &c.v, &c.vn) != 6)
                            if (sscanf(&linebuf[2], "%u/%u %u/%u %u/%u", &a.v, &a.vt, &b.v, &b.vt, &c.v, &c.vt) != 6)
                                if (sscanf(&linebuf[2], "%u %u %u", &a.v, &b.v, &c.v) != 3) {
                                    //	error, unsupported polygon type
                                    return 0;
                                }

                    //	construct final triangle:
                    Size3 tri;
                    for (size_t j = 0; j < 3; ++j) {
                        //	correct indices:
                        --tmpTri[j].v;
                        --tmpTri[j].vt;
                        --tmpTri[j].vn;

                        if (tmpVertexList[tmpTri[j].v].smoothGroup == thisSmoothGroup
                            && tmpVertexList[tmpTri[j].v].vt == tmpTri[j].vt) {
                            tri[j] = tmpVertexList[tmpTri[j].v].realIndex;
                        } else {
                            tmpVertexList[tmpTri[j].v].smoothGroup = thisSmoothGroup;
                            tmpVertexList[tmpTri[j].v].vt = tmpTri[j].vt;
                            tmpVertexList[tmpTri[j].v].realIndex = result->CountVertices();
                            tri[j] = result->CountVertices();

                            Vertex ver;
                            ver.origin = vertexBuffer[tmpTri[j].v];
                            if (texcoordBufferSize) {
                                ver.texCoord = texcoordBuffer[tmpTri[j].vt];
                            }

                            if (normalBufferSize) {
                                ver.normal = normalBuffer[tmpTri[j].vn];
                            }

                            result->AddVertex(ver);

                        }
                    }

                    result->AddTriangle(tri);
                }

                break;
            }

            default:
                break;
        };
    } while (nextLine(fileBuffer, &i, fileLength));

    delete[] tmpVertexList;
    delete[] vertexBuffer;
    delete[] normalBuffer;
    delete[] texcoordBuffer;
    hdFreeSafely(fileBuffer);

    //result->vertexMask_ = mask;
    //if (mask & TANGENTS) {
    //result->GenerateTangents();
    //}
    if (normalBufferSize == 0) {
        result->GenerateSmoothNormals();
    }

    result->GenerateExtents();

    hd::log::i("\tGenerated %u vertices, %u triangles, %u quads", result->CountVertices(), result->CountTriangles(), result->CountQuads());
    hd::log::i("...DONE");

    hd::graphics::MeshData *meshSet = new hd::graphics::MeshData(m_meshSets->GetSize());
    meshSet->AddMesh(result);

    return meshSet;
}


hd::graphics::MeshData * hd::graphics::MeshLoader::LoadFromJson(const std::string &path) {
    hd::log::i("Loading mesh from \"%s\"...", path.c_str());

    hd::File *modelFile = NULL;
    if (NULL == (modelFile = hd::FileSystem::OpenFile(path.c_str()))) {
        hd::log::i("[ShaderProgram]: Could not open (%s) for reading. Couldn't find file\n", path.c_str());
        return NULL;
    }

    char *fileBuffer = NULL;
    int fileLength = hd::FileSystem::GetFileSize(modelFile);
    if (NULL == (fileBuffer = (char *)calloc(1, sizeof(char) * fileLength))) {
        hd::log::e("CALLOC!\n");
        return NULL;
    }

    hd::FileSystem::ReadFile(fileBuffer, fileLength, 1, modelFile);
    hd::FileSystem::CloseFile(modelFile);

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(fileBuffer, fileBuffer + ((sizeof(char) * fileLength) - 1), root)) {
        hd::log::e("Error reading json!\n");
        hdFreeSafely(fileBuffer);
        return NULL;
    }

    hdFreeSafely(fileBuffer);

    const std::string ASSIMP_ROOT_NODE = "rootnode";
    const std::string ASSIMP_NAME = "name";
    const std::string ASSIMP_CHILDREN_ARRAY = "children";
    const std::string ASSIMP_MESH_NAME = "name";
    const std::string ASSIMP_TRANSFORMATION_ARRAY = "transform";
    const std::string ASSIMP_MESH_INDEX = "meshes";

    const std::string ASSIMP_FLAGS = "flags";
    const std::string ASSIMP_MESHES_ARRAY = "meshes";
    const std::string ASSIMP_MESH_MATERIAL_INDEX = "materialindex";
    const std::string ASSIMP_MESH_PRIMITIVE_TYPES = "primitivetypes";
    const std::string ASSIMP_MESH_VERTICES_ARRAY = "vertices";
    const std::string ASSIMP_MESH_NORMALS_ARRAY = "normals";
    const std::string ASSIMP_MESH_TANGENTS_ARRAY = "tangents";
    const std::string ASSIMP_MESH_NUM_UV_COMPONENTS = "numuvcomponents";
    const std::string ASSIMP_MESH_TEXTURE_COORDS = "texturecoords";
    const std::string ASSIMP_MESH_FACES_ARRAY = "faces";

    const std::string ASSIMP_MATERIALS_ARRAY = "materials";
    const std::string ASSIMP_MATERIAL_PROPERTIES = "properties";
    const std::string ASSIMP_MATERIAL_KEY = "key";
    const std::string ASSIMP_MATERIAL_SEMANTIC = "semantic";
    const std::string ASSIMP_MATERIAL_INDEX = "index";
    const std::string ASSIMP_MATERIAL_TYPE = "type";
    const std::string ASSIMP_MATERIAL_VALUE = "value";

    const std::string ASSIMP_MATERIAL_PROPERTY_NAME = "?mat.name";
    const std::string ASSIMP_MATERIAL_PROPERTY_SHADINGM = "$mat.shadingm";
    const std::string ASSIMP_MATERIAL_PROPERTY_AMBIENT = "$clr.ambient";
    const std::string ASSIMP_MATERIAL_PROPERTY_DIFFUSE = "$clr.diffuse";
    const std::string ASSIMP_MATERIAL_PROPERTY_SPECULAR = "$clr.specular";
    const std::string ASSIMP_MATERIAL_PROPERTY_SHININESS = "$mat.shininess";
    const std::string ASSIMP_MATERIAL_PROPERTY_OPACITY = "$mat.opacity";
    const std::string ASSIMP_MATERIAL_PROPERTY_REFRACTIVE = "$mat.refracti";
    const std::string ASSIMP_MATERIAL_PROPERTY_FILE = "$tex.file";

    if (root.isObject()
        && root.isMember(ASSIMP_MATERIALS_ARRAY)
        && root[ASSIMP_MATERIALS_ARRAY].isArray()) {

        hd::Vector<hd::graphics::MeshMaterial *> materials;
        Json::Value materialsJson = root[ASSIMP_MATERIALS_ARRAY];

        Json::ArrayIndex materialIndex = 0;
        Json::ArrayIndex materialArraySize = materialsJson.size();

        while (materialIndex < materialArraySize) {
            Json::Value material = materialsJson[materialIndex];
            hd::graphics::MeshMaterial *meshMaterial = new hd::graphics::MeshMaterial();

            if (material.isObject()
                && material.isMember(ASSIMP_MATERIAL_PROPERTIES)
                && material[ASSIMP_MATERIAL_PROPERTIES].isArray()) {

                Json::Value properties = material[ASSIMP_MATERIAL_PROPERTIES];

                Json::ArrayIndex propertyIndex = 0;
                Json::ArrayIndex propertyArraySize = properties.size();

                while (propertyIndex < propertyArraySize) {
                    Json::Value property = properties[propertyIndex];

                    if (property.isMember(ASSIMP_MATERIAL_KEY)
                        && property.isMember(ASSIMP_MATERIAL_SEMANTIC)
                        && property.isMember(ASSIMP_MATERIAL_INDEX)
                        && property.isMember(ASSIMP_MATERIAL_TYPE)
                        && property.isMember(ASSIMP_MATERIAL_VALUE)) {

                        std::string key = property[ASSIMP_MATERIAL_KEY].asString();
                        uint32_t semantic = property[ASSIMP_MATERIAL_SEMANTIC].asUInt();
                        uint32_t type = property[ASSIMP_MATERIAL_TYPE].asUInt();
                        Json::Value value = property[ASSIMP_MATERIAL_VALUE];

                        if (key == ASSIMP_MATERIAL_PROPERTY_NAME) {
                            std::string name = value.asString();
                            meshMaterial->SetName(name);
                        } else if (key == ASSIMP_MATERIAL_PROPERTY_SHADINGM) {

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_AMBIENT) {
                            float r = value[0].asFloat();
                            float g = value[1].asFloat();
                            float b = value[2].asFloat();
                            meshMaterial->SetAmbient(Color4f(r, g, b, 1.0f));

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_DIFFUSE) {
                            float r = value[0].asFloat();
                            float g = value[1].asFloat();
                            float b = value[2].asFloat();
                            meshMaterial->SetDiffuse(Color4f(r, g, b, 1.0f));

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_SPECULAR) {
                            float r = value[0].asFloat();
                            float g = value[1].asFloat();
                            float b = value[2].asFloat();
                            meshMaterial->SetSpecular(Color4f(r, g, b, 1.0f));

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_SHININESS) {
                            float shininess = value.asFloat();
                            meshMaterial->SetShininess(shininess);

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_OPACITY) {

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_REFRACTIVE) {

                        } else if (key == ASSIMP_MATERIAL_PROPERTY_FILE) {
                            std::string texture = value.asString();
                            if (semantic == 3) {
                                hd::Texture *tex = hd::TextureManager::Instance()->FindTexture(texture.c_str());
                                meshMaterial->SetColorTexture(tex);
                            }
                            if (semantic == 5) {
                                hd::Texture *tex = hd::TextureManager::Instance()->FindTexture(texture.c_str());
                                meshMaterial->SetNormalTexture(tex);
                            }
                            if (semantic == 8) {
                                hd::Texture *tex = hd::TextureManager::Instance()->FindTexture(texture.c_str());
                                meshMaterial->SetAlphaMaskTexture(tex);
                            }
                        }
                    }

                    ++propertyIndex;
                }

                materials.Add(meshMaterial);
            }

            ++materialIndex;
        }

        if (root.isMember(ASSIMP_ROOT_NODE)
            && root[ASSIMP_ROOT_NODE].isArray()
            && root[ASSIMP_ROOT_NODE].isMember(ASSIMP_CHILDREN_ARRAY)
            && root[ASSIMP_ROOT_NODE][ASSIMP_CHILDREN_ARRAY].isArray()) {

            Json::Value children = root[ASSIMP_ROOT_NODE][ASSIMP_CHILDREN_ARRAY];

            Json::ArrayIndex childIndex = 0;
            Json::ArrayIndex childArraySize = children.size();
            while (childIndex < childArraySize) {
                Json::Value child = children[childIndex];
                if (child.isMember(ASSIMP_MESH_NAME)
                    && child.isMember(ASSIMP_TRANSFORMATION_ARRAY)
                    && child.isMember(ASSIMP_MESH_INDEX)) {

                    Json::Value meshName = child[ASSIMP_MESH_NAME];
                    Json::Value transformation = child[ASSIMP_TRANSFORMATION_ARRAY];
                    Json::Value meshIndex = child[ASSIMP_MESH_INDEX];
                }
                ++childIndex;
            }
        }

        hd::graphics::MeshData *meshSet = new hd::graphics::MeshData(m_meshSets->GetSize());

        if (root.isMember(ASSIMP_MESHES_ARRAY)
            && root[ASSIMP_MESHES_ARRAY].isArray()) {

            Json::Value children = root[ASSIMP_CHILDREN_ARRAY];
            Json::Value meshes = root[ASSIMP_MESHES_ARRAY];

            Json::ArrayIndex meshIndex = 0;
            Json::ArrayIndex meshArraySize = meshes.size();

            hd::log::i("Loading meshes: %d\n", meshArraySize);

            while (meshIndex < meshArraySize) {
                Json::Value mesh = meshes[meshIndex];
                if (mesh.isMember(ASSIMP_MESH_NAME)
                    && mesh.isMember(ASSIMP_MESH_MATERIAL_INDEX)
                    && mesh.isMember(ASSIMP_MESH_PRIMITIVE_TYPES)
                    && mesh.isMember(ASSIMP_MESH_VERTICES_ARRAY)
                    && mesh[ASSIMP_MESH_VERTICES_ARRAY].isArray()
                    && mesh.isMember(ASSIMP_MESH_FACES_ARRAY)
                    && mesh[ASSIMP_MESH_FACES_ARRAY].isArray()) {

                    Mesh *result = new(std::nothrow) Mesh();
                    assert(result);

                    meshSet->AddMesh(result);

                    // material
                    unsigned int materialIndex = mesh[ASSIMP_MESH_MATERIAL_INDEX].asUInt();
                    hd::graphics::MeshMaterial *meshMaterial = materials.At(materialIndex);
                    if (meshMaterial) {
                        result->SetMeshMaterial(meshMaterial);
                    }

                    // vertices
                    Json::Value vertices = mesh[ASSIMP_MESH_VERTICES_ARRAY];
                    Json::Value normals = mesh[ASSIMP_MESH_NORMALS_ARRAY];
                    Json::Value tangents = mesh[ASSIMP_MESH_TANGENTS_ARRAY];
                    Json::Value textureCoords = mesh[ASSIMP_MESH_TEXTURE_COORDS];
                    if (textureCoords.isArray()) {
                        textureCoords = textureCoords[0];
                    }

                    Json::ArrayIndex vertexIndex = 0;
                    Json::ArrayIndex textureCoordIndex = 0;
                    Json::ArrayIndex vertexArraySize = vertices.size();

                    hd::log::i("\tVertices: %d, Normals: %d, Texture Coords: %d\n", vertexArraySize, normals.size(), textureCoords.size());

                    while (vertexIndex < vertexArraySize) {
                        float v1 = vertices[vertexIndex + 0].asFloat();
                        float v2 = vertices[vertexIndex + 1].asFloat();
                        float v3 = vertices[vertexIndex + 2].asFloat();

                        Vertex vertex;
                        vertex.origin.Set(v1, v2, v3);

                        if (normals.isArray()
                            && normals[vertexIndex + 0].isConvertibleTo(Json::ValueType::realValue)
                            && normals[vertexIndex + 1].isConvertibleTo(Json::ValueType::realValue)
                            && normals[vertexIndex + 2].isConvertibleTo(Json::ValueType::realValue)) {
                            float n1 = normals[vertexIndex + 0].asFloat();
                            float n2 = normals[vertexIndex + 1].asFloat();
                            float n3 = normals[vertexIndex + 2].asFloat();
                            vertex.normal.Set(n1, n2, n3);
                        }

                        if (tangents.isArray()
                            && tangents[vertexIndex + 0].isConvertibleTo(Json::ValueType::realValue)
                            && tangents[vertexIndex + 1].isConvertibleTo(Json::ValueType::realValue)
                            && tangents[vertexIndex + 2].isConvertibleTo(Json::ValueType::realValue)) {
                            float t1 = tangents[vertexIndex + 0].asFloat();
                            float t2 = tangents[vertexIndex + 1].asFloat();
                            float t3 = tangents[vertexIndex + 2].asFloat();
                            vertex.tangent.Set(t1, t2, t3);
                        }

                        if (textureCoords.isArray()
                            && textureCoords[textureCoordIndex + 0].isConvertibleTo(Json::ValueType::realValue)
                            && textureCoords[textureCoordIndex + 1].isConvertibleTo(Json::ValueType::realValue)) {
                            float t1 = textureCoords[textureCoordIndex + 0].asFloat();
                            float t2 = textureCoords[textureCoordIndex + 1].asFloat();
                            vertex.texCoord.Set(t1, t2);
                        }
                        
                        result->AddVertex(vertex);
                        
                        vertexIndex += 3;
                        textureCoordIndex += 2;
                    }
                    
                    // faces
                    Json::Value faces = mesh[ASSIMP_MESH_FACES_ARRAY];
                    
                    Json::ArrayIndex faceIndex = 0;
                    Json::ArrayIndex faceArraySize = faces.size();
                    
                    hd::log::i("\tFaces: %d\n", faceArraySize);
                    
                    while (faceIndex < faceArraySize) {
                        Json::Value face = faces[faceIndex];
                        if (!face.isArray()) {
                            continue;
                        }
                        if (face.size() == 3) {
                            unsigned int f1 = face[0].asUInt();
                            unsigned int f2 = face[1].asUInt();
                            unsigned int f3 = face[2].asUInt();
                            
                            Size3 triangle(f1, f2, f3);
                            result->AddTriangle(triangle);
                        } else if (face.size() == 4) {
                            unsigned int f1 = face[0].asUInt();
                            unsigned int f2 = face[1].asUInt();
                            unsigned int f3 = face[2].asUInt();
                            unsigned int f4 = face[3].asUInt();
                            
                            Size4 quad(f1, f2, f3, f4);
                            result->AddQuad(quad);
                        }
                        ++faceIndex;
                    }
                    if (normals.size() == 0) {
                        result->GenerateNormals();
                    }
                    result->GenerateExtents();
                }
                ++meshIndex;
            }
        }
        return meshSet;
    } else {
        return NULL;
    }
}
