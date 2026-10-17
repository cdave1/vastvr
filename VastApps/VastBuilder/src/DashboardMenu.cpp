#include "DashboardMenu.h"
#include "Builder.h"
#include <array>

class MenuAnimation {
public:
    static Vec3 startPosition;
    static Vec3 endPosition;
    static Quaternion rotation;
};

class MainMenu {
public:
    static hd::Entity *dashboardPanel;
    static hd::Entity *hueSelectionPanel;
    static hd::Entity *colorSelectionPanel;
    static hd::Entity *colorBrightnessPanel;
};

class MeshSelectionMenu {
public:
    static hd::Entity *panel;
};

class GridOptionsMenu {
public:
    static hd::Entity *panel;
};

class TextureMenu {
public:
    static hd::Entity *panel;

    static hd::Array<hd::Entity *> *menuRotater;

    // Menu delegate functions, where Index is a size2(x, y).
    //
    // hd::Entity entityForItemAtIndex(Size2)   // cached entity at the given index.
    // uint32_t numberOfSections
    // uint32_t numberOfColumnsInSection(SectionIndex)
    // uint32_t numberOfRowsInSection(SectionIndex)
    // Vector<std::string> sectionIndexTitles
    // std::string titleForHeaderInSection(SectionIndex)
    //
    // float heightForItemAtIndex(Size2)
    // float widthForItemAtIndex(Size2)
    // didSelectItemAtIndex(Size2)
    // float
    //
    // Placement delegate function
    // * setTransformForElementAtPosition(Transform, Size2)
    //
};

Vec3 MenuAnimation::startPosition;
Vec3 MenuAnimation::endPosition;
Quaternion MenuAnimation::rotation;

hd::Entity *MainMenu::dashboardPanel = nullptr;
hd::Entity *MainMenu::hueSelectionPanel = nullptr;
hd::Entity *MainMenu::colorSelectionPanel = nullptr;
hd::Entity *MainMenu::colorBrightnessPanel = nullptr;

hd::Entity *GridOptionsMenu::panel = nullptr;
hd::Entity *MeshSelectionMenu::panel = nullptr;
hd::Entity *TextureMenu::panel = nullptr;

static bool m_isMenuVisible = false;

using namespace hd::input;
using namespace hd::input::pointer;

