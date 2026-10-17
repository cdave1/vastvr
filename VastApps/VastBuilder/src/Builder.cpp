#include "Builder.h"
#include "BuilderSerialization.h"

using namespace hd::input;

// TODO: Put these #defines and constants into a separate config file.
#define MESH_RESOURCE_FILE ""
#define COLLISION_VOLUME_NAME "COLLISION_VOLUME_NAME"
#define BUILDER_ENTITY_NAME "Block"
#define WORLD_UI_ENTITY_NAME "WorldUI"
#define LINES_UI_ENTITY_NAME "LinesUI"

const float COLLISION_PLANE_SIZE = 5000.0f * 0.01f;
const float COLLISION_NODE_SIZE = COLLISION_PLANE_SIZE / 100.0f;
const float GAP_SIZE = 3.0f * 0.01f;
const Vec3 GRID_NODE_SIZE(COLLISION_NODE_SIZE - GAP_SIZE, GAP_SIZE, COLLISION_NODE_SIZE - GAP_SIZE);
const Vec3 GRID_GAP_SIZE(GAP_SIZE, GAP_SIZE, GAP_SIZE);

static GridSnapOption m_gridSnapOption = GridSnapOption::Cube;// ::Plane;

static Collision3 lastLookAtCollision;

static Collision3 startingCollision;

static hd::Entity *m_currentGeometry = nullptr;

static OnPaintDragOption m_onPaintDragOption = OnPaintDragOption::Paint;

static Vec3 paintStartOrigin;



Builder::Builder(hd::application::Client *client) :
AppPrototype(client),
m_cullingSystem(this->GetEntityPool(), client),
m_dashboardRenderer(this->GetEntityPool(), client, DASHBOARD_BUTTON_NAME),
m_dashboardMenu(this, this->GetEntityPool()),
m_collisionInputSystem(this->GetEntityPool(), client, COLLISION_VOLUME_NAME),
m_collisionVolumeRenderer(this->GetEntityPool(), client, COLLISION_VOLUME_NAME),
m_instancedRenderer(this->GetEntityPool(), client, BUILDER_ENTITY_NAME),
m_worldUIRenderer(this->GetEntityPool(), client, WORLD_UI_ENTITY_NAME),
m_lineRenderer(this->GetEntityPool(), client, LINES_UI_ENTITY_NAME),
m_dashboardInputSystem(this->GetEntityPool(), client, DASHBOARD_MENU_ITEM_NAME) {
    hd::Components::CreateLargePool<Counter>();


    auto onRestoreFunction = [this](hd::Entity *entity) {
        entity->Add<hd::input::pointer::ResponderState>();
        //entity->Add<hd::input::pointer::SelectResponder>();
        //entity->Add<hd::input::pointer::DragResponder>();
        //entity->Add<hd::input::pointer::ReleaseResponder>();
        entity->Add<hd::input::LookAtResponder>();

        entity->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &Builder::OnPaintItemLookAtBegin );
        entity->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &Builder::OnPaintItemLookAtHover);
        entity->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &Builder::OnPaintItemLookAtEnd);
        entity->Edit<hd::input::LookAtResponder>()->OnLookAt([this](const hd::Entity *entity, const hd::input::Headset *headset, const Collision3 &collision) {
            this->OnGeometryLookAt(entity, headset, collision);
        });

        //entity->Get<hd::input::pointer::SelectResponder>()->OnSelect(this, &Builder::OnPaintItemSelect);
        //entity->Get<hd::input::pointer::DragResponder>()->OnDrag(this, &Builder::OnPaintItemDrag);
        //entity->Get<hd::input::pointer::ReleaseResponder>()->OnReleaseInside(this, &Builder::OnPaintItemReleaseInside);
        //entity->Get<hd::input::pointer::ReleaseResponder>()->OnRelease(this, &Builder::OnPaintItemRelease);
    };
    BuilderSerialization::LoadFromJSON(this->GetEntityPool(), onRestoreFunction);
    InitUI();

    SetGridSnapOption(GridSnapOption::Plane);
    RefreshGhostItem();
}


void Builder::InitUI() {
    { // Target
        m_target = GetEntityPool()->Create();
        m_target->SetName(WORLD_UI_ENTITY_NAME);

        auto transform = m_target->Add<hd::graphics::Transform>();
        auto shader = m_target->Add<hd::graphics::Shader>();
        auto meshRef = m_target->Add<hd::graphics::MeshReference>();
        auto material = m_target->Add<hd::graphics::Material>();
        auto opacity = m_target->Add<hd::graphics::OpacityData>();
        auto collisionData = m_target->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        transform->SetOrigin(Vec3(-5000.0f, -5000.0f, -5000.0f));
        transform->SetRotation(Quaternion(Vec3(1, 0, 0), hd_half_pi));

        AABB cubeSize(0.1f, 0.1f, Vec3());

        shader->LoadShader("decal.vert", "decal.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(cubeSize.Size());
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);

        collisionData->SetMeshData(meshData);

        hd::Texture *texture = hd::TextureManager::Instance()->FindTexture("resources/images/reticule.png");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
        material->SetSpecular(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(1.0f);
        material->SetTexture(texture);

        opacity->SetOpacity(1.0f);

        m_target->Remove<hd::input::pointer::ResponderState>();
        m_target->Remove<hd::input::pointer::SelectResponder>();
        m_target->Remove<hd::input::pointer::DragResponder>();
        m_target->Remove<hd::input::pointer::ReleaseResponder>();
    }

    CreateCollisionPlane();

    ConstructSkybox(3);
    CreateLights();

    hd::graphics::MeshLoader *loader = hd::graphics::MeshLoader::Instance();
    hd::graphics::MeshData *mesh = loader->GetMeshFromFile (MESH_RESOURCE_FILE);

    if (mesh) {
        for (int i = 0; i < mesh->GetMeshes()->Size(); ++i) {
            auto m = mesh->GetMeshes()->At(i);
            //m->GenerateNormals();
            //m->FlipNormals();
        }
    }

    { // Gap placement grid
        auto gapGrid = GetEntityPool()->Create();
        gapGrid->SetName(LINES_UI_ENTITY_NAME);

        auto transform = gapGrid->Add<hd::graphics::Transform>();
        auto shader = gapGrid->Add<hd::graphics::Shader>();
        auto meshRef = gapGrid->Add<hd::graphics::MeshReference>();
        auto material = gapGrid->Add<hd::graphics::Material>();

        const Vec3 GRID_SIZE(10.0f);
        const Vec3 GRID_NODE_SIZE(COLLISION_NODE_SIZE);

        shader->LoadShader("grid.vert", "grid.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetGapGrid(GRID_SIZE, GRID_NODE_SIZE);
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(0.0f);

        m_gapGrid = gapGrid;
    }

    /*
     void Builder::OnCameraSelect(hd::Entity *entity, const Collision3 &collision) {
     hd::log::i("On Camera Select\n");
     }


     void Builder::OnCameraRelease(hd::Entity *entity, const Collision3 &collision) {
     hd::log::i("On Camera Release\n");
     }

     */
    auto select = Cameras::mainCamera->Edit<hd::input::pointer::SelectResponder>();
    auto move = Cameras::mainCamera->Edit<hd::input::pointer::MoveResponder>();
    auto drag = Cameras::mainCamera->Edit<hd::input::pointer::DragResponder>();
    auto release = Cameras::mainCamera->Edit<hd::input::pointer::ReleaseResponder>();

    move->OnMove(this, &Builder::OnPointerMove);
    select->OnSelect(this, &Builder::OnPointerSelect);
    drag->OnDrag(this, &Builder::OnPointerDrag);
    release->OnRelease(this, &Builder::OnPointerRelease);

    auto headset = Cameras::mainCamera->GetComponent<Headset>();
    m_pointerCollision = Collision3(Vec3(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f));

	m_currentHue = Color4f(0.3f, 0.6f, 0.9f, 1.0f);
}


void Builder::OnGridSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());

    if (element == m_collisionVolume) {
        startingCollision = collision;
    }
}


