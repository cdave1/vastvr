#include "OculusHMD.h"
#include <Vast/VastGraphics.h>
#include "gibs/headset/oculus/DistortionMesh.h"
#include "gibs/headset/oculus/DistortionVertex.h"

ovrSession m_ovrSession;
ovrHmdDesc m_ovrDesc;

static double frameTiming = 0.0f;
static double sensorSampleTime = 0.0f;
static ovrPosef EyeRenderPose[2];


hd::input::OculusHMD::OculusHMD() : m_displayName(),
    m_displayId(0) {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

}


static bool ALREADY_INITTED = false;
int hd::input::OculusHMD::InitDevice() {

	if (ALREADY_INITTED) {
		hd::log::w("Already initialized");
		return 0;
	}

	ovrResult result = ovr_Initialize(nullptr);
	if (OVR_FAILURE(result)) {
		hd::log::e("Failed to initialzise libOVR (code: %d).", result);
		return result;
	}
	
	ovrGraphicsLuid luid;
	result = ovr_Create(&m_ovrSession, &luid);

	if (OVR_FAILURE(result)) {
		hd::log::e("Error creating ovr device (code: %d).", result);
		return result;
	}

    m_ovrDesc = ovr_GetHmdDesc(m_ovrSession);
    RecenterDevice();

	ALREADY_INITTED = true;

	return 0;
}


void hd::input::OculusHMD::InitForRendering() {
    for (int eye = 0; eye < 2; eye++) {
        ovrSizei idealSize = ovr_GetFovTextureSize(m_ovrSession, (ovrEyeType)eye, m_ovrDesc.DefaultEyeFov[eye], 1.0f);
		m_renderTargets[eye] = new OculusTextureTarget();
		m_renderTargets[eye]->Create(m_ovrSession, Size2(idealSize.w, idealSize.h));
    }

	float windowWidth = hd::Device::GetDisplayScreenWidth();
	float windowHeight = hd::Device::GetDisplayScreenHeight();

	m_mirrorTarget = new OculusMirrorTarget();
	m_mirrorTarget->Create(m_ovrSession, Size2(windowWidth, windowHeight));

	ovr_SetTrackingOriginType(m_ovrSession, ovrTrackingOrigin_FloorLevel);

	//SDL_GL_SetSwapInterval(0);
	
	//wglSwapIntervalEXT(0);
}


//
// HACK HACK HACK
//
// Put this into a separate class.
//
static float LastSensorYaw = 0;
static float EyeYaw = 0;

Quaternion hd::input::OculusHMD::GetDeviceOrientation() {
	ovrTrackingState ts = ovr_GetTrackingState(m_ovrSession, frameTiming, ovrTrue);
    ovrPosef pose = ts.HeadPose.ThePose;
	Quaternion q(pose.Orientation.x, pose.Orientation.y, pose.Orientation.z, pose.Orientation.w);
	return q;
}


void hd::input::OculusHMD::RecenterDevice() {
	ovr_RecenterTrackingOrigin(m_ovrSession);
    //ovr_RecenterPose(m_ovrSession);
}


// DEPRECATED: EyeOffsets now contain this information.
Vec3 hd::input::OculusHMD::GetDeviceTrackingOffset() {
	return Vec3();
}


AABB hd::input::OculusHMD::GetViewport(const hd::input::EyePosition eye) {
    const float DesiredPixelDensity = 1.0f;

    ovrSizei tex0Size = ovr_GetFovTextureSize(m_ovrSession, ovrEye_Left, m_ovrDesc.DefaultEyeFov[ovrEye_Left], DesiredPixelDensity);
    ovrSizei tex1Size = ovr_GetFovTextureSize(m_ovrSession, ovrEye_Right, m_ovrDesc.DefaultEyeFov[ovrEye_Right], DesiredPixelDensity);
    ovrSizei res = m_ovrDesc.Resolution;

    if (eye == hd::input::EyePosition::Left) {
		return AABB(0.0f, 0.0f, tex0Size.w, tex0Size.h);
    } else if (eye == hd::input::EyePosition::Right) {
		return AABB(0.0f, 0.0f, tex1Size.w, tex1Size.h);
    } else {
		return AABB(0.0f, 0.0f, tex0Size.w + tex1Size.w, tex0Size.h);
    }
}


static const float DEFAULT_PERSPECTIVE_FOV = 100.0f*(hd_pi/180.0f);

Matrix hd::input::OculusHMD::GetProjectionMatrix(hd::input::EyePosition eye) {
    ovrEyeRenderDesc eyeRenderDesc[2];

    eyeRenderDesc[0] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Left, m_ovrDesc.DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Right, m_ovrDesc.DefaultEyeFov[1]);

    const bool USE_OVR_PROJECTION = true;
    Matrix tmp, projection;

    if (eye == hd::input::EyePosition::Left) {
		ovrMatrix4f proj = ovrMatrix4f_Projection(m_ovrDesc.DefaultEyeFov[0], 0.1f, 60000.0f, ovrProjection_None);// ovrProjection_FarClipAtInfinity);
        Matrix::Set(projection, (float *)proj.M);
    } else if (eye == hd::input::EyePosition::Right) {
		ovrMatrix4f proj = ovrMatrix4f_Projection(m_ovrDesc.DefaultEyeFov[1], 0.1f, 60000.0f, ovrProjection_None); // ovrProjection_FarClipAtInfinity);
        Matrix::Set(projection, (float *)proj.M);
    } else {
		const float displayWidth = Device::GetDisplayScreenWidth();
		const float displayHeight = Device::GetDisplayScreenHeight();
        Matrix::PerspectiveFovRH(projection, 0.5f * DEFAULT_PERSPECTIVE_FOV, displayWidth / displayHeight, 1.0f, 60000.0f);
    }

    return projection;
}