DashboardMenu::DashboardMenu(Builder *builder, hd::EntityPool *entityPool) :
m_target(nullptr),
m_builder(builder) {

    CreateMainMenu(entityPool);
    CreateMeshSelectionMenu(entityPool);
    CreateGridOptionsMenu(entityPool);
    CreateTextureMenu(entityPool);

    { // Target
        m_target = entityPool->Create();
        m_target->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = m_target->Add<hd::graphics::Transform>();
        auto shader = m_target->Add<hd::graphics::Shader>();
        auto mesh = m_target->Add<hd::graphics::MeshReference>();
        auto material = m_target->Add<hd::graphics::Material>();
        auto opacity = m_target->Add<hd::graphics::OpacityData>();
        auto collisionData = m_target->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        //auto parent = m_target->Add<hd::data::Parent>();

        transform->SetOrigin(Vec3(-5000.0f, -5000.0f, -5000.0f));
        transform->SetRotation(Quaternion(Vec3(1, 0, 0), hd_half_pi));

        AABB cubeSize(5.0f, 5.0f, Vec3());

        shader->LoadShader("decal.vert", "decal.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetQuad(cubeSize.Size());
        mesh->SetMeshReference(meshData);
        meshData->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        hd::Texture *texture = hd::TextureManager::Instance()->FindTexture("resources/images/target.png");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
        material->SetSpecular(Color4f(0.9f, 0.9f, 0.9f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(0.6f);
        material->SetTexture(texture);

        opacity->SetOpacity(1.0f);
    }
}


void DashboardMenu::CreateMainMenu(hd::EntityPool *entityPool) {
    const uint32_t rows = 4;
    const uint32_t cols = 7;
    const float sphereRadius = 150.0f;
    const float arcWidth = 0.45f * hd_pi;

    m_sphereLayout = new hd::ui::SphereLayout(rows, cols, sphereRadius, arcWidth);

    MainMenu::dashboardPanel = entityPool->Create();
    MainMenu::dashboardPanel->SetName(DASHBOARD_PANEL_NAME);
    MainMenu::dashboardPanel->Add<hd::graphics::OpacityData>();
    MainMenu::dashboardPanel->Add<hd::graphics::Transform>();
    MainMenu::dashboardPanel->Add<hd::animation::AnimationTimer>();
    MainMenu::dashboardPanel->Add<hd::animation::PositionBlock>();
    MainMenu::dashboardPanel->Add<hd::animation::OpacityBlock>();
    MainMenu::dashboardPanel->Add<hd::animation::ScaleBlock>();

    Vec2 size = 0.9f * m_sphereLayout->GetGridNodeSize();
    auto undo = InterfaceUtils::AddMenuItem(entityPool, "Undo", size);
    auto paint = InterfaceUtils::AddMenuItem(entityPool, "Paint", size);
    auto erase = InterfaceUtils::AddMenuItem(entityPool, "Erase", size);
    auto save = InterfaceUtils::AddMenuItem(entityPool, "Save", size);
    auto newWorld = InterfaceUtils::AddMenuItem(entityPool, "New", size);
    auto grid = InterfaceUtils::AddMenuItem(entityPool, "Grid", size);
    auto meshes = InterfaceUtils::AddMenuItem(entityPool, "Brushes", size);
    //auto textures = InterfaceUtils::AddMenuItem(entityPool, "Textures", size);
    auto dismiss = InterfaceUtils::AddMenuItem(entityPool, "x", 0.5f * size);

    auto bg = InterfaceUtils::AddBackground(entityPool, m_sphereLayout->GetSphereRadius(), size);
    m_sphereLayout->PositionOnSphere(2, 0, bg, Vec2(-1, 0));

/*
    auto rotate = InterfaceUtils::AddMenuItem(entityPool, "Rotate");
    auto move = InterfaceUtils::AddMenuItem(entityPool, "Move");
    auto scale = InterfaceUtils::AddMenuItem(entityPool, "Scale");
*/
    auto hue = InterfaceUtils::AddHuePicker(entityPool, size);
    auto color = InterfaceUtils::AddColorPicker(entityPool, size, &MainMenu::colorBrightnessPanel);
    auto colorPanel = InterfaceUtils::AddColorPanel(entityPool, &m_currentColorPanel);

    save->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSave);
    newWorld->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnNewWorld);
    undo->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnUndo);

    paint->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectPaint);
    erase->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectErase);
    grid->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        ShowPanel(GridOptionsMenu::panel);
        HidePanel(MainMenu::dashboardPanel);
    });

    meshes->Edit<ReleaseResponder>()->OnRelease([this](const hd::Entity *entity, const Collision3 &collision) {
        ShowPanel(MeshSelectionMenu::panel);
        HidePanel(MainMenu::dashboardPanel);
    });