void Builder::OnGridDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    hd::log::i("%s: %d (%3.3f, %3.3f, %3.3f)\n", __FUNCTION__, element->GetID(),
               delta[0], delta[1], delta[2]);
}


void Builder::OnGridRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
}


void Builder::OnGridReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
}


void Builder::OnPaintItemSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
}


void Builder::OnPaintItemDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());

    if (currentPaintOption == PaintOption::PaintOnTarget) {
        Vec3 normal = hd::normalize(collision.normal);
        auto transform = m_target->Edit<hd::graphics::Transform>();
        transform->SetOrigin(collision.origin + (50.0f * normal));
    }
}


void Builder::OnPaintItemRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
}


void Builder::OnPaintItemReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());

    if (currentPaintOption == PaintOption::PaintOnTarget) {
    //    OnConfirmAction(element, startingCollision, collision);
    }
}


void Builder::OnPaintItemLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());

    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();

    if (timer && scaleBlock && transform) {
        if (timer->IsActive()) {
            transform->SetScale(1.2f);
        } else {
            scaleBlock->SetStartScale(1.0f);
            scaleBlock->SetEndScale(1.2f);

            timer->SetDuration(0.10f);
            timer->Start();

            std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
            hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void Builder::OnPaintItemLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    if (currentPaintOption == PaintOption::PaintOnTarget) {
        Vec3 normal = hd::normalize(collision.normal);

        if (element != m_ghostMesh) {
            Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(collision.normal), Vec3(0, 1, 0));
            rotation.Invert();

            lastLookAtCollision = collision;
            m_currentPaintTarget = element;

            if (m_ghostMesh) {
                auto xform = m_ghostMesh->Edit<hd::graphics::Transform>();
                xform->SetOrigin(collision.origin - (GRID_NODE_SIZE[0] * 0.5 * normal));
                xform->SetRotation(rotation);
            }
        }
    } else {
        m_currentPaintTarget = element;
    }
}


void Builder::OnPaintItemLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());

    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();

    if (timer && scaleBlock && transform) {
        if (timer->IsActive()) {
            transform->SetScale(1.0f);
        } else {
            scaleBlock->SetStartScale(1.2f);
            scaleBlock->SetEndScale(1.0f);

            timer->SetDuration(0.10f);
            timer->Start();
        }
    }
}


void Builder::OnGeometryLookAt(const hd::Entity *element, const hd::input::Headset *headset, const Collision3 &collision) {
    m_currentGeometry = const_cast<hd::Entity *>(element);

    //Vec3 normal = hd::normalize(collision.normal);
    //Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(collision.normal), Vec3(0, 1, 0));
    //rotation.Invert();
}


#pragma mark - Pointer selection stuff

void Builder::OnPointerMove(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta) {
    m_pointerCollision = collision;
}


void Builder::OnPointerSelect(hd::Entity *entity, const Collision3 &collision) {
    hd::log::i("On Camera Select\n");
    if (currentSelectAction == SelectAction::Paint) {
        BeginPaintSession();
    }
}


void Builder::OnPointerDrag(hd::Entity *entity, const Collision3 &collision, const Vec3 &delta) {
    m_pointerCollision = collision;
    if (currentSelectAction == SelectAction::Paint) {
        OnPaint();
    }
}


void Builder::OnPointerRelease(hd::Entity *entity, const Collision3 &collision) {
    hd::log::i("On Camera Release\n");
    if (currentSelectAction == SelectAction::Paint) {
        EndPaintSession();
    } else if (currentSelectAction == SelectAction::Erase) {
        OnErase(m_currentPaintTarget);
    }
}


#pragma mark - Menu selection

void Builder::OnSelectPaint(hd::Entity *element, const Collision3 &collision) {
    currentSelectAction = SelectAction::Paint;

    hd::Texture *texture = hd::TextureManager::Instance()->FindTexture("resources/images/pencil.png");

    auto material = m_target->Edit<hd::graphics::Material>();
    material->SetTexture(texture);

    RefreshGhostItem();
}


void Builder::OnSelectErase(hd::Entity *element, const Collision3 &collision) {
    currentSelectAction = SelectAction::Erase;

    hd::Texture *texture = hd::TextureManager::Instance()->FindTexture("resources/images/eraser.png");

    auto material = m_target->Edit<hd::graphics::Material>();
    material->SetTexture(texture);

    if (m_ghostMesh) {
        GetEntityPool()->Free(m_ghostMesh);
    }
    m_ghostMesh = nullptr;
}


