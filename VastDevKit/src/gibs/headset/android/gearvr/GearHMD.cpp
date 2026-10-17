#include "OculusHMD.h"
#include <Vast/VastGraphics.h>


hd::input::OculusHMD::OculusHMD() : m_displayName(),
    m_displayId(0),
    m_distortionShader(NULL),
    m_meshDebugShader(NULL) {
}


void hd::input::OculusHMD::InitDevice() {
    ovr_Initialize();
    int count = ovrHmd_Detect();
    if (count == 0) {
        m_hmd = ovrHmd_CreateDebug(ovrHmdType::ovrHmd_DK2);
    } else {
        m_hmd = ovrHmd_Create(0);

        // Sensor setup
        if (false == (ovrHmd_ConfigureTracking(m_hmd,
                                               ovrTrackingCap_Orientation |
                                               ovrTrackingCap_Position |
                                               ovrTrackingCap_MagYawCorrection, 0))){
            hd::log::e("Error in configuring headset sensor tracking\n");
            return;
        }
    }

    RecenterDevice();
}


void hd::input::OculusHMD::InitForRendering() {
}


//
// HACK HACK HACK
//
// Put this into a separate class.
//
static float LastSensorYaw = 0;
static float EyeYaw = 0;

static ovrFrameTiming frameTiming;

Quaternion hd::input::OculusHMD::GetDeviceOrientation() {
    /*
     In a production application, however, you should use one of the real-time computed values 
     returned by ovrHmd_BeginFrame or ovrHmd_BeginFrameTiming.
     */
    const double predictedDisplayTime = vrapi_GetPredictedDisplayTime(m_ovrMobile, m_frameIndex);
    const ovrTracking tracking = vrapi_GetPredictedTracking( appState.Ovr, predictedDisplayTime );
    ovrPosef pose = tracking.HeadPose.Pose;
    Quaternion q(-pose.Orientation.x, pose.Orientation.y, -pose.Orientation.z, pose.Orientation.w);
    return q;
}


void hd::input::OculusHMD::RecenterDevice() {
    vrapi_RecenterPose(m_hmd);
}


static const float TRACKING_ADJUSTMENT_FACTOR = 150.0f;

Vec3 hd::input::OculusHMD::GetDeviceTrackingOffset() {
    const double predictedDisplayTime = vrapi_GetPredictedDisplayTime(m_ovrMobile, m_frameIndex);
    const ovrTracking tracking = vrapi_GetPredictedTracking( appState.Ovr, predictedDisplayTime );
    ovrPosef pose = tracking.HeadPose.Pose;

    return TRACKING_ADJUSTMENT_FACTOR * Vec3(-pose.Translation.x, pose.Translation.y, -pose.Translation.z);
}


static bool USE_SCREEN_FOR_VIEWPORT = true;

AABB hd::input::OculusHMD::GetViewport(const hd::input::EyePosition eye) {
    const ovrHmdInfo hmdInfo = vrapi_GetHmdInfo(&m_java);
    const float width = hmdInfo.SuggestedEyeResolution[0];
    const float height = hmdInfo.SuggestedEyeResolution[1];

    if (eye == hd::input::EyePosition::Left) {
        return AABB(0.0f, 0.0f, 0.5f * width, height);
    } else if (eye == hd::input::EyePosition::Right) {
        return AABB(0.0f, 0.0f, 0.5f * width, height);
    } else {
        return AABB(0.0f, 0.0f, width, height);
    }
}


static const float DEFAULT_PERSPECTIVE_FOV = 100.0f*(hd_pi/180.0f);