/*
    textures->Edit<ReleaseResponder>()->OnRelease([this](const hd::Entity *entity, const Collision3 &collision) {
        ShowPanel(TextureMenu::panel);
        HidePanel(MainMenu::dashboardPanel);
    });*/

    dismiss->Edit<ReleaseResponder>()->OnRelease([this](const hd::Entity *entity, const Collision3 &collision) {
        HidePanel(MeshSelectionMenu::panel);
        HidePanel(MainMenu::dashboardPanel);
        m_isMenuVisible = false;
    });

    hue->Edit<SelectResponder>()->OnSelect(this, &DashboardMenu::OnHueSelect);
    hue->Edit<DragResponder>()->OnDrag(this, &DashboardMenu::OnHueDrag);
    hue->Edit<ReleaseResponder>()->OnRelease(this, &DashboardMenu::OnHueRelease);
    hue->Edit<LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);

    color->Edit<SelectResponder>()->OnSelect(this, &DashboardMenu::OnColorSelect);
    color->Edit<DragResponder>()->OnDrag(this, &DashboardMenu::OnColorDrag);
    color->Edit<ReleaseResponder>()->OnRelease(this, &DashboardMenu::OnColorRelease);
    color->Edit<LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);

    m_sphereLayout->PositionOnSphere(1, 5, color, Vec2(-0.5f, 0.5f));
    m_sphereLayout->PositionOnSphere(2, 6, hue);
    m_sphereLayout->PositionOnSphere(3, 4, colorPanel);

    m_sphereLayout->PositionOnSphere(1, 0, paint);
    m_sphereLayout->PositionOnSphere(1, 1, undo);
    m_sphereLayout->PositionOnSphere(3, 0, save);
    m_sphereLayout->PositionOnSphere(2, 1, newWorld);

    m_sphereLayout->PositionOnSphere(2, 0, erase);

    m_sphereLayout->PositionOnSphere(1, 3, meshes);
    m_sphereLayout->PositionOnSphere(2, 3, grid);
   // m_sphereLayout->PositionOnSphere(3, 3, textures);

    m_sphereLayout->PositionOnSphere(3, 6, dismiss, Vec2(0.0f, 0.5f));

    MainMenu::hueSelectionPanel = hue;
    MainMenu::colorSelectionPanel = color;

    // Set parents
    std::vector<hd::Entity *> items = {undo, paint, erase, save, newWorld, grid, meshes, /* textures,*/ hue, color, colorPanel, dismiss, bg};
    for (auto item : items) {
        if (item->Get<hd::input::pointer::MoveResponder>()) {
            item->Edit<hd::input::pointer::MoveResponder>()->OnMove(this, &DashboardMenu::OnPointerMove);
        }

        item->Edit<hd::data::Parent>()->SetEntity(MainMenu::dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(MainMenu::dashboardPanel->GetID());
    }

    items = {undo, paint, erase, save, newWorld, grid, meshes, /* textures, */ dismiss};
    for (auto item : items) {
        item->Edit<hd::input::pointer::SelectResponder>()->OnSelect(this, &DashboardMenu::OnPointerSelect);
        item->Edit<hd::input::pointer::ReleaseResponder>()->OnRelease(this, &DashboardMenu::OnPointerRelease);

        if (item->Get<hd::input::LookAtResponder>()) {
            item->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &DashboardMenu::OnLookAtBegin);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &DashboardMenu::OnLookAtEnd);
        }
    }

    bg->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
}


