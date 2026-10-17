#include "SSAOTest.h"

// Retained as a geometry preview; the historical SSAO pass is disabled.
SSAOTest::SSAOTest(hd::application::Client *client) : AppPrototype(client) {
    CreateSSAOTest();
}
void SSAOTest::CreateSSAOTest() {
    ConstructSkybox(3);
    AddFloorCube(1400, 20, Vec3(0, -100, 0));
    for (int i = -2; i <= 2; ++i) {
        auto cube = AddCube(110, 120 + (i + 2) * 35, 110, Vec3(i * 300.0f, 50, 0));
        cube->Edit<hd::input::pointer::ReleaseResponder>()->OnReleaseInside(this, &SSAOTest::OnBlockRelease);
    }
    AddLabel(Vec3(0, 550, 0), Vec3(), "Look at a block and press Space to change its color.");
}
void SSAOTest::OnBlockSelect(hd::Entity *, const Collision3 &) {}
void SSAOTest::OnBlockDrag(hd::Entity *, const Collision3 &, const Vec3 &) {}
void SSAOTest::OnBlockRelease(hd::Entity *block, const Collision3 &) {
    block->Edit<hd::graphics::Material>()->SetAmbient(Color4f(hd::randomf(0.1f, 1.0f), hd::randomf(0.1f, 1.0f), hd::randomf(0.1f, 1.0f), 1.0f));
}
