#include "DeviceDetector.h"
#include <Vast/VastGraphics.h>
#include "gibs/headset/DistortionMesh.h"
#include "gibs/headset/DistortionVertex.h"


hd::input::DeviceDetector::DeviceDetector() : m_displayName(),
    m_displayId(0),
    m_distortionShader(NULL),
    m_meshDebugShader(NULL) {
        m_distortionShader = NULL;
        m_shaderPool    = new ComponentPool<hd::graphics::Shader>(4);
        m_leftDistortionMesh = new hd::input::DistortionMesh();
        m_rightDistortionMesh = new hd::input::DistortionMesh();
}


void hd::input::DeviceDetector::CheckDevices() {
    // Look for oculus devices

    FindHeadsets();
}


void hd::input::DeviceDetector::FindHeadsets() {
}


void hd::input::DeviceDetector::InitForRendering() {
}


//
// HACK HACK HACK
//
// Put this into a separate class.
//
static float LastSensorYaw = 0;
static float EyeYaw = 0;

//static ovrFrameTiming frameTiming;

Quaternion hd::input::DeviceDetector::GetDeviceOrientation() {
    return Quaternion();
}


void hd::input::DeviceDetector::RecenterDevice() {
   // ovrHmd_RecenterPose(m_hmd);
}


static const float TRACKING_ADJUSTMENT_FACTOR = 150.0f;

Vec3 hd::input::DeviceDetector::GetDeviceTrackingOffset() {
    return TRACKING_ADJUSTMENT_FACTOR * Vec3();
}


static bool USE_SCREEN_FOR_VIEWPORT = true;

AABB hd::input::DeviceDetector::GetViewport(const hd::input::EyePosition eye) {
        float width =  hd::Device::GetDisplayScreenWidth();
        float height = hd::Device::GetDisplayScreenHeight();

        if (eye == hd::input::EyePosition::Left) {
            return AABB(0.0f, 0.0f, 0.5f * width, height);
        } else if (eye == hd::input::EyePosition::Right) {
            return AABB(0.0f, 0.0f, 0.5f * width, height);
        } else {
            return AABB(0.0f, 0.0f, width, height);
        }
}


static const float DEFAULT_PERSPECTIVE_FOV = 100.0f*(hd_pi/180.0f);

Matrix hd::input::DeviceDetector::GetProjectionMatrix(hd::input::EyePosition eye) {
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();

    Matrix projection;
    Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);

    return projection;
}


static const float IPD_ADJUSTMENT_FACTOR = 300.0f;

Vec3 hd::input::DeviceDetector::GetIPDOffset(hd::input::EyePosition eye) {
    return Vec3();
}


Matrix hd::input::DeviceDetector::GetInverseProjectionMatrix(hd::input::EyePosition eye) {
    const float displayWidth = Device::GetDisplayScreenWidth();
    const float displayHeight = Device::GetDisplayScreenHeight();

    Matrix projection;
    Matrix inverse;
        Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);
    
    Matrix::InverseLong(inverse, projection);
    return inverse;
}


void hd::input::DeviceDetector::BeginTiming() {
    //frameTiming = ovrHmd_BeginFrameTiming(m_hmd, 0);
}


void hd::input::DeviceDetector::UpdateTiming() {
    //ovr_WaitTillTime(frameTiming.TimewarpPointSeconds);
}


void hd::input::DeviceDetector::EndTiming() {
    //ovrHmd_EndFrameTiming(m_hmd);
}


/**
 * (DONE) Initialize LibOVR
 * (DONE) Load an initialize HMD
 * (DONE) Start Sensor Input
 * (DONE) Functions for getting sensor data from the device
 *          Orientation Quaternion
 *          Tracking position
 * (DONE) IPD distance stuff.
 * Movement input needs heading optionality: FPS style (movement control xz plane only) vs FlightSim style (xyz).
 * (DONE) Camera viewports: get viewport size and proj from the headset api
 * (DONE) Get distortion meshes for each eye and store them in a vertex buffer
 * (DONE) New distortion vertex/fragment shaders.
 */