void DashboardMenu::CreateGridOptionsMenu(hd::EntityPool *entityPool) {
    const uint32_t rows = 4;
    const uint32_t cols = 7;
    const float sphereRadius = 150.0f;
    const float arcWidth = 0.45f * hd_pi;

    m_sphereLayout = new hd::ui::SphereLayout(rows, cols, sphereRadius, arcWidth);

    GridOptionsMenu::panel = entityPool->Create();
    GridOptionsMenu::panel->SetName(DASHBOARD_PANEL_NAME);
    GridOptionsMenu::panel->Add<hd::graphics::Transform>();
    GridOptionsMenu::panel->Add<hd::graphics::OpacityData>();
    GridOptionsMenu::panel->Add<hd::animation::AnimationTimer>();
    GridOptionsMenu::panel->Add<hd::animation::PositionBlock>();
    GridOptionsMenu::panel->Add<hd::animation::OpacityBlock>();
    GridOptionsMenu::panel->Add<hd::animation::ScaleBlock>();

    GridOptionsMenu::panel->Edit<hd::graphics::OpacityData>()->SetOpacity(0.0f);

    Vec2 size = 0.9f * m_sphereLayout->GetGridNodeSize();
    auto cube = InterfaceUtils::AddMenuItem(entityPool, "Cube", size);
    auto sphere = InterfaceUtils::AddMenuItem(entityPool, "Sphere", size);
    auto plane = InterfaceUtils::AddMenuItem(entityPool, "Plane", size);
    auto curve = InterfaceUtils::AddMenuItem(entityPool, "Curved\nQuad", size);

    auto onDragResize = InterfaceUtils::AddMenuItem(entityPool, "Resize", size);
    auto onDragPaint = InterfaceUtils::AddMenuItem(entityPool, "Paint", size);

    auto snap = InterfaceUtils::AddBooleanOptionButton(entityPool, "Snap to grid", Vec2(size[0] * 3, 0.6f * size[1]));

    auto back = InterfaceUtils::AddMenuItem(entityPool, "<", size);

    auto bg = InterfaceUtils::AddBackground(entityPool, m_sphereLayout->GetSphereRadius(), size);
    m_sphereLayout->PositionOnSphere(2, 0, bg, Vec2(-1, 0));

    cube->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetGridSnapOption(GridSnapOption::Cube);
    });
    sphere->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetGridSnapOption(GridSnapOption::Sphere);
    });
    plane->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetGridSnapOption(GridSnapOption::Plane);
    });
    curve->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetGridSnapOption(GridSnapOption::CurvedQuad);
    });

    onDragResize->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetPaintDragOption(OnPaintDragOption::Resize);
    });

    onDragPaint->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        m_builder->SetPaintDragOption(OnPaintDragOption::Paint);
    });

    snap->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        //m_builder->SetPaintDragOption(OnPaintDragOption::Paint);
        auto option = entity->Edit<hd::ui::BooleanOption>();
        if (option->IsOn()) {
            option->Off(nullptr);
        } else {
            option->On(nullptr);
        }
    });


    back->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        HidePanel(GridOptionsMenu::panel);
        ShowPanel(MainMenu::dashboardPanel);
    });

    m_sphereLayout->PositionOnSphere(3, 0, cube);
    m_sphereLayout->PositionOnSphere(3, 1, sphere);
    m_sphereLayout->PositionOnSphere(3, 2, plane);
    m_sphereLayout->PositionOnSphere(3, 3, curve);
    m_sphereLayout->PositionOnSphere(1, 1, snap);
    m_sphereLayout->PositionOnSphere(2, 3, onDragResize);
    m_sphereLayout->PositionOnSphere(2, 4, onDragPaint);
    m_sphereLayout->PositionOnSphere(3, 6, back);

    std::vector<hd::Entity *> items = { cube, sphere, plane, curve, snap, onDragResize, onDragPaint, back, bg };
    for (auto item : items) {
        item->Edit<hd::data::Parent>()->SetEntity(GridOptionsMenu::panel);
        item->Edit<hd::data::Parent>()->SetId(GridOptionsMenu::panel->GetID());
    }

    bg->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);

    items = { cube, sphere, plane, curve, snap, onDragResize, onDragPaint, back };
    for (auto item : items) {
        item->Edit<hd::input::pointer::SelectResponder>()->OnSelect(this, &DashboardMenu::OnPointerSelect);
        item->Edit<hd::input::pointer::ReleaseResponder>()->OnRelease(this, &DashboardMenu::OnPointerRelease);

        if (item->Get<hd::input::LookAtResponder>()) {
            item->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &DashboardMenu::OnLookAtBegin);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &DashboardMenu::OnLookAtEnd);
        }
    }
}


