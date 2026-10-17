#include "SphereMenu.h"
#include "Game.h"

static AABB worldSize(0, 0, 1024, 512);

using namespace hd::input::pointer;

SphereMenu::SphereMenu(Builder *builder, hd::EntityPool *entityPool) {
    hd::Entity *dashboardPanel = entityPool->Create();
    dashboardPanel->SetName(SPHERE_PANEL_NAME);

    dashboardPanel->Add<hd::graphics::Transform>();
    dashboardPanel->Add<hd::animation::AnimationTimer>();
    dashboardPanel->Add<hd::animation::PositionBlock>();
    dashboardPanel->Add<hd::animation::ScaleBlock>();

    for (int i = 0; i < 42; ++i) {
        char name[64];
        snprintf(name, sizeof(name), "block %d", i);
        auto item = AddMenuItem(entityPool, name);
        item->Edit<hd::data::Parent>()->SetEntity(dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(dashboardPanel->GetID());
        item->Edit<SelectResponder>()->OnSelect(this, &SphereMenu::OnSelect);
        item->Edit<DragResponder>()->OnDrag(this, &SphereMenu::OnDrag);
        item->Edit<ReleaseResponder>()->OnRelease(this, &SphereMenu::OnRelease);
    }

    const int width = 12;
    const int height = 4;
    float layoutArcWidth = hd_pi * 0.7f;
    float layoutArcHeight = layoutArcWidth * (float(height) / float(width));

        LayoutMenu(entityPool, width, height,
                   Vec2(layoutArcWidth, layoutArcHeight));
        
        HideMenu(entityPool, nullptr);
}


hd::Entity * SphereMenu::AddMenuItem(hd::EntityPool *entityPool, const std::string labelText) {
    hd::Entity *menuItem = entityPool->Create();
    menuItem->SetName(SPHERE_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::OpacityBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    {
        auto label = entityPool->Create();
        label->SetName(SPHERE_LABEL_NAME);

        auto transform = label->Add<hd::graphics::Transform>();
        auto shader = label->Add<hd::graphics::Shader>();
        auto material = label->Add<hd::graphics::Material>();
        auto font = label->Add<hd::ui::Font>();
        auto vertexBuffer = label->Add<hd::graphics::VertexBuffer>();
		auto text = label->Add<hd::ui::Text>();
		auto fontMesh = label->Add<hd::ui::FontMesh>();
        auto parent = label->Add<hd::data::Parent>();

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
        material->SetSpecular(Color4f(0.8f, 0.8f, 0.9f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(1.0f);

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 48);
        font->SetFont(ffont);
        font->SetFaceSize(10.0f);

        text->SetText(labelText);

        fontMesh->RenderMesh(font, labelText);
        vertexBuffer->AddMeshes(fontMesh->GetMeshData());
        vertexBuffer->SetDefaultAttributeLocations(shader);

        //transform->SetOrigin(Vec3() +Vec3(0.0f, 0.0f, 8.0f));

        AABB fontSize = font->GetSize(text->GetText());
        Vec3 fontSizeOffset = fontSize.Half(); //Vec3(0.0f, 0.5f * fontSize.Height(), 0.5f * fontSize.Width());

        transform->SetOrigin(Vec3(0.0f, 0.0f, 8.0f) - fontSizeOffset);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(50, 50, 10), 5.0f);
    {
		hd::Entity *button = entityPool->Create();
        button->SetName(SPHERE_BUTTON_NAME);

        auto transform = button->Add<hd::graphics::Transform>();
        auto shader = button->Add<hd::graphics::Shader>();
        auto buffer = button->Add<hd::graphics::VertexBuffer>();
        auto material = button->Add<hd::graphics::Material>();
        auto opacityData = button->Add<hd::graphics::OpacityData>();
        auto parent = button->Add<hd::data::Parent>();

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        hd::Vector<hd::graphics::Mesh *> *meshes = meshData->GetMeshes();
        for (int i = 0; i < meshes->GetSize(); ++i) {
            hd::graphics::Mesh *mesh = meshes->At(i);
            buffer->AddMesh(mesh);
        }
        buffer->SetDefaultAttributeLocations(shader);
        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(13.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
        material->SetSpecular(Color4f(0.6f, 0.6f, 0.6f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(0.3f);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto collisionMesh = menuItem->Add<hd::graphics::CollisionData>();
	auto identifier = menuItem->Add<hd::ui::Identifier>();
    collisionMesh->SetMeshData(meshData);

    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();
    lookAtResponder->OnLookAtBegin(this, &SphereMenu::OnLookAtBegin);
    lookAtResponder->OnLookAtEnd(this, &SphereMenu::OnLookAtEnd);

    return menuItem;
}


void SphereMenu::LayoutMenu(hd::EntityPool *entityPool, int len, int height, const Vec2 &arcLength) {
	hd::EntityQuery *menuItems = entityPool->QueryByName(SPHERE_MENU_ITEM_NAME);

    Vec3 rayDirection(0.0f, 0.0f, -1.0f);
    Vec3 up(0.0f, 1.0f, 0.0f);

	float horizontalStepRadians = (arcLength[0] / hd::max(1, len));
	float verticalStepRadians = (arcLength[1] / hd::max(1, height));

    hd::Array<Size2> positions(len * height);
    for (int x = 0; x < len; ++x) {
        for (int y = 0; y < height; ++y) {
            positions.Add(Size2(x, y), x * height + y);
        }
    }

    float MIN_RADIUS = 300.0f;
    Vec3 origin, endPosition;

    for (int i = 0; i < positions.Capacity(); ++i) {
        if (i >= menuItems->Size()) {
            break;
        }
        auto menuItem = menuItems->At(i);

        Size2 position = *positions.Item(i);

        Quaternion horizontalStep(Vec3(0, 1, 0), position[0] * horizontalStepRadians - (0.5f * arcLength[0]));
        Quaternion verticalStep(Vec3(1, 0, 0), position[1] * verticalStepRadians - (0.5f * arcLength[1]));

        endPosition = rayDirection * MIN_RADIUS;

        Quaternion rotation = Quaternion::LookAt(hd::normalize(endPosition), origin, Vec3(0, 1, 0));
        rotation = rotation * horizontalStep;
        rotation = rotation * verticalStep;
        endPosition = rotation * endPosition;
        rotation.Invert();

        auto transform = menuItem->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);
        transform->SetOrigin(endPosition);
    }
}


void SphereMenu::OnSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Select: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.2f);
}


void SphereMenu::OnDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    //hd::log::i("Drag: %d\n", element->GetID());
   //// auto transform = element->Get<hd::graphics::Transform>();
   // transform->SetOrigin(transform->GetOrigin() + delta);
}


void SphereMenu::OnRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    //transform->SetScale(1.0f);
 
    //m_musicPlayer.PlayMusic("/Users/davidpetrie/Projects/dmg/VastBrowser/resources/sfx/blop_001.wav");
    
    auto identifier = element->Get<hd::ui::Identifier>();
    //if (m_onAppSelected) {
    //    m_onAppSelected(identifier->GetId());
    //}
    std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
    hd::sound::SoundController::PlayEffect(path.c_str());
}


void SphereMenu::OnReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release Inside: %d\n", element->GetID());
    //auto transform = element->Get<hd::graphics::Transform>();
    //transform->SetScale(0.9f);
}


void SphereMenu::OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = element->Edit<hd::animation::PositionBlock>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();
    auto opacityBlock = element->Edit<hd::animation::OpacityBlock>();

    //element->Get<hd::graphics::Material>()->SetAmbient(Color4f(12.0f/255.0f, 202.0f/255.0f, 255.0f/232.0f, 1.0f));

    if (timer) {
        if (timer->IsActive()) {

            transform->SetScale(1.2f);
                        timer->Stop();
        } else {
            positionBlock->Disable();
            opacityBlock->Disable();
            scaleBlock->SetStartScale(1.0f);
            scaleBlock->SetEndScale(1.2f);

            timer->SetDuration(0.15f);
            timer->Start();

            std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/vast_menu_sesh_B_001_005.wav");
            hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void SphereMenu::OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = element->Edit<hd::animation::PositionBlock>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();
    auto opacityBlock = element->Edit<hd::animation::OpacityBlock>();

    //element->Get<hd::graphics::Material>()->SetAmbient(Color4f(13.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f, 1.0f));

    if (timer) {
        if (timer->IsActive()) {
            transform->SetScale(1.0f);
            timer->Stop();
        } else {
            positionBlock->Disable();
            opacityBlock->Disable();
            scaleBlock->SetStartScale(1.2f);
            scaleBlock->SetEndScale(1.0f);

            timer->SetDuration(0.15f);
            timer->Start();

            //std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/vast_menu_sesh_B_001_009.wav");
            //hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void _ShowMenuItem(hd::Entity *item, const Vec3 &startPosition, const Vec3 &endPosition, const Quaternion &rotation) {
    auto timer = item->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = item->Edit<hd::animation::PositionBlock>();
    auto transform = item->Edit<hd::graphics::Transform>();
    //auto opacityBlock = item->Get<hd::animation::OpacityBlock>();
    
    auto lookAtResponder = item->Edit<hd::input::LookAtResponder>();
    if (lookAtResponder) {
        lookAtResponder->Enable();
    }

    if (timer) {
        if (timer->IsActive()) {
            timer->Stop();
        }

        positionBlock->Enable();
        //opacityBlock->Enable();

        transform->SetRotation(rotation);

        positionBlock->SetStartPosition(startPosition);
        positionBlock->SetEndPosition(endPosition);
       // opacityBlock->SetStartOpacity(0.0f);
      //  opacityBlock->SetEndOpacity(1.0f);

        timer->SetDuration(0.3f);
        timer->Start();
    }
}


void SphereMenu::ShowMenu(hd::EntityPool *entityPool, hd::Entity *camera) {
	hd::EntityQuery *blocks = entityPool->QueryByName(SPHERE_PANEL_NAME);

    auto headset = camera->Get<hd::input::Headset>();

    Quaternion rotation = headset->GetWorldOrientation();

    Vec3 rayDirection(0.0f, 0.0f, 1.0f);
    Vec3 up(0.0f, 1.0f, 0.0f);

    float MAX_RADIUS = 400.0f;
    float MIN_RADIUS = 300.0f;
    Vec3 origin(0, 0, 0), startPos, endPos;

	hd::RayIntersectsSphere(origin - Vec3(0, 5000.0, 0), rayDirection, origin - Vec3(0, 5000.0, 0), MAX_RADIUS, &startPos);
	hd::RayIntersectsSphere(origin, rayDirection, origin, MIN_RADIUS, &endPos);

    Quaternion rot = Quaternion::LookAt(hd::normalize(endPos), origin, up);
    rot = rotation * rot;
    rot.Invert();

    startPos = headset->GetWorldPosition() + (rotation * startPos);
    endPos = headset->GetWorldPosition();

    for (int i = 0; i < blocks->Size(); ++i) {
        _ShowMenuItem(blocks->At(i), startPos, endPos, rot);
    }
}


void SphereMenu::RepositionMenu(hd::EntityPool *entityPool, hd::Entity *camera) {

}


void _HideMenuItem(hd::Entity *item) {
    auto timer = item->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = item->Edit<hd::animation::PositionBlock>();
    auto opacityBlock = item->Edit<hd::animation::OpacityBlock>();
    auto lookAtResponder = item->Edit<hd::input::LookAtResponder>();

    if (lookAtResponder) {
        lookAtResponder->Disable();
    }

    positionBlock->Enable();
    //opacityBlock->Enable();

    if (timer) {
        if (timer->IsActive()) {
            timer->Stop();
        }
        positionBlock->Reverse();
        //opacityBlock->Reverse();
        timer->Start();
    }
}


void SphereMenu::HideMenu(hd::EntityPool *entityPool, hd::Entity *camera) {
	hd::EntityQuery *blocks = entityPool->QueryByName(SPHERE_PANEL_NAME);

    for (int i = 0; i < blocks->Size(); ++i) {
        _HideMenuItem(blocks->At(i));
    }
}