void Builder::OnSelectCube(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Cube;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectSphere(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Sphere;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectCylinder(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Cylinder;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectHexagon(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Hexagon;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectQuad(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Quad;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectMesh(hd::Entity *element, const Collision3 &collision) {
    currentPaletteItem = PaletteItem::Mesh;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::OnSelectWarp(hd::Entity *element, const Collision3 &collision) {
    currentSelectAction = SelectAction::Warp;
}


void Builder::OnSelectHue(hd::Entity *element, const Collision3 &collision) {
    //GetEntityPool()->Free(m_ghostMesh);
    //m_ghostMesh = Create(Vec3());
/*
    auto material = m_ghostMesh->Get<hd::graphics::Material>();
    if (material) {
        material->SetAmbient(m_currentHue);
    }*/
}


void Builder::OnSave(hd::Entity *element, const Collision3 &collision) {
    BuilderSerialization::SaveToJSON(this->GetEntityPool(), BUILDER_ENTITY_NAME);
}


void Builder::OnNewWorld(hd::Entity *element, const Collision3 &collision) {
    if (!BuilderSerialization::SaveToJSON(this->GetEntityPool(), BUILDER_ENTITY_NAME)) return;
    EntityQuery *q = GetEntityPool()->QueryByName(BUILDER_ENTITY_NAME);
    for (int i = 0; i < q->Size(); ++i) {
        Entity *entity = q->At(i);
        GetEntityPool()->Free(entity);
    }
    m_ghostMesh = nullptr;
    RefreshGhostItem();
}


void Builder::OnUndo(hd::Entity *element, const Collision3 &collision) {
    if (m_undoStack.Size() > 0) {
        hd::Entity *top = m_undoStack.Top();
        if (top) {
            m_undoStack.Remove(top);
            GetEntityPool()->Free(top);

            std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/undo.wav");
            hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void Builder::SetGridSnapOption(GridSnapOption option) {
    m_gridSnapOption = option;
    //RefreshGhostItem();

    if (!m_gridSnapGuideMesh) {
        m_gridSnapGuideMesh = GetEntityPool()->Create();
        m_gridSnapGuideMesh->SetName(LINES_UI_ENTITY_NAME);

        auto transform = m_gridSnapGuideMesh->Add<hd::graphics::Transform>();
        auto shader = m_gridSnapGuideMesh->Add<hd::graphics::Shader>();
        auto meshRef = m_gridSnapGuideMesh->Add<hd::graphics::MeshReference>();
        auto material = m_gridSnapGuideMesh->Add<hd::graphics::Material>();

        shader->LoadShader("grid.vert", "grid.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(0.0f);
    }

    auto meshRef = m_gridSnapGuideMesh->Edit<hd::graphics::MeshReference>();
    auto shader = m_gridSnapGuideMesh->Edit<hd::graphics::Shader>();

    if (m_gridSnapOption == GridSnapOption::Sphere) {
        auto meshData = hd::graphics::MeshLoader::Instance()->GetSphere(1.0f, 48);
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);
    } else if (m_gridSnapOption == GridSnapOption::Cube) {
        auto meshData = hd::graphics::MeshLoader::Instance()->GetSimpleCube(Size3f(1.0f));
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);
    } else if (m_gridSnapOption == GridSnapOption::Plane) {
        auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(AABB(Vec3(-1.0f, -1.0f, -0.9f), Vec3(1.0f, 1.0f, -1.1f)));
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);
    } else if (m_gridSnapOption == GridSnapOption::CurvedQuad) {
        auto meshData = hd::graphics::MeshLoader::Instance()->GetCurvedQuad(0.75f * hd_pi, 10.0f, 10.0f, 36);
        meshRef->SetMeshReference(meshData);
        meshRef->GetMeshData()->SetDefaultAttributeLocations(shader);
    }
}


void Builder::SetPaintDragOption(OnPaintDragOption option) {
    m_onPaintDragOption = option;
}


void Builder::SetCurrentHue(const Color4f &hue) {
    m_currentHue = hue;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::SetCurrentTexture(hd::Texture *texture) {
    m_currentTexture = texture;

    if (currentSelectAction == SelectAction::Paint) {
        RefreshGhostItem();
    }
}


void Builder::GridLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
    {
        //auto transform = m_target->Edit<hd::graphics::Transform>();
        //transform->SetOrigin(collision.origin);
    }
}


void Builder::GridLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
   /* if (m_currentPaintTarget == nullptr) {
        auto collisionVolume = element->Get<hd::graphics::CollisionVolume>();

        {
            Vec3 normal = hd::normalize(collision.normal);

            Quaternion rotation = Quaternion::LookAt(collision.origin, collision.origin + (5.0f * normal), Vec3(0, 1, 0));
            //Quaternion invertedRotation = rotation * Quaternion(Vec3(1, 0, 0), hd_half_pi);
            //invertedRotation.Invert();

            auto transform = m_target->Edit<hd::graphics::Transform>();
            transform->SetRotation(rotation); //invertedRotation * Quaternion(Vec3(1, 0, 0), hd_half_pi));
            transform->SetOrigin(collision.origin + (-5.0f * normal));
        }
    }*/
}


void Builder::GridLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    hd::log::i("%s: %d\n", __FUNCTION__, element->GetID());
    if (m_currentPaintTarget == nullptr) {
        auto transform = m_target->Edit<hd::graphics::Transform>();
        transform->SetOrigin(Vec3(10000.0f, 10000.0f, 10000.0f));
    }
}


void Builder::WarpTo(hd::Entity *camera) {
    auto headset = camera->Edit<Headset>();
    auto rayTracer = camera->GetComponent<hd::graphics::RayTracer>();

    if (m_ghostMesh) {
        auto transform = m_ghostMesh->Edit<hd::graphics::Transform>();
        headset->SetPosition(transform->GetOrigin());

        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/warp.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
    }
}


#pragma mark - Update

void Builder::Update(Entity *camera) {
    hdAssert(GetEntityPool());
    hdAssert(m_gapGrid);
    hdAssert(m_gridSnapGuideMesh);

    GetEntityPool()->Update();

    m_cullingSystem.Update(camera);

    m_currentPaintTarget = nullptr;

    auto headset = camera->GetComponent<Headset>();
    auto rayTracer = camera->GetComponent<hd::graphics::RayTracer>();

    Vec3 pointerToWorld = GetPointerWorldPosition();

    Vec3 origin, reticulePosition;

    Collision3 pointerGridMeshCollision;
    bool collidesWithGridMesh = false;



    { // Check if the current pointer collides with the grid mesh.
        auto mesh = m_gridSnapGuideMesh->Get<hd::graphics::MeshReference>();
        auto collisionData = m_gridSnapGuideMesh->Get<hd::graphics::CollisionData>();
        auto xform = m_gridSnapGuideMesh->Get<hd::graphics::Transform>();

        // TODO: Split this update function so its obvious what is happening.
        collidesWithGridMesh = hd::graphics::RayHitTester::TestRayMeshData(headset->GetPosition(), pointerToWorld,
                                                                                mesh->GetMeshData(),
                                                                                &pointerGridMeshCollision,
                                                                                xform->GetMatrix());
    }

    Vec3 lookAtPosition(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f);

    Collision3 headsetCollision;
    Vec3 headsetCollisionLocation = rayTracer->PointerToWorld(lookAtPosition,
                                                              headset->GetViewport(),
                                                              headset->GetProjection(),
                                                              headset->GetLookAtMatrix());


    //RayIntersectsSphere(headset->GetPosition(), pointerGridMeshCollision.origin,
    //                    headset->GetPosition(), 0.8f * m_ghostMeshRadius, &reticulePosition);


	if (currentPaintOption == PaintOption::FreePaint) {
		Vec3 normal = hd::normalize(headsetCollision.origin);
		Vec3 pos = headset->GetPosition() + (100.0f * normal);
		reticulePosition = pos;

		RayIntersectsSphere(headset->GetPosition(),
			headsetCollisionLocation, //pointerGridMeshCollision.origin,
			headset->GetPosition(), 
			0.8f * m_ghostMeshRadius, 
			&reticulePosition);
	} else if (currentPaintOption == PaintOption::PaintOnTarget) {
        Vec3 reticuleOffset = (50.0f * m_paintItemScale * hd::normalize(lastLookAtCollision.normal));
        reticulePosition = lastLookAtCollision.origin - reticuleOffset;
    } else if (currentPaintOption == PaintOption::SnapToGrid) {
        Vec3 reticuleOffset = (50.0f * m_paintItemScale * hd::normalize(pointerGridMeshCollision.normal));
        reticulePosition = pointerGridMeshCollision.origin - reticuleOffset;
    }


    // Place the ghostItem at the position the user is looking at.
    if (m_ghostMesh) {
        auto collisionVolume = m_collisionVolume->Edit<hd::graphics::CollisionVolume>();

        if (!m_dashboardMenu.IsMenuVisible()) {
            if (collidesWithGridMesh) {
				if (currentPaintOption == PaintOption::FreePaint) {
					m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(reticulePosition);

					Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(reticulePosition), Vec3(0, 1, 0));
					rotation.Invert();
					m_ghostMesh->Edit<hd::graphics::Transform>()->SetRotation(rotation);

				} else if (currentPaintOption == PaintOption::SnapToGrid) {
                    Collision3 coll;
                    coll.origin = pointerGridMeshCollision.origin;

                    // Idea: invert the mesh rotation.  We rotate the collision point by this to get its virtual point
                    // within the world space aligned collision volume.
                    //
                    // Once we have the collision volume position, multiply this position by the original rotation.
                    Vec3 gridSpaceOrigin = m_gridSnapGuideMesh->Get<hd::graphics::Transform>()->GetOrigin();
                    Quaternion gridSpaceRotation = m_gridSnapGuideMesh->Get<hd::graphics::Transform>()->GetRotation();
                    Quaternion gridSpaceRotationInverse = gridSpaceRotation;
                    gridSpaceRotation.Invert();

                    coll.origin = (gridSpaceRotationInverse * (pointerGridMeshCollision.origin - gridSpaceOrigin)) + (hd::normalize(pointerGridMeshCollision.normal) * 5.0f);

                    Vec3 position = collisionVolume->GetCollisionCenter(coll);
                    position = (gridSpaceRotation * position);

                    m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(position + gridSpaceOrigin);

                    Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(pointerGridMeshCollision.normal), Vec3(0, 1, 0));
                    rotation.Invert();
                    m_ghostMesh->Edit<hd::graphics::Transform>()->SetRotation(gridSpaceRotationInverse); //m_gridSnapGuideMesh->Get<hd::graphics::Transform>()->GetRotation());
                    
                    
                } else {
                    m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(lastLookAtCollision.origin);
                }
            } else {
				if (currentPaintOption == PaintOption::FreePaint) {
					m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(reticulePosition);

					Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(reticulePosition), Vec3(0, 1, 0));
					rotation.Invert();
					m_ghostMesh->Edit<hd::graphics::Transform>()->SetRotation(rotation);
				}
				else if (currentPaintOption == PaintOption::SnapToGrid) {
                    Collision3 coll;
                    coll.origin = pointerGridMeshCollision.origin;
                    Vec3 position = collisionVolume->GetCollisionCenter(coll);
                    m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(position);
                } else {
                    m_ghostMesh->Edit<hd::graphics::Transform>()->SetOrigin(pointerGridMeshCollision.origin);
                }
            }
        }

        m_ghostMesh->Edit<hd::graphics::Transform>()->SetScale(m_paintItemScale);
        m_ghostMesh->Edit<hd::graphics::Transform>()->Update();

        // Hide the ghost item if the menu is visible.
        if (m_ghostMesh->Get<hd::graphics::OpacityData>()) {
            if (m_dashboardMenu.IsMenuVisible()) {
                m_ghostMesh->Edit<hd::graphics::OpacityData>()->SetOpacity(0.0f);
            } else {
                m_ghostMesh->Edit<hd::graphics::OpacityData>()->SetOpacity(1.0f);
            }
        }
    }

    // If we're not looking at geometry, place the targetting reticule in the middle of the screen.
    //if (!m_currentGeometry) {
//        Quaternion rotation = headset->GetWorldOrientation();
  //      rotation.Invert();

        Quaternion rotation = Quaternion::LookAt(headset->GetPosition(), pointerGridMeshCollision.origin, Vec3(0, 1, 0));
        rotation.Invert();

        auto transform = m_target->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);
        transform->SetOrigin(reticulePosition);
    //}

    // Position the grid such that it is always centered on the headset location
    if (!m_paintSessionStarted) {
        auto headset = camera->GetComponent<Headset>();
        auto collisionVolume = m_collisionVolume->Edit<hd::graphics::CollisionVolume>();

        if (currentPaintOption == PaintOption::SnapToGrid) {
            Collision3 coll;
            coll.origin = headset->GetWorldPosition();
            Vec3 position = collisionVolume->GetCollisionCenter(coll);

            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetOrigin(position);
            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetScale(m_ghostMeshRadius);
            m_gapGrid->Edit<hd::graphics::Transform>()->SetOrigin(position);
        } else {
            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetOrigin(headset->GetWorldPosition());
            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetScale(m_ghostMeshRadius);
            m_gapGrid->Edit<hd::graphics::Transform>()->SetOrigin(headset->GetWorldPosition());
        }

        m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->Update();
        m_gapGrid->Edit<hd::graphics::Transform>()->Update();

        // HACK HACK HACK -- this is lazy, done specifically to avoid having another entity system.
        m_gapGrid->Edit<hd::graphics::Shader>()->Begin();
        m_gapGrid->Edit<hd::graphics::Shader>()->SetUniform3f("GridCollisionPosition", pointerGridMeshCollision.origin);
    }

    if (!m_paintSessionStarted) {
        Quaternion headsetOrientation = headset->GetWorldOrientation();
        headsetOrientation.Invert();

        if (currentPaintOption == PaintOption::SnapToGrid) {
            Quaternion snapRotation;
            Vec3 snapPosition;

            m_sphereLayout.NearestPosition(&snapPosition, &snapRotation, headsetCollisionLocation - headset->GetPosition());
            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetRotation(snapRotation);
        } else {
            m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->SetRotation(headsetOrientation);
        }
        m_gridSnapGuideMesh->Edit<hd::graphics::Transform>()->Update();
    }

    m_animationSystem.Update();
    m_alarmSystem.Update();
    if (m_dashboardMenu.IsMenuVisible()) m_dashboardInputSystem.Update(camera);
    //m_keyboardInputSystem.Update(camera);
    m_blockInputSystem.Update(camera);
    m_skyboxRenderer.Update(camera);
    m_deferredLightSystem.Update(camera);

    m_transformSystem.Update();
    
    m_currentGeometry = nullptr;
}


void Builder::HandleInput(Entity *camera) {
    const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();
    for (int i = 0; i < events->Size(); ++i) {
        Entity *event = events->At(i);

        auto baseEvent = event->GetComponent<BaseEvent>();
        hdAssert(baseEvent);
        hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
        if (baseEvent->IsConsumed()) {
            continue;
        }

        if (event->ContainsComponent<gamepad::ControlSelect>()) {
            auto control = event->GetComponent<gamepad::ControlSelect>();
            if (control->GetControl() == gamepad::Control::LEFT_TRIGGER) {
                //m_sphereMenu.ShowMenu(GetEntityPool(), camera);
            }

            if (control->GetControl() == gamepad::Control::RIGHT_TRIGGER) {
                currentPaintOption = PaintOption::SnapToGrid;
            }

			if (control->GetControl() == gamepad::Control::XBOX_A) {
				if (currentSelectAction == SelectAction::Paint) {
					BeginPaintSession();
				}
			}

            if (control->GetControl() == gamepad::Control::XBOX_B ||
                control->GetControl() == gamepad::Control::PLAYSTATION_CIRCLE) {
				
            }
        }

        if (event->ContainsComponent<gamepad::ControlRelease>()) {
            auto control = event->GetComponent<gamepad::ControlRelease>();
            if (control->GetControl() == gamepad::Control::LEFT_TRIGGER) {
				m_dashboardMenu.ToggleMenu(); //.HideMenu(GetEntityPool(), camera);
                //m_sphereMenu.HideMenu(GetEntityPool(), camera);
            }

            if (control->GetControl() == gamepad::Control::RIGHT_TRIGGER) {
                currentPaintOption = PaintOption::PaintOnTarget;
                m_currentPaintTarget = nullptr;
            }

            if (control->GetControl() == gamepad::Control::XBOX_A ||
                control->GetControl() == gamepad::Control::PLAYSTATION_CROSS) {
                if (currentSelectAction == SelectAction::Paint) {
					EndPaintSession();
                    //OnPaint(m_currentPaintTarget);
                } else if (currentSelectAction == SelectAction::Erase) {
                    OnErase(m_currentPaintTarget);
                }
            }
            if (control->GetControl() == gamepad::Control::XBOX_B ||
                control->GetControl() == gamepad::Control::PLAYSTATION_CIRCLE) {
                //OnUndo(camera, Collision3);
            }
            if (control->GetControl() == gamepad::Control::RIGHT_BUMPER) {
                WarpTo(camera);
            }
        }

        if (event->ContainsComponent<keyboard::KeySelect>()) {
            auto key = event->Get<keyboard::KeySelect>();
            if (key) {
                if (key->GetKeyCode() == hd::Device::Keycode::TAB) {

                }
                if (key->GetKeyCode() == hd::Device::Keycode::SPACE) {
                    BeginPaintSession();
                }
            }
        }

        if (event->ContainsComponent<keyboard::KeyRelease>()) {
            auto key = event->Get<keyboard::KeyRelease>();
			key->PrintDebugInfo();
            if (key) {
                if (key->GetKeyCode() == hd::Device::Keycode::TAB) {
                    m_dashboardMenu.ToggleMenu();
                    EndPaintSession();
                    //m_keyboard.Hide(GetEntityPool(), camera);
                } else if (key->GetKeyCode() == hd::Device::Keycode::RETURN) {
                    WarpTo(camera);
                } else if (key->GetKeyCode() == hd::Device::Keycode::SPACE) {
                    if (currentSelectAction == SelectAction::Paint) {
                        EndPaintSession();
                    } else if (currentSelectAction == SelectAction::Erase) {
                        OnErase(m_currentPaintTarget);
                    }
                } else if (key->GetKeyCode() == hd::Device::Keycode::G) {
                    auto headset = Cameras::mainCamera->Edit<Headset>();
                    HMD *hmd = m_client->GetEventQueue()->GetCurrentHMD();
                    hmd->RecenterDevice();

                    headset->SetOrientationOffset(Quaternion(Vec3(0, 1, 0), 0.0f));
                    headset->SetOrientation(hmd->GetDeviceOrientation());
                }
            }
        }

        const float RADIUS_MODIFICATION_INCREMENT = 3.0f;
        const float SCALE_MODIFICATION_INCREMENT = 0.01f;
        const Quaternion X_ROTATION_MODIFICATION_INCREMENT(Vec3(1, 0, 0), 0.01f);
        const Quaternion Y_ROTATION_MODIFICATION_INCREMENT(Vec3(0, 1, 0), 0.01f);

        if (event->ContainsComponent<device::Keyboard>()) {
            auto keyboard = event->Get<device::Keyboard>();
            if (keyboard->IsKeyDown(hd::Device::Keycode::Z)) {
                m_ghostMeshRadius += RADIUS_MODIFICATION_INCREMENT;
            }
            if (keyboard->IsKeyDown(hd::Device::Keycode::X)) {
                m_ghostMeshRadius -= RADIUS_MODIFICATION_INCREMENT;
            }

            if (keyboard->IsKeyDown(hd::Device::Keycode::C)) {
                m_paintItemScale = max(SCALE_MODIFICATION_INCREMENT, m_paintItemScale + SCALE_MODIFICATION_INCREMENT);
            }
            if (keyboard->IsKeyDown(hd::Device::Keycode::V)) {
                m_paintItemScale = max(SCALE_MODIFICATION_INCREMENT, m_paintItemScale - SCALE_MODIFICATION_INCREMENT);
            }

            if (keyboard->IsKeyDown(hd::Device::Keycode::KEY_1)) {
                currentPaintOption = PaintOption::PaintOnTarget;
            } else {
               // currentPaintOption = PaintOption::SnapToGrid;
            }

            if (keyboard->IsKeyDown(hd::Device::Keycode::A)) {
                m_paintItemRotation = m_paintItemRotation * X_ROTATION_MODIFICATION_INCREMENT;
            }

            if (keyboard->IsKeyDown(hd::Device::Keycode::S)) {
                m_paintItemRotation = m_paintItemRotation * Y_ROTATION_MODIFICATION_INCREMENT;
            }

            if (keyboard->IsKeyDown(hd::Device::Keycode::KEY_2)) {
                m_paintItemRotation = Quaternion(Vec3(0, 0, 0), 0);
            }

            // Always paint while space is held down.
            //
            // How to avoid painting in the same spot repeatedly without using grid coordinates?
            //
            // This is a tricky system but the solution is as follows:
            // 1. Start a paint session
            // 2. Whenever a new item is due to be painted, check if that new item will COLLIDE with a previous item
            //    painted in the session.  If so, then don't paint it.
            // 3. Otherwise, paint it and add to the session list.
            // 4. Goto 2 until the the paint key is released.
            // 5. Close the session.
            if (keyboard->IsKeyDown(hd::Device::Keycode::SPACE)) {
                if (currentSelectAction == SelectAction::Paint) {
                    OnPaint();
                }
            }
        }

		if (event->ContainsComponent<device::Gamepad>()) {
			auto gamepad = event->Get<device::Gamepad>();

			const float ROTATION_SPEED = 0.025f;
			const float MOVEMENT_SPEED = 10.0f;

			if (gamepad->IsControlActive(gamepad::Control::LEFT_THUMBSTICK)) {
				auto axis = gamepad->GetAxis(gamepad::Control::LEFT_THUMBSTICK);

			}

			if (gamepad->IsControlActive(gamepad::Control::XBOX_A)) {
				OnPaint();
			}

			if (gamepad->IsControlActive(gamepad::Control::RIGHT_THUMBSTICK)) {
				auto axis = gamepad->GetAxis(gamepad::Control::RIGHT_THUMBSTICK);

				Quaternion vert(Vec3(0, 1, 0), -axis->GetDirection()[0] * ROTATION_SPEED);
				//Quaternion horiz(Vec3(1, 0, 0), axis->GetDirection()[1] * ROTATION_SPEED);

				Quaternion rotXYZ = vert; // * horiz;
				//headset->RotateBy(rotXYZ);
			}
		}
    }

    m_deferredLightSystem.HandleInput();
    
    m_instancedRenderer.Update(camera);
    m_dashboardRenderer.Update(camera);
    m_worldUIRenderer.Update(camera);
}


void Builder::DrawDepth(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    m_instancedRenderer.RenderDepth(camera, eyePosition);
}


void Builder::Draw(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    // Frustum culling functions performed here:


    m_instancedRenderer.RenderOpaque(camera, eyePosition);
    m_worldUIRenderer.RenderOpaque(camera, eyePosition);

    m_instancedRenderer.RenderTransparent(camera, eyePosition);

    m_dashboardRenderer.RenderOpaque(camera, eyePosition);

    //m_keyboardRenderer.RenderOpaque(camera, eyePosition);
    //m_keyboardRenderer.RenderTransparent(camera, eyePosition);


    m_skyboxRenderer.Draw(camera, eyePosition);


    m_worldUIRenderer.RenderTransparent(camera, eyePosition);
    m_dashboardRenderer.RenderTransparent(camera, eyePosition);
    m_labelRenderer.Update(camera, eyePosition);
    m_lineRenderer.RenderLines(camera, eyePosition);
}


void Builder::OnErase(hd::Entity *element) {
    hdAssert(currentSelectAction == SelectAction::Erase);
    if (m_dashboardMenu.IsMenuVisible()) return;
    if (element && element == m_currentPaintTarget) {
        GetEntityPool()->Free(element);
    }
}


static const bool randomizeBrushSize = false;

void Builder::OnPaint() {
    hdAssert(currentSelectAction == SelectAction::Paint);
    if (m_dashboardMenu.IsMenuVisible()) return;

    Collision3 releaseCollision = lastLookAtCollision;
    if (m_currentPaintSession.Contains(m_currentPaintTarget) || m_currentPaintTarget == m_ghostMesh) {
        return;
    }

    if (m_onPaintDragOption == OnPaintDragOption::Resize && m_currentPaintSession.Size() == 1) {
        Entity *item = m_currentPaintSession.At(0);

        auto transform = item->Edit<hd::graphics::Transform>();
        auto collisionVolume = m_collisionVolume->Edit<hd::graphics::CollisionVolume>();

        Collision3 startCollision, endCollision;

        Vec3 start, end, position;

        {
            Vec3 gridSpaceOrigin = m_gridSnapGuideMesh->Get<hd::graphics::Transform>()->GetOrigin();
            Quaternion gridSpaceRotation = m_gridSnapGuideMesh->Get<hd::graphics::Transform>()->GetRotation();
            Quaternion gridSpaceRotationInverse = gridSpaceRotation;
            gridSpaceRotation.Invert();

            startCollision.origin = (gridSpaceRotationInverse * (paintStartOrigin - gridSpaceOrigin));
            endCollision.origin = (gridSpaceRotationInverse * (m_ghostMesh->Get<hd::graphics::Transform>()->GetOrigin() - gridSpaceOrigin));

            start = collisionVolume->GetCollisionCenter(startCollision);
            end = collisionVolume->GetCollisionCenter(endCollision);
            position = (gridSpaceRotation * endCollision.origin) + gridSpaceOrigin;
        }

        Vec3 unit(GRID_NODE_SIZE[0], GRID_NODE_SIZE[0], GRID_NODE_SIZE[0]);

        transform->SetOrigin(paintStartOrigin - 0.5f * (paintStartOrigin - position));

        Vec3 diff = (end - start).abs() + Vec3(GRID_NODE_SIZE[0]);
        Vec3 min(m_paintItemScale);
        Vec3 scale = hd::max(min, m_paintItemScale * diff / GRID_NODE_SIZE[0]);

        transform->SetScale(scale);
    } else {
        Entity *newItem = Create(Vec3());

        if (newItem) {
            Vec3 position;
            auto transform = newItem->Edit<hd::graphics::Transform>();

            if (currentPaintOption == PaintOption::SnapToGrid) {
                auto collisionVolume = m_collisionVolume->Edit<hd::graphics::CollisionVolume>();
                Collision3 coll;
                coll.origin = m_ghostMesh->Get<hd::graphics::Transform>()->GetOrigin();
                position = m_ghostMesh->Get<hd::graphics::Transform>()->GetOrigin();// collisionVolume->GetCollisionCenter(coll);

                transform->SetRotation(m_ghostMesh->Get<hd::graphics::Transform>()->GetRotation());
                //transform->SetRotation(m_paintItemRotation);
            } else {
                AABB size = newItem->Get<hd::graphics::MeshReference>()->GetMeshData()->GetAABB();
                position = m_ghostMesh->Get<hd::graphics::Transform>()->GetOrigin(); //releaseCollision.origin - (size.Height() * transform->GetScale()[1] * 0.5 * hd::normalize(releaseCollision.normal));

				Quaternion rotation = m_ghostMesh->Get<hd::graphics::Transform>()->GetRotation();// Quaternion::LookAt(Vec3(0), hd::normalize(releaseCollision.normal), Vec3(0, 1, 0));
               // rotation.Invert();
                transform->SetRotation(m_paintItemRotation * rotation);
            }

            // If the new item's position is inside any of the items in the current paint session, then
            // discard the new item.
            for (int i = 0; i < m_currentPaintSession.Size(); ++i) {
                Entity *item = m_currentPaintSession.At(i);
                auto xform = item->Get<hd::graphics::Transform>();
                AABB aabb = newItem->Get<hd::graphics::MeshReference>()->GetMeshData()->GetAABB();
                Vec3 lower, upper;
                Matrix::Vec3Multiply(lower, aabb.lower, xform->GetMatrix());
                Matrix::Vec3Multiply(upper, aabb.upper, xform->GetMatrix());
                AABB xformedAABB(min(lower, upper), max(lower, upper));

                if (hd::PointInsideAABB(position, xformedAABB)) {
                    GetEntityPool()->Free(newItem);
                    return;
                }
            }

            transform->SetOrigin(position);
            if (randomizeBrushSize) {
                transform->SetScale(Vec3(hd::randomf(0.6f * m_paintItemScale, 1.4f * m_paintItemScale)));
            } else {
                transform->SetScale(Vec3(m_paintItemScale));
            }
            transform->Update();
            
            //m_undoStack.Add(newItem);
            if (m_currentPaintSession.Size() == 0) {
                paintStartOrigin = position;
            }
            m_currentPaintSession.Add(newItem);
        }
    }

    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}


hd::Entity * Builder::Create(const Vec3 &location) {
    Entity *newItem = nullptr;

    if (currentPaletteItem == PaletteItem::Cube
        || currentPaletteItem == PaletteItem::Quad
        || currentPaletteItem == PaletteItem::Sphere
        || currentPaletteItem == PaletteItem::Cylinder
        || currentPaletteItem == PaletteItem::Hexagon) {
        newItem = GetEntityPool()->Create();
        newItem->SetName(BUILDER_ENTITY_NAME);

        auto transform = newItem->Add<hd::graphics::Transform>();
        auto shader = newItem->Add<hd::graphics::Shader>();
        auto mesh = newItem->Add<hd::graphics::MeshReference>();
        auto material = newItem->Add<hd::graphics::Material>();
        auto collisionData = newItem->Add<hd::graphics::CollisionData>();
        auto opacityData = newItem->Add<hd::graphics::OpacityData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        if (m_currentTexture) {
            shader->LoadShader("blinnPhongGeometryInstanced.vert", "blinnPhongTexture.frag");
        } else {
            shader->LoadShader("blinnPhongGeometryInstanced.vert", "blinnPhongGeometry.frag");
        }

        hd::graphics::MeshData *meshData = nullptr;
        if (currentPaletteItem == PaletteItem::Cube) {
            meshData = hd::graphics::MeshLoader::Instance()->GetCube(Vec3(GRID_NODE_SIZE[0], GRID_NODE_SIZE[0], GRID_NODE_SIZE[0]));
        } else if (currentPaletteItem == PaletteItem::Quad) {
            meshData = hd::graphics::MeshLoader::Instance()->GetQuad(Vec3(GRID_NODE_SIZE[0], GRID_NODE_SIZE[0], GRID_NODE_SIZE[0]));
        } else if (currentPaletteItem == PaletteItem::Sphere) {
            meshData = hd::graphics::MeshLoader::Instance()->GetSphere(GRID_NODE_SIZE[0] * 0.5f, 24);
        } else if (currentPaletteItem == PaletteItem::Cylinder) {
            meshData = hd::graphics::MeshLoader::Instance()->GetCylinder(GRID_NODE_SIZE[0] * 0.5, GRID_NODE_SIZE[0], 24);
        } else if (currentPaletteItem == PaletteItem::Hexagon) {
            transform->SetRotation(Quaternion(Vec3(1, 0, 0), hd_half_pi));
            meshData = hd::graphics::MeshLoader::Instance()->GetCylinder(GRID_NODE_SIZE[0] * 0.5, GRID_NODE_SIZE[0], 6);
        }
        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        mesh->GetMeshData()->SetInstancedAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        material->SetAmbient(m_currentHue);
        material->SetDiffuse(Color4f(0.5f, 0.5f, 0.5f, 1.0f));
        material->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(10.0f);
        material->SetTexture(m_currentTexture);

    } else if (currentPaletteItem == PaletteItem::Mesh) {
        newItem = AddMesh(MESH_RESOURCE_FILE, 1.0f, location);

		if (newItem) {
			newItem->Edit<hd::graphics::Material>()->SetTexture(m_currentTexture);
			newItem->Edit<hd::graphics::Material>()->SetAmbient(m_currentHue);
			newItem->Edit<hd::graphics::Material>()->SetDiffuse(Color4f(0.5f, 0.5f, 0.5f, 1.0f));
			newItem->Edit<hd::graphics::Material>()->SetSpecular(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
			newItem->Edit<hd::graphics::Material>()->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
			newItem->Edit<hd::graphics::Material>()->SetShininess(10.0f);

			newItem->Edit<hd::graphics::Transform>()->Update();
		}
    }

    if (newItem) {
        newItem->Add<hd::input::pointer::ResponderState>();
        //newItem->Add<hd::input::pointer::SelectResponder>();
        //newItem->Add<hd::input::pointer::DragResponder>();
        //newItem->Add<hd::input::pointer::ReleaseResponder>();
        newItem->Add<hd::input::LookAtResponder>();

        newItem->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &Builder::OnPaintItemLookAtBegin );
        newItem->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &Builder::OnPaintItemLookAtHover);
        newItem->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &Builder::OnPaintItemLookAtEnd);
        newItem->Edit<hd::input::LookAtResponder>()->OnLookAt([this](const hd::Entity *entity, const hd::input::Headset *headset, const Collision3 &collision) {
            this->OnGeometryLookAt(entity, headset, collision);
        });

        //newItem->Get<hd::input::pointer::SelectResponder>()->OnSelect(this, &Builder::OnPaintItemSelect);
        //newItem->Get<hd::input::pointer::DragResponder>()->OnDrag(this, &Builder::OnPaintItemDrag);
        //newItem->Get<hd::input::pointer::ReleaseResponder>()->OnReleaseInside(this, &Builder::OnPaintItemReleaseInside);
        //newItem->Get<hd::input::pointer::ReleaseResponder>()->OnRelease(this, &Builder::OnPaintItemRelease);
    }
    
    return newItem;
}


