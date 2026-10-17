#include "iCrackedApp.h"

#define ICRACKED_RESOURCE_FILE "models/icracked/icracked.obj.json"

using namespace hd;

iCrackedApp::iCrackedApp(hd::application::Client *client) : AppPrototype(client) {
    hd::graphics::MeshLoader *loader = hd::graphics::MeshLoader::Instance();
    hd::graphics::MeshData *logo = loader->GetMeshFromFile(ICRACKED_RESOURCE_FILE);
    //logo->FlipNormals();
    CreateApp();
}


iCrackedApp::~iCrackedApp() {}


void iCrackedApp::CreateApp() {
    
    const float BOUNDARY_THICKNESS = 600.0f;
    
    const float PLANE_WIDTH = 4096;
    
    for (float x = -PLANE_WIDTH; x < PLANE_WIDTH; x += BOUNDARY_THICKNESS) {
        for (float z = -PLANE_WIDTH; z < PLANE_WIDTH; z += BOUNDARY_THICKNESS) {
            //AddFloorCube(0.5 * BOUNDARY_THICKNESS - 5.0f, Vec3(x, -worldSize.Height(), z));
            AddMesh(ICRACKED_RESOURCE_FILE, hd::randomf(20.0f, 100.0f), Vec3(x, -200.0f, z));
        }
    }
    
    CreateLights();
    ConstructSkybox(1);
}