Vec3 hd::input::OculusHMD::GetEyeOffset(hd::input::EyePosition eye) {
    if (eye == hd::input::EyePosition::Left) {
		return Vec3(EyeRenderPose[0].Position.x, EyeRenderPose[0].Position.y, EyeRenderPose[0].Position.z);
    } else if (eye == hd::input::EyePosition::Right) {
		return Vec3(EyeRenderPose[1].Position.x, EyeRenderPose[1].Position.y, EyeRenderPose[1].Position.z);
    } else {
        return Vec3();
    }
}

Quaternion hd::input::OculusHMD::GetEyeOrientation(hd::input::EyePosition eye) {
	if (eye == hd::input::EyePosition::Left) {
		return Quaternion(EyeRenderPose[0].Orientation.x, EyeRenderPose[0].Orientation.y, EyeRenderPose[0].Orientation.z, EyeRenderPose[0].Orientation.w);
	} else if (eye == hd::input::EyePosition::Right) {
		return Quaternion(EyeRenderPose[1].Orientation.x, EyeRenderPose[1].Orientation.y, EyeRenderPose[1].Orientation.z, EyeRenderPose[1].Orientation.w);
	} else {
		return GetDeviceOrientation();
	}
}


float hd::input::OculusHMD::GetIPDRadius() const {
    return 0.0f;
}


Matrix hd::input::OculusHMD::GetInverseProjectionMatrix(hd::input::EyePosition eye) {
    Matrix projection = this->GetProjectionMatrix(eye);
    Matrix inverse;
    Matrix::InverseLong(inverse, projection);
    return inverse;
}


static const float IPD_SCALE_FACTOR = 10.0f;
static const float IPD_ADJUSTMENT_FACTOR = 1.0f;// 10.0f;
static long long frameIndex = 0;

void hd::input::OculusHMD::BeginTiming() {
	ovrEyeRenderDesc eyeRenderDesc[2];
	eyeRenderDesc[0] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Left, m_ovrDesc.DefaultEyeFov[0]);
	eyeRenderDesc[1] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Right, m_ovrDesc.DefaultEyeFov[1]);

	ovrPosef hmdToEyePose[2] = {
		eyeRenderDesc[0].HmdToEyePose,
		eyeRenderDesc[1].HmdToEyePose
	};

	frameTiming = ovr_GetPredictedDisplayTime(m_ovrSession, 0);
	// Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
	ovrTrackingState hmdState = ovr_GetTrackingState(m_ovrSession, frameTiming, ovrTrue);
	ovr_GetEyePoses(m_ovrSession, frameIndex, ovrTrue, hmdToEyePose, EyeRenderPose, &sensorSampleTime);
}


void hd::input::OculusHMD::UpdateTiming() {
}


void hd::input::OculusHMD::EndTiming() {
	frameIndex++;
}


void  hd::input::OculusHMD::PreRender(hd::input::EyePosition eye) {
	OculusTextureTarget *target = nullptr;
	if (eye == hd::input::EyePosition::Left) {
		target = m_renderTargets[0];
	} else if (eye == hd::input::EyePosition::Right) {
		target = m_renderTargets[1];
	}

	if (target) {
		target->Start(m_ovrSession);
	}
}


void hd::input::OculusHMD::Render() {
    ovrEyeRenderDesc eyeRenderDesc[2];
    eyeRenderDesc[0] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Left, m_ovrDesc.DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovr_GetRenderDesc(m_ovrSession, ovrEye_Right, m_ovrDesc.DefaultEyeFov[1]);

	ovrPosef hmdToEyePose[2] = {
		eyeRenderDesc[0].HmdToEyePose,
		eyeRenderDesc[1].HmdToEyePose
	};

    // Set up positional data.
    ovrViewScaleDesc viewScaleDesc;
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f; // 0.025;
    viewScaleDesc.HmdToEyePose[0] = hmdToEyePose[0];
    viewScaleDesc.HmdToEyePose[1] = hmdToEyePose[1];
    
    ovrLayerEyeFov ld;
    ld.Header.Type = ovrLayerType_EyeFov;
	ld.Header.Flags =ovrLayerFlag_TextureOriginAtBottomLeft;

    for (int eye = 0; eye < 2; ++eye) {
        ovrRecti viewport;
		viewport.Pos.x = 0;
		viewport.Pos.y = 0;
        viewport.Size.w = m_renderTargets[eye]->GetSize()[0];
        viewport.Size.h = m_renderTargets[eye]->GetSize()[1];

		ld.ColorTexture[eye] = m_renderTargets[eye]->GetOvrTextureSwapChain();
        ld.Viewport[eye] = viewport;
		ld.Fov[eye] = eyeRenderDesc[eye].Fov;
        ld.RenderPose[eye] = EyeRenderPose[eye];
        ld.SensorSampleTime = sensorSampleTime;
    }

    ovrLayerHeader* layers = &ld.Header;
    ovrResult result = ovr_SubmitFrame(m_ovrSession, 0, &viewScaleDesc, &layers, 1);

    bool isVisible = (result == ovrSuccess);
}


void  hd::input::OculusHMD::PostRender(hd::input::EyePosition eye) {
	OculusTextureTarget *target = nullptr;
	if (eye == hd::input::EyePosition::Left) {
		target = m_renderTargets[0];
	}
	else if (eye == hd::input::EyePosition::Right) {
		target = m_renderTargets[1];
	}

	if (target) {
		target->End();
		ovr_CommitTextureSwapChain(m_ovrSession, target->GetOvrTextureSwapChain());
	}
}


void hd::input::OculusHMD::CreateDistortionMeshes() {
}


void hd::input::OculusHMD::RenderMirror() {
	m_mirrorTarget->Render();
}