void Builder::RefreshGhostItem() {
    auto newGhostItem = Create(Vec3());

	if (newGhostItem) {
		newGhostItem->Remove<hd::input::pointer::ResponderState>();
		newGhostItem->Remove<hd::input::pointer::SelectResponder>();
		newGhostItem->Remove<hd::input::pointer::DragResponder>();
		newGhostItem->Remove<hd::input::pointer::ReleaseResponder>();
		newGhostItem->Remove<hd::input::LookAtResponder>();

		if (m_ghostMesh) {
			auto xform = m_ghostMesh->Get<hd::graphics::Transform>();
			newGhostItem->Edit<hd::graphics::Transform>()->SetOrigin(xform->GetOrigin());
			newGhostItem->Edit<hd::graphics::Transform>()->SetRotation(xform->GetRotation());
			newGhostItem->Edit<hd::graphics::Transform>()->Update();

			GetEntityPool()->Free(m_ghostMesh);
		}

		m_ghostMesh = newGhostItem;
	}
}


void Builder::CreateCollisionPlane() {
    m_collisionVolume = GetEntityPool()->Create();
    m_collisionVolume->SetName(COLLISION_VOLUME_NAME);

    Components::CreateSmallPool<hd::graphics::CollisionVolume>();

    auto transform = m_collisionVolume->Add<hd::graphics::Transform>();
    auto polygon = m_collisionVolume->Add<hd::graphics::Polygon>();
    auto shader = m_collisionVolume->Add<hd::graphics::Shader>();
    auto buffer = m_collisionVolume->Add<hd::graphics::VertexBuffer>();
    auto material = m_collisionVolume->Add<hd::graphics::Material>();
    auto responderState = m_collisionVolume->Add<hd::input::pointer::ResponderState>();
    auto collisionVolume = m_collisionVolume->Add<hd::graphics::CollisionVolume>();

    const float ELEVATION = -400.0f;
    AABB collisionAABB(Vec3(-COLLISION_PLANE_SIZE, -GAP_SIZE + ELEVATION, -COLLISION_PLANE_SIZE),
                       Vec3( COLLISION_PLANE_SIZE, ELEVATION,  COLLISION_PLANE_SIZE));

    transform->SetOrigin(Vec3(0, ELEVATION, 0));
    polygon->CreateCube(collisionAABB.Width(), collisionAABB.Height(), collisionAABB.Depth(), Vec3());

    shader->LoadShader("blinnPhongGeometryInstanced.vert", "blinnPhongGeometry.frag");

    auto meshData = hd::graphics::MeshLoader::Instance()->GetRepeatingGrid(collisionAABB, GRID_NODE_SIZE, GRID_GAP_SIZE);
    hd::Vector<hd::graphics::Mesh *> *meshes = meshData->GetMeshes();
    for (int i = 0; i < meshes->GetSize(); ++i) {
        hd::graphics::Mesh *mesh = meshes->At(i);
        mesh->FlipNormals();
        buffer->AddMesh(mesh);
    }
    buffer->SetDefaultAttributeLocations(shader);

    material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
    material->SetDiffuse(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
    material->SetSpecular(Color4f(0.3f, 0.3f, 0.3f, 1.0f));
    material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
    material->SetShininess(0.0f);

    collisionVolume->SetAABB(collisionAABB);
    collisionVolume->SetNodeSize(Vec3(COLLISION_NODE_SIZE, COLLISION_NODE_SIZE, COLLISION_NODE_SIZE));

    auto lookAtResponder = m_collisionVolume->Add<hd::input::LookAtResponder>();
    lookAtResponder->OnLookAtBegin(this, &Builder::GridLookAtBegin);
    lookAtResponder->OnLookAtHover(this, &Builder::GridLookAtHover);
    lookAtResponder->OnLookAtEnd(this, &Builder::GridLookAtEnd);

   // auto selectResponder = m_collisionVolume->Add<hd::input::pointer::SelectResponder>();
    //auto dragResponder = m_collisionVolume->Add<hd::input::pointer::DragResponder>();
    //auto releaseResponder = m_collisionVolume->Add<hd::input::pointer::ReleaseResponder>();

   // selectResponder->OnSelect(this, &Builder::OnGridSelect);
    //dragResponder->OnDrag(this, &Builder::OnGridDrag);
    //releaseResponder->OnReleaseInside(this, &Builder::OnGridReleaseInside);
  //  releaseResponder->OnRelease(this, &Builder::OnGridRelease);
}


void Builder::PaintOnCollisionPlane(hd::Entity *element, const Collision3 &selectCollision, const Collision3 &releaseCollision) {
    auto collisionVolume = element->Edit<hd::graphics::CollisionVolume>();
    hdAssert(collisionVolume);

    const uint32_t INDEX_SIZE = 6000;
    Size2 indexes[INDEX_SIZE];
    Vec3 centers[INDEX_SIZE];

    const float DO_SCALE_EXPERIMENT = true;

    if (DO_SCALE_EXPERIMENT) {
        uint32_t count = collisionVolume->IndexesOf(selectCollision, releaseCollision, indexes, centers, INDEX_SIZE);
        if (count > 0) {
            hd::log::i("Drag count: %d\n", count);
            Vec3 first = centers[0];
            Vec3 last = centers[count - 1];

            AABB aabb(min(first, last), max(first, last));

            Size2 size(max(1, (int)(1 + hd::abs(indexes[count - 1][0] - indexes[0][0]))),
                       max(1, (int)(1 + hd::abs(indexes[count - 1][1] - indexes[0][1]))));

            Entity *newItem = Create(aabb.Center() + Vec3(0.0f, 0.55f * COLLISION_NODE_SIZE, 0.0f));
            auto transform = newItem->Edit<hd::graphics::Transform>();
            transform->SetScale(Vec3(size[0], 1.0f, size[1]));
        }

    } else {
        uint32_t count = collisionVolume->IndexesOf(selectCollision, releaseCollision, indexes, centers, INDEX_SIZE);
        for (int i = 0; i < count; ++i) {
            Size2 index = indexes[i];
            Vec3 center = centers[i];
            Entity *newItem = Create(center + Vec3(0.0f, 0.55f * COLLISION_NODE_SIZE, 0.0f));
            if (newItem) {
                auto transform = newItem->Edit<hd::graphics::Transform>();
                auto timer = newItem->Add<hd::animation::AnimationTimer>();
                //auto scaleBlock = newItem->Add<hd::animation::ScaleBlock>();

                if (transform) {
                    //transform->SetScale(0.0f);
                }

                //scaleBlock->SetStartScale(Vec3(0.0f));
                //scaleBlock->SetEndScale(Vec3(1.0f));

                timer->SetDuration(hd::randomf(0.05f, 0.5f));
                timer->Start();
            }
        }
    }
}


void Builder::BeginPaintSession() {
    m_paintSessionStarted = true;
    hd::log::i("Begin paint session\n");
    m_currentPaintSession.Clear();
}


void Builder::EndPaintSession() {
    for (int i = 0; i < m_currentPaintSession.Size(); ++i) {
        auto item = m_currentPaintSession.At(i);
       // item->Add<hd::animation::AnimationTimer>();
       //- item->Add<hd::animation::ScaleBlock>();
    }
    m_currentPaintSession.Clear();
   // hd::log::i("Painted %i items.\n", m_undoStack.Size());
    //for (int i = 0; i < m_undoStack.Size(); ++i) {
//
//    }
    m_paintSessionStarted = false;
}


Vec3 Builder::GetPointerWorldPosition() const {
    return m_pointerCollision.origin;
}
