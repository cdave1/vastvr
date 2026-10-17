#include "TeapotDemo.h"

TeapotDemo::TeapotDemo(hd::application::Client *client) : AppPrototype(client) {
    CreateTeapotDemo();
}
void TeapotDemo::CreateTeapotDemo() {
    ConstructSkybox(3);
    AddFloorCube(1600, 20, Vec3(0, -70, 0));
    for (int x = -1; x <= 1; ++x) for (int z = -1; z <= 1; ++z) {
        AddMesh("resources/models/teapot/teapot.obj", 14, Vec3(x * 430.0f, 0, z * 430.0f));
    }
    AddLabel(Vec3(0, 500, 0), Vec3(), "Utah teapot - CC0 model. Tab returns to the browser.");
}
void TeapotDemo::OnUiSelect(hd::Entity *, const Collision3 &) {}
void TeapotDemo::OnUiRelease(hd::Entity *, const Collision3 &) {}