Matrix hd::input::OculusHMD::GetProjectionMatrix(hd::input::EyePosition eye) {
    const ovrHmdInfo hmdInfo = vrapi_GetHmdInfo(&m_java);
    const float displayWidth = hmdInfo.SuggestedEyeResolution[0];
    const float displayHeight = hmdInfo.SuggestedEyeResolution[1];

    Matrix tmp, projection;

    if (eye == hd::input::EyePosition::Left) {
        //OVR::Matrix4f proj = ovrMatrix4f_Projection(eyeFov[0], 1.0f, 60000.0f, true);
        //Matrix::Set(tmp, (float *)proj.M);
        Matrix::PerspectiveFovRH(projection, DEFAULT_PERSPECTIVE_FOV, 0.5f * displayWidth / displayHeight, 1.0f, 60000.0f);
    } else if (eye == hd::input::EyePosition::Right) {
        //OVR::Matrix4f proj = ovrMatrix4f_Projection(eyeFov[1], 1.0f, 60000.0f, true);
        //Matrix::Set(tmp, (float *)proj.M);
        Matrix::PerspectiveFovRH(projection, DEFAULT_PERSPECTIVE_FOV, 0.5f * displayWidth / displayHeight, 1.0f, 60000.0f);
    } else {
        Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);
    }

    return projection;
}


static const float IPD_ADJUSTMENT_FACTOR = 300.0f;

Vec3 hd::input::OculusHMD::GetIPDOffset(hd::input::EyePosition eye) {
    ovrEyeRenderDesc eyeRenderDesc[2];

    eyeRenderDesc[0] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Left, m_hmd->DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovrHmd_GetRenderDesc(m_hmd, ovrEye_Right, m_hmd->DefaultEyeFov[1]);

    if (eye == hd::input::EyePosition::Left) {
        return IPD_ADJUSTMENT_FACTOR * Vec3(eyeRenderDesc[0].HmdToEyeViewOffset.x,
                                            eyeRenderDesc[0].HmdToEyeViewOffset.y,
                                            eyeRenderDesc[0].HmdToEyeViewOffset.z);

    } else if (eye == hd::input::EyePosition::Right) {
        return IPD_ADJUSTMENT_FACTOR * Vec3(eyeRenderDesc[1].HmdToEyeViewOffset.x,
                                            eyeRenderDesc[1].HmdToEyeViewOffset.y,
                                            eyeRenderDesc[1].HmdToEyeViewOffset.z);
    } else {
        return Vec3();
    }
}


float hd::input::OculusHMD::GetIPDRadius() const {
    return 0.0f;
}


Matrix hd::input::OculusHMD::GetInverseProjectionMatrix(hd::input::EyePosition eye) {
    const ovrHmdInfo hmdInfo = vrapi_GetHmdInfo(&m_java);
    const float displayWidth = hmdInfo.SuggestedEyeResolution[0];
    const float displayHeight = hmdInfo.SuggestedEyeResolution[1];

    Matrix projection;
    Matrix inverse;

    if (eye == hd::input::EyePosition::Left) {
       //// OVR::Matrix4f proj = ovrMatrix4f_Projection(eyeFov[0], 1.0f, 60000.0f, true);
       // Matrix::Set(projection, (float *)proj.Inverted().M);
        Matrix::PerspectiveFovRH(projection, DEFAULT_PERSPECTIVE_FOV, 0.5f * displayWidth / displayHeight, 1.0f, 60000.0f);
    } else if (eye == hd::input::EyePosition::Right) {
        //OVR::Matrix4f proj = ovrMatrix4f_Projection(eyeFov[1], 1.0f, 60000.0f, true);
        //Matrix::Set(projection, (float *)proj.Inverted().M);
        Matrix::PerspectiveFovRH(projection, DEFAULT_PERSPECTIVE_FOV, 0.5f * displayWidth / displayHeight, 1.0f, 60000.0f);
    } else {
        Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);
    }
    Matrix::InverseLong(inverse, projection);
    return inverse;
}


void hd::input::OculusHMD::BeginTiming() {
    //frameTiming = ovrHmd_BeginFrameTiming(m_hmd, 0);
}


void hd::input::OculusHMD::UpdateTiming() {
    //ovr_WaitTillTime(frameTiming.TimewarpPointSeconds);
}


void hd::input::OculusHMD::EndTiming() {
    //ovrHmd_EndFrameTiming(m_hmd);
}


void hd::input::OculusHMD::Render(const hd::Texture *leftTexture, const hd::Texture *rightTexture) {
        m_frameIndex++;
}