void DashboardMenu::CreateMeshSelectionMenu(hd::EntityPool *entityPool) {
    const uint32_t rows = 4;
    const uint32_t cols = 7;
    const float sphereRadius = 150.0f;
    const float arcWidth = 0.45f * hd_pi;

    m_sphereLayout = new hd::ui::SphereLayout(rows, cols, sphereRadius, arcWidth);

    MeshSelectionMenu::panel = entityPool->Create();
    MeshSelectionMenu::panel->SetName(DASHBOARD_PANEL_NAME);
    MeshSelectionMenu::panel->Add<hd::graphics::Transform>();
    MeshSelectionMenu::panel->Add<hd::graphics::OpacityData>();
    MeshSelectionMenu::panel->Add<hd::animation::AnimationTimer>();
    MeshSelectionMenu::panel->Add<hd::animation::PositionBlock>();
    MeshSelectionMenu::panel->Add<hd::animation::OpacityBlock>();
    MeshSelectionMenu::panel->Add<hd::animation::ScaleBlock>();

    MeshSelectionMenu::panel->Edit<hd::graphics::OpacityData>()->SetOpacity(0.0f);

    Vec2 size = 0.9f * m_sphereLayout->GetGridNodeSize();
    auto cube = InterfaceUtils::AddMenuItem(entityPool, "Cube", size);
    auto sphere = InterfaceUtils::AddMenuItem(entityPool, "Sphere", size);
    auto cylinder = InterfaceUtils::AddMenuItem(entityPool, "Cylinder", size);
    auto mesh = InterfaceUtils::AddMenuItem(entityPool, "Mesh", size);
    auto hexagon = InterfaceUtils::AddMenuItem(entityPool, "Hexagon", size);
    auto quad = InterfaceUtils::AddMenuItem(entityPool, "Quad", size);
    auto back = InterfaceUtils::AddMenuItem(entityPool, "<", size);

    auto bg = InterfaceUtils::AddBackground(entityPool, m_sphereLayout->GetSphereRadius(), size);
    m_sphereLayout->PositionOnSphere(2, 0, bg, Vec2(-1, 0));

    cube->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectCube);
    sphere->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectSphere);
    cylinder->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectCylinder);
    mesh->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectMesh);
    hexagon->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectHexagon);
    quad->Edit<ReleaseResponder>()->OnReleaseInside(m_builder, &Builder::OnSelectQuad);

    back->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        HidePanel(MeshSelectionMenu::panel);
        ShowPanel(MainMenu::dashboardPanel);
    });

    m_sphereLayout->PositionOnSphere(2, 0, cube);
    m_sphereLayout->PositionOnSphere(2, 1, sphere);
    m_sphereLayout->PositionOnSphere(2, 2, hexagon);
    m_sphereLayout->PositionOnSphere(2, 3, mesh);
    m_sphereLayout->PositionOnSphere(2, 4, cylinder);
    m_sphereLayout->PositionOnSphere(1, 0, quad);
    m_sphereLayout->PositionOnSphere(1, 4, back);

    std::vector<hd::Entity *> items = { cube, sphere, cylinder, mesh, hexagon, quad, back, bg };
    for (auto item : items) {
        item->Edit<hd::data::Parent>()->SetEntity(MeshSelectionMenu::panel);
        item->Edit<hd::data::Parent>()->SetId(MeshSelectionMenu::panel->GetID());
    }

    bg->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);

    items = { cube, sphere, cylinder, mesh, hexagon, quad, back };
    for (auto item : items) {
        item->Edit<hd::input::pointer::SelectResponder>()->OnSelect(this, &DashboardMenu::OnPointerSelect);
        item->Edit<hd::input::pointer::ReleaseResponder>()->OnRelease(this, &DashboardMenu::OnPointerRelease);

        if (item->Get<hd::input::LookAtResponder>()) {
            item->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &DashboardMenu::OnLookAtBegin);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &DashboardMenu::OnLookAtEnd);
        }
    }
}


void DashboardMenu::CreateTextureMenu(hd::EntityPool *pool) {
    const uint32_t rows = 6;
    const uint32_t cols = 9;
    const float sphereRadius = 150.0f;
    const float arcWidth = 0.55f * hd_pi;

    m_sphereLayout = new hd::ui::SphereLayout(rows, cols, sphereRadius, arcWidth);

    TextureMenu::panel = pool->Create();
    TextureMenu::panel->SetName(DASHBOARD_PANEL_NAME);
    TextureMenu::panel->Add<hd::graphics::Transform>();
    TextureMenu::panel->Add<hd::graphics::OpacityData>();
    TextureMenu::panel->Add<hd::animation::AnimationTimer>();
    TextureMenu::panel->Add<hd::animation::PositionBlock>();
    TextureMenu::panel->Add<hd::animation::OpacityBlock>();
    TextureMenu::panel->Add<hd::animation::ScaleBlock>();

    TextureMenu::panel->Edit<hd::graphics::OpacityData>()->SetOpacity(0.0f);

    Vec2 size = 0.9f * m_sphereLayout->GetGridNodeSize();

    std::vector<hd::Texture *> textures = {
        hd::TextureManager::Instance()->FindTexture("resources/images/maps.png"),
        hd::TextureManager::Instance()->FindTexture("resources/images/music_player.png"),
        hd::TextureManager::Instance()->FindTexture("resources/images/paint.png"),
        hd::TextureManager::Instance()->FindTexture("resources/images/wood.png")
    };

    unsigned textureCount = 0;
    for (auto texture : textures) {
        auto menu = InterfaceUtils::AddTextureMenuItem(pool, texture, ":)", 2.0f * size);
        m_sphereLayout->PositionOnSphere(2, textureCount, menu);
        menu->Edit<hd::data::Parent>()->SetEntity(TextureMenu::panel);
        menu->Edit<hd::data::Parent>()->SetId(TextureMenu::panel->GetID());
        textureCount += 2;

        menu->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &DashboardMenu::OnLookAtBegin);
        menu->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
        menu->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &DashboardMenu::OnLookAtEnd);

        menu->Edit<ReleaseResponder>()->OnReleaseInside([this, texture](const hd::Entity *entity, const Collision3 &collision) {
            m_builder->SetCurrentTexture(texture);
            HidePanel(TextureMenu::panel);
            HidePanel(MeshSelectionMenu::panel);
            HidePanel(MainMenu::dashboardPanel);
            m_isMenuVisible = false;
        });
    }

    auto back = InterfaceUtils::AddMenuItem(pool, "<", 0.5f * size);
    auto bg = InterfaceUtils::AddBackground(pool, m_sphereLayout->GetSphereRadius(), size);
    m_sphereLayout->PositionOnSphere(3, 0, bg, Vec2(-1, 0));

    back->Edit<ReleaseResponder>()->OnReleaseInside([this](const hd::Entity *entity, const Collision3 &collision) {
        HidePanel(TextureMenu::panel);
        ShowPanel(MainMenu::dashboardPanel);
    });

    m_sphereLayout->PositionOnSphere(5, 2, back);

    std::vector<hd::Entity *> items = { back, bg };
    for (auto item : items) {
        item->Edit<hd::data::Parent>()->SetEntity(TextureMenu::panel);
        item->Edit<hd::data::Parent>()->SetId(TextureMenu::panel->GetID());
    }

    bg->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);

    items = { back };
    for (auto item : items) {
        if (item->Get<hd::input::LookAtResponder>()) {
            item->Edit<hd::input::LookAtResponder>()->OnLookAtBegin(this, &DashboardMenu::OnLookAtBegin);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtHover(this, &DashboardMenu::OnLookAtHover);
            item->Edit<hd::input::LookAtResponder>()->OnLookAtEnd(this, &DashboardMenu::OnLookAtEnd);
        }
    }
}

