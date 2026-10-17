#ifndef _VAST_GRAPHICS_H_
#define _VAST_GRAPHICS_H_

#include "graphics/texture/Texture.h"
#include "graphics/texture/TextureManager.h"

#include "graphics/shader/ShaderProgram.h"
#include "graphics/shader/ShaderLoader.h"

#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshData.h"
#include "graphics/mesh/MeshMaterial.h"
#include "graphics/mesh/MeshLoader.h"
#include "graphics/mesh/MeshInstanceData.h"

#include "graphics/gl/GraphicsInterface.h"
#include "graphics/gl/LightRenderer.h"
#include "graphics/gl/PixelBufferObject.h"
#include "graphics/util/hdNoise.h"

// Camera components
#include "graphics/components/LookAtData.h"
#include "graphics/components/PerspectiveData.h"
#include "graphics/components/OrthographicData.h"
#include "graphics/components/Projection.h"
#include "graphics/components/Viewport.h"
#include "graphics/components/Visibility.h"
#include "graphics/components/ScreenData.h"

// lighting components
#include "graphics/components/Light.h"
#include "graphics/components/LightModel.h"
#include "graphics/components/Material.h"
#include "graphics/components/OpacityData.h"

// geometry components
#include "graphics/components/Polygon.h"
#include "graphics/components/CollisionData.h"
#include "graphics/components/PolygonRenderer.h"
#include "graphics/components/Curve.h"
#include "graphics/components/CurveRenderer.h"
#include "graphics/components/ExtrusionData.h"
#include "graphics/components/ExtrusionRenderer.h"
#include "graphics/components/CollisionVolume.h"
#include "graphics/components/MeshReference.h"

#include "graphics/components/RayHitTester.h"
#include "graphics/components/Transform.h"

#include "graphics/components/Shader.h"
#include "graphics/components/VertexBuffer.h"
#include "graphics/components/RenderTarget.h"
#include "graphics/components/TextureTarget.h"
#include "graphics/components/DepthRenderTarget.h"
#include "graphics/components/GeometryRenderTarget.h"
#include "graphics/components/RayTracer.h"
#include "graphics/components/PointKernel.h"
#include "graphics/components/PixelBufferSample.h"

#endif