void hd::input::DeviceDetector::Render(const hd::Texture *leftTexture, const hd::Texture *rightTexture) {
    /*
    const char * UV_SCALE_NAME = "eyeToSourceUVScale";
    const char * UV_OFFSET_NAME = "eyeToSourceUVOffset";
    const char * EYE_ROT_START_NAME = "eyeRotationStart";
    const char * EYE_ROT_END_NAME = "eyeRotationEnd";
    const char * TEXTURE_NAME = "iTexture0";

    hdAssert(m_distortionShader);

    ovrEyeRenderDesc eyeRenderDesc[2];
    eyeRenderDesc[0] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Left, m_hmd->DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Right, m_hmd->DefaultEyeFov[1]);

    const hd::Texture * textures[2];
    textures[0] = leftTexture;
    textures[1] = rightTexture;

    hd::input::DistortionMesh *distortionMeshes[2];
    distortionMeshes[0] = m_leftDistortionMesh;
    distortionMeshes[1] = m_rightDistortionMesh;

    ovrRecti viewports[2];
    viewports[0].Pos.x = 0.0f;
    viewports[0].Pos.y = 0.0f;
    viewports[0].Size = OVR::Sizei(leftTexture->width, leftTexture->height);

    viewports[1].Pos.x = 0.0f;
    viewports[1].Pos.y = 0.0f;
    viewports[1].Size = OVR::Sizei(rightTexture->width, rightTexture->height);

    ovrFovPort eyeFov[2];
    eyeFov[0] = m_hmd->DefaultEyeFov[0];
    eyeFov[1] = m_hmd->DefaultEyeFov[1];

    for (int eyeNum = 0; eyeNum < 2; eyeNum++) {
        const hd::Texture *texture = textures[eyeNum];
        hd::input::DistortionMesh *distortionMesh = distortionMeshes[eyeNum];

        ovrVector2f scaleAndOffset[2];
        ovrHmd_GetRenderScaleAndOffset(eyeFov[eyeNum],
                                       viewports[eyeNum].Size,
                                       viewports[eyeNum],
                                       scaleAndOffset);
        OVR::Posef pose  = ovrHmd_GetHmdPosePerEye(m_hmd, (ovrEyeType)eyeNum);

        ovrMatrix4f timeWarpMatrices[2];
        ovrHmd_GetEyeTimewarpMatrices(m_hmd, (ovrEyeType)eyeNum, pose, timeWarpMatrices);

        Matrix start, end;
        Matrix::Set(start, (float *)OVR::Matrix4f(timeWarpMatrices[0]).Transposed().M);
        Matrix::Set(end, (float *)OVR::Matrix4f(timeWarpMatrices[1]).Transposed().M);
        m_distortionShader->Begin();
        m_distortionShader->BindTexture(TEXTURE_NAME, texture);
        m_distortionShader->SetUniform2f(UV_SCALE_NAME, scaleAndOffset[0].x, scaleAndOffset[0].y);
        m_distortionShader->SetUniform2f(UV_OFFSET_NAME, scaleAndOffset[1].x, scaleAndOffset[1].y);
        m_distortionShader->SetUniformMatrix(EYE_ROT_START_NAME, start);
        m_distortionShader->SetUniformMatrix(EYE_ROT_END_NAME, end);
        distortionMesh->Render();
        m_distortionShader->End();
    }*/
}


void hd::input::DeviceDetector::CreateDistortionMeshes() {
    /*
    hdAssert(m_leftDistortionMesh && m_rightDistortionMesh);

    ovrEyeRenderDesc eyeRenderDesc[2];
    eyeRenderDesc[0] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Left, m_hmd->DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Right, m_hmd->DefaultEyeFov[1]);

    hd::input::DistortionMesh *meshes[2];
    meshes[0] = m_leftDistortionMesh;
    meshes[1] = m_rightDistortionMesh;

    // This variable is currently unused in the oculus SDK, but required for the ovrHmd_CreateDistortionMesh
    // function header.
    unsigned int distortionCaps = 0;

    for (int eyeNum = 0; eyeNum < 2; eyeNum++) {
        ovrDistortionMesh meshData;

        OVR::Sizei textureSize = ovrHmd_GetFovTextureSize(m_hmd, eyeRenderDesc[eyeNum].Eye, eyeRenderDesc[eyeNum].Fov, 1.0f);

        ovrVector2f offset;

        ovrHmd_CreateDistortionMesh(m_hmd,
                                    eyeRenderDesc[eyeNum].Eye,
                                    eyeRenderDesc[eyeNum].Fov,
                                    distortionCaps, &meshData);
        ovrHmd_GetRenderScaleAndOffset(eyeRenderDesc[eyeNum].Fov,
                                       textureSize, eyeRenderDesc[eyeNum].DistortedViewport,
                                       &offset);

        hd::input::DistortionMesh *mesh = meshes[eyeNum];
        hd::Vector<hd::input::DistortionVertex> *distortionVertices = new hd::Vector<hd::input::DistortionVertex>(meshData.VertexCount);

        ovrDistortionVertex *ov = meshData.pVertexData;
        for (unsigned vertNum = 0; vertNum < meshData.VertexCount; vertNum++) {
            DistortionVertex vertex;
            vertex.position.Set(ov->ScreenPosNDC.x,
                                ov->ScreenPosNDC.y,
                                ov->TimeWarpFactor,
                                ov->VignetteFactor);
            vertex.tanEyeAnglesR.Set(ov->TanEyeAnglesR.x, ov->TanEyeAnglesR.y);
            vertex.tanEyeAnglesG.Set(ov->TanEyeAnglesG.x, ov->TanEyeAnglesG.y);
            vertex.tanEyeAnglesB.Set(ov->TanEyeAnglesB.x, ov->TanEyeAnglesB.y);
            distortionVertices->Add(vertex);
            ov++;
        }

        mesh->Reset();
        mesh->SetVertices(distortionVertices);
        mesh->SetIndices(meshData.pIndexData, meshData.IndexCount);

        ovrHmd_DestroyDistortionMesh( &meshData );

        DeleteSafely(distortionVertices);
    }*/
}