#pragma mark - Pointer functions for interacting with menu geometry.

void DashboardMenu::OnPointerMove(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    //hd::log::i("Move: %d\n", element->GetID());
    //// auto transform = element->Get<hd::graphics::Transform>();
    // transform->SetOrigin(transform->GetOrigin() + delta);

    if (m_isMenuVisible) {
        Vec3 normal = hd::normalize(collision.normal);
        Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(collision.normal), Vec3(0, 1, 0));
        rotation.Invert();

        auto transform = m_target->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);
        transform->SetOrigin(collision.origin + (-1.0f * normal));
    }
}



void DashboardMenu::OnPointerSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Select: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.2f);
}


void DashboardMenu::OnPointerDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    hd::log::i("Drag: %d\n", element->GetID());
    //// auto transform = element->Get<hd::graphics::Transform>();
    // transform->SetOrigin(transform->GetOrigin() + delta);
}


void DashboardMenu::OnPointerRelease(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hd::log::i("Release: %d\n", element->GetID());
        auto transform = element->Edit<hd::graphics::Transform>();
        transform->SetScale(1.0f);

        auto identifier = element->Edit<hd::ui::Identifier>();
        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
    }
}


void DashboardMenu::OnPointerReleaseInside(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hd::log::i("Release Inside: %d\n", element->GetID());
        auto transform = element->Edit<hd::graphics::Transform>();
        transform->SetScale(0.9f);
    }
}


// Map the hit into the picker's local rectangle. This works with both pointer
// and gaze selection and does not depend on an asynchronous framebuffer read.
void DashboardMenu::UpdatePickerColor(hd::Entity *element, const Collision3 &collision, bool hue) {
    auto data = element->Get<hd::graphics::CollisionData>();
    Matrix inverse;
    if (!Matrix::InverseLong(inverse, data->GetTransformMatrix())) return;
    Vec3 local;
    Matrix::Vec3Multiply(local, collision.origin, inverse);
    const AABB bounds = data->GetMeshData()->GetAABB();
    const float u = hd::clamp((local[0] - bounds.lower[0]) / bounds.Width(), 0.0f, 1.0f);
    const float v = hd::clamp((local[1] - bounds.lower[1]) / bounds.Height(), 0.0f, 1.0f);
    if (hue) {
        const float wheel[6][3] = {{1,0,0}, {1,1,0}, {0,1,0}, {0,1,1}, {0,0,1}, {1,0,1}};
        const float position = v * 6.0f;
        const int sector = int(position) % 6;
        const float fraction = position - std::floor(position);
        for (int channel = 0; channel < 3; ++channel)
            m_selectedHue[channel] = wheel[sector][channel] * (1.0f - fraction) +
                wheel[(sector + 1) % 6][channel] * fraction;
    } else {
        const float white = hd::smoothstep(0.05f, 0.95f, u);
        for (int channel = 0; channel < 3; ++channel)
            m_selectedColor[channel] = (m_selectedHue[channel] * (1.0f - white) + white) * v;
    }
}

