#include "ShadowTest.h"

static AABB worldSize(0, 0, 2048, 512);

ShadowTest::ShadowTest(hd::application::Client *client) : AppPrototype(client) {
    m_skyboxRenderer.m_musicPlayer = &m_musicPlayer;
    m_blockMover.m_musicPlayer = &m_musicPlayer;
    
    Create();
}


void ShadowTest::Create() {
    ConstructSkybox(3);
    AddLabel(Vec3(0, 500, 0), Vec3(), "Shadow test - WASD to move. Tab returns to the browser.");
    AddFloorCube(2048.0f, 50.0f, Vec3(0, -150.0f, 0.0f));
    AddFloorCube(150.0f, 200.0f, Vec3(-300.0f, 0.0f, -150.0f));
    AddFloorCube(150.0f, 200.0f, Vec3(300.0f, 0.0f, 150.0f));
}