void DashboardMenu::OnHueSelect(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::hueSelectionPanel);
        UpdatePickerColor(element, collision, true);
        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
        m_currentColorPanel->Edit<hd::graphics::Material>()->SetDiffuse(m_selectedHue);
        MainMenu::colorBrightnessPanel->Edit<hd::graphics::Material>()->SetDiffuse(m_selectedHue);
    }
}


void DashboardMenu::OnHueDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::hueSelectionPanel);
        UpdatePickerColor(element, collision, true);
        m_currentColorPanel->Edit<hd::graphics::Material>()->SetDiffuse(m_selectedHue);
        MainMenu::colorBrightnessPanel->Edit<hd::graphics::Material>()->SetAmbient(m_selectedHue);
    }
}


void DashboardMenu::OnHueRelease(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::hueSelectionPanel);
        UpdatePickerColor(element, collision, true);
        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
        m_builder->SetCurrentHue(m_selectedHue);
    }
}


void DashboardMenu::OnColorSelect(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::colorSelectionPanel);
        UpdatePickerColor(element, collision, false);
        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
        m_currentColorPanel->Edit<hd::graphics::Material>()->SetDiffuse(m_selectedColor);
    }
}


void DashboardMenu::OnColorDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::colorSelectionPanel);
        UpdatePickerColor(element, collision, false);
        m_currentColorPanel->Edit<hd::graphics::Material>()->SetDiffuse(m_selectedColor);
    }
}


void DashboardMenu::OnColorRelease(hd::Entity *element, const Collision3 &collision) {
    if (m_isMenuVisible) {
        hdAssert(element == MainMenu::colorSelectionPanel);
        UpdatePickerColor(element, collision, false);
        std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
        hd::sound::SoundController::PlayEffect(path.c_str());
        m_builder->SetCurrentHue(m_selectedColor);
    }
}


void DashboardMenu::OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    if (m_isMenuVisible) {
        auto timer = element->Edit<hd::animation::AnimationTimer>();
        auto positionBlock = element->Edit<hd::animation::PositionBlock>();
        auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
        auto transform = element->Edit<hd::graphics::Transform>();
        //auto opacityBlock = element->Get<hd::animation::OpacityBlock>();

        if (timer) {
            if (timer->IsActive()) {

                transform->SetScale(1.2f);
            } else {
                positionBlock->Disable();
                //opacityBlock->Disable();
                scaleBlock->SetStartScale(1.0f);
                scaleBlock->SetEndScale(1.2f);

                timer->SetDuration(0.10f);
                timer->Start();

                std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
                hd::sound::SoundController::PlayEffect(path.c_str());
            }
        }
    }
}


static float a = 0.0f;
void DashboardMenu::OnLookAtHover(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    /*
    if (m_isMenuVisible) {
        Vec3 normal = hd::normalize(collision.normal);
        Quaternion rotation = Quaternion::LookAt(Vec3(0), hd::normalize(collision.normal), Vec3(0, 1, 0));
        rotation.Invert();

        auto transform = m_target->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);
        transform->SetOrigin(collision.origin + (-1.0f * normal));

        if (element == MainMenu::hueSelectionPanel) {
            m_selectedHue = headset->GetCollisionColor(collision);
        } else if (element == MainMenu::colorSelectionPanel) {
            m_selectedColor = headset->GetCollisionColor(collision);
        }
    }*/
}


void DashboardMenu::OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    if (m_isMenuVisible) {
        auto timer = element->Edit<hd::animation::AnimationTimer>();
        auto positionBlock = element->Edit<hd::animation::PositionBlock>();
        auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
        auto transform = element->Edit<hd::graphics::Transform>();
        //auto opacityBlock = element->Get<hd::animation::OpacityBlock>();

        //element->Get<hd::graphics::Material>()->SetAmbient(Color4f(13.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f, 1.0f));

        if (timer) {
            if (timer->IsActive()) {
                transform->SetScale(1.0f);
            } else {
                positionBlock->Disable();
                //opacityBlock->Disable();
                scaleBlock->SetStartScale(1.2f);
                scaleBlock->SetEndScale(1.0f);

                timer->SetDuration(0.10f);
                timer->Start();
            }
        }
    }
}


void DashboardMenu::ShowPanel(hd::Entity *item) {
    auto timer = item->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = item->Edit<hd::animation::PositionBlock>();
    auto opacityBlock = item->Edit<hd::animation::OpacityBlock>();

    auto lookAtResponder = item->Edit<hd::input::LookAtResponder>();
    if (lookAtResponder) {
        lookAtResponder->Enable();
    }

    if (timer) {
        if (timer->IsActive()) {
            timer->Stop();
        }

        positionBlock->Enable();

        auto transform = item->Edit<hd::graphics::Transform>();
        transform->SetRotation(MenuAnimation::rotation);

        //opacityBlock->SetStartOpacity(0.0f);
        //opacityBlock->SetEndOpacity(1.0f);

        positionBlock->SetStartPosition(MenuAnimation::startPosition);
        positionBlock->SetEndPosition(MenuAnimation::endPosition);

        timer->SetDuration(0.3f);
        timer->Start();
    }
}


void DashboardMenu::ToggleMenu() {
    auto headset = Cameras::mainCamera->Get<hd::input::Headset>();

    Vec3 rayDirection(0.0f, 0.0f, -1.0f);
    Vec3 up(0.0f, 1.0f, 0.0f);

    float RADIUS = 20.0f;
    Vec3 origin(0, 0, 0), startPosition, endPosition;

    RayIntersectsSphereOrigin(rayDirection, RADIUS, &startPosition);
    Quaternion rotation = headset->GetWorldOrientation() * Quaternion::LookAt(hd::normalize(startPosition), origin, up);

    MenuAnimation::startPosition = headset->GetWorldPosition() + Vec3(0, -5000, 0);
    MenuAnimation::endPosition = headset->GetWorldPosition() - (rotation * startPosition);

    hd::log::i("TOGGLE MENU: %3.3f %3.3f %3.3f -> %3.3f %3.3f %3.3f\n",
               MenuAnimation::startPosition[0],
               MenuAnimation::startPosition[1],
               MenuAnimation::startPosition[2],
               MenuAnimation::endPosition[0],
               MenuAnimation::endPosition[1],
               MenuAnimation::endPosition[2]);

    rotation.Invert();
    MenuAnimation::rotation = rotation;

    if (m_isMenuVisible) {
        HidePanel(MainMenu::dashboardPanel);
        HidePanel(MeshSelectionMenu::panel);
        HidePanel(GridOptionsMenu::panel);
        HidePanel(TextureMenu::panel);
        m_isMenuVisible = false;
    } else {
        ShowPanel(MainMenu::dashboardPanel);
        m_isMenuVisible = true;
    }

    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_004.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}


bool DashboardMenu::IsMenuVisible() const {
    return m_isMenuVisible;
}


void DashboardMenu::HidePanel(hd::Entity *item) {
    auto timer = item->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = item->Edit<hd::animation::PositionBlock>();
    auto opacityBlock = item->Edit<hd::animation::OpacityBlock>();
    auto lookAtResponder = item->Edit<hd::input::LookAtResponder>();

    if (lookAtResponder) {
        lookAtResponder->Disable();
    }
    
    if (timer) {
        if (timer->IsActive()) {
            timer->Stop();
        }
        //opacityBlock->SetStartOpacity(1.0f);
        //opacityBlock->SetEndOpacity(0.0f);
        
        positionBlock->SetStartPosition(MenuAnimation::endPosition);
        positionBlock->SetEndPosition(MenuAnimation::startPosition);
        timer->SetDuration(0.3f);
        timer->Start();
    }
}


void DashboardMenu::HideMenu(hd::EntityPool *entityPool, hd::Entity *camera) {
    HidePanel(MainMenu::dashboardPanel);
    HidePanel(MeshSelectionMenu::panel);
    HidePanel(TextureMenu::panel);
    
    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_008.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}
