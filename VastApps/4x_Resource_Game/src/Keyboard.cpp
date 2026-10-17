#include "Keyboard.h"
#include "Game.h"

static AABB worldSize(0, 0, 1024, 512);

using namespace hd::input::pointer;

Keyboard::Keyboard(Builder *builder, hd::EntityPool *entityPool) {
    Entity *dashboardPanel = entityPool->Create();
    dashboardPanel->SetName(KEYBOARD_PANEL_NAME);

    dashboardPanel->Add<hd::graphics::Transform>();
    dashboardPanel->Add<hd::animation::AnimationTimer>();
    dashboardPanel->Add<hd::animation::PositionBlock>();
    dashboardPanel->Add<hd::animation::ScaleBlock>();

    const unsigned NUMBER_COUNT = 10;
    const unsigned QWERTY_COUNT = 10;
    const unsigned ASDF_COUNT = 9;
    const unsigned ZXCV_COUNT = 8;

    const std::string numbers[NUMBER_COUNT] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
    };

    const std::string qwerty[QWERTY_COUNT] = {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"
    };

    const std::string asdf[ASDF_COUNT] = {
        "A", "S", "D", "F", "G", "H", "J", "K", "L"
    };

    const std::string zxcv[ZXCV_COUNT] = {
        "Z", "X", "C", "V", "B", "N", "M", "[ ]"
    };

    for (int i = 0; i < NUMBER_COUNT; ++i) {
        auto item = AddKey(entityPool, numbers[i], float(3), float(i));
        item->Edit<hd::data::Parent>()->SetEntity(dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(dashboardPanel->GetID());
    }

    for (int i = 0; i < QWERTY_COUNT; ++i) {
        auto item = AddKey(entityPool, qwerty[i], float(2), float(i) + 0.33f);
        item->Edit<hd::data::Parent>()->SetEntity(dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(dashboardPanel->GetID());
    }

    for (int i = 0; i < ASDF_COUNT; ++i) {
        auto item = AddKey(entityPool, asdf[i], float(1), float(i) + 0.66f);
        item->Edit<hd::data::Parent>()->SetEntity(dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(dashboardPanel->GetID());
    }

    for (int i = 0; i < ZXCV_COUNT; ++i) {
        auto item = AddKey(entityPool, zxcv[i], float(0), float(i) + 1.0f);
        item->Edit<hd::data::Parent>()->SetEntity(dashboardPanel);
        item->Edit<hd::data::Parent>()->SetId(dashboardPanel->GetID());
    }

     Hide(entityPool, nullptr);
}


hd::Entity * Keyboard::AddKey(hd::EntityPool *entityPool, const std::string labelText, const float row, const float column) {
    Entity *menuItem = entityPool->Create();
    menuItem->SetName(KEYBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::OpacityBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    {
        auto label = entityPool->Create();
        label->SetName(KEYBOARD_LABEL_NAME);

        auto transform = label->Add<hd::graphics::Transform>();
        auto shader = label->Add<hd::graphics::Shader>();
        auto material = label->Add<hd::graphics::Material>();
        auto font = label->Add<ui::Font>();
        auto vertexBuffer = label->Add<hd::graphics::VertexBuffer>();
        auto text = label->Add<ui::Text>();
        auto fontMesh = label->Add<ui::FontMesh>();
        auto parent = label->Add<hd::data::Parent>();

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
        material->SetSpecular(Color4f(0.8f, 0.8f, 0.9f, 1.0f));
        material->SetEmission(Color4f(0.1f, 0.1f, 0.1f, 1.0f));
        material->SetShininess(1.0f);

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 48);
        font->SetFont(ffont);
        font->SetFaceSize(16.0f);

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

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(40, 40, 5), 5.0f);
    {
        Entity *button = entityPool->Create();
        button->SetName(KEYBOARD_BUTTON_NAME);

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

        material->SetAmbient(Color4f(51.0f/255.0f, 51.0f/255.0f, 51.0f/255.0f, 1.0f));
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
    auto identifier = menuItem->Add<ui::Identifier>();
    collisionMesh->SetMeshData(meshData);

    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();
    lookAtResponder->OnLookAtBegin(this, &Keyboard::OnLookAtBegin);
    lookAtResponder->OnLookAtEnd(this, &Keyboard::OnLookAtEnd);

    menuItem->Edit<SelectResponder>()->OnSelect(this, &Keyboard::OnSelect);
    menuItem->Edit<DragResponder>()->OnDrag(this, &Keyboard::OnDrag);
    menuItem->Edit<ReleaseResponder>()->OnRelease(this, &Keyboard::OnRelease);

    {
        int height = 4;
        int width = 10;

        float layoutArcWidth = hd_pi * 0.5f;
        float layoutArcHeight = layoutArcWidth * (float(height) / float(width));

        float horizontalStepRadians = (layoutArcWidth / max(1, width));
        float verticalStepRadians = (layoutArcHeight / max(1, height));

        float MIN_RADIUS = 300.0f;

        Quaternion horizontalStep(Vec3(0, -1, 0), column * horizontalStepRadians - (0.5f * layoutArcWidth));
        Quaternion verticalStep(Vec3(1, 0, 0), row * verticalStepRadians - (0.5f * layoutArcHeight));

        Vec3 position = (Vec3(0, 0, -1) * MIN_RADIUS);

        Quaternion rotation = Quaternion::LookAt(hd::normalize(position), Vec3(), Vec3(0, 1, 0));
        rotation = rotation * horizontalStep;
        rotation = rotation * verticalStep;
        position = rotation * position;
        rotation.Invert();

        auto transform = menuItem->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);
        transform->SetOrigin(position);
    }

    return menuItem;
}


void Keyboard::OnSelect(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Select: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    transform->SetScale(1.2f);
}


void Keyboard::OnDrag(hd::Entity *element, const Collision3 &collision, const Vec3 &delta) {
    //hd::log::i("Drag: %d\n", element->GetID());
   //// auto transform = element->Get<hd::graphics::Transform>();
   // transform->SetOrigin(transform->GetOrigin() + delta);
}


void Keyboard::OnRelease(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release: %d\n", element->GetID());
    auto transform = element->Edit<hd::graphics::Transform>();
    //transform->SetScale(1.0f);
 
    //m_musicPlayer.PlayMusic("/Users/davidpetrie/Projects/dmg/VastBrowser/resources/sfx/blop_001.wav");
    
    auto identifier = element->Edit<hd::ui::Identifier>();
    //if (m_onAppSelected) {
    //    m_onAppSelected(identifier->GetId());
    //}
}


void Keyboard::OnReleaseInside(hd::Entity *element, const Collision3 &collision) {
    hd::log::i("Release Inside: %d\n", element->GetID());
    //auto transform = element->Get<hd::graphics::Transform>();
    //transform->SetScale(0.9f);
}


void Keyboard::OnLookAtBegin(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = element->Edit<hd::animation::PositionBlock>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();
    auto opacityBlock = element->Edit<hd::animation::OpacityBlock>();

    //element->Get<hd::graphics::Material>()->SetAmbient(Color4f(12.0f/255.0f, 202.0f/255.0f, 255.0f/232.0f, 1.0f));
    Quaternion rot = transform->GetRotation();
    rot.Invert();

    if (timer) {
        if (timer->IsActive()) {

            transform->SetOrigin(transform->GetOrigin() - (rot * Vec3(0, 0, -40)));

            transform->SetScale(1.0f);
                        timer->Stop();
        } else {
            positionBlock->Enable();
            opacityBlock->Disable();
            scaleBlock->SetStartScale(1.0f);
            scaleBlock->SetEndScale(1.0f);

            positionBlock->SetStartPosition(transform->GetOrigin());

            positionBlock->SetEndPosition(transform->GetOrigin() - (rot * Vec3(0, 0, -40)));


            timer->SetDuration(0.15f);
            timer->Start();

            std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav");
            hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void Keyboard::OnLookAtEnd(hd::Entity *element, hd::input::Headset *headset, const Collision3 &collision) {
    auto timer = element->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = element->Edit<hd::animation::PositionBlock>();
    auto scaleBlock = element->Edit<hd::animation::ScaleBlock>();
    auto transform = element->Edit<hd::graphics::Transform>();
    auto opacityBlock = element->Edit<hd::animation::OpacityBlock>();

    //element->Get<hd::graphics::Material>()->SetAmbient(Color4f(13.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f, 1.0f));

    if (timer) {
        if (timer->IsActive()) {
            transform->SetScale(1.0f);
            transform->SetOrigin(positionBlock->GetStartPosition());
            timer->Stop();
        } else {
            //positionBlock->Disable();
            positionBlock->Enable();
            positionBlock->Reverse();
            opacityBlock->Disable();
            scaleBlock->SetStartScale(1.0f);
            scaleBlock->SetEndScale(1.0f);

            timer->SetDuration(0.15f);
            timer->Start();

            //std::string path = hd::FileSystem::FullPathTo("resources/sfx/converted/vast_menu_sesh_B_001_009.wav");
            //hd::sound::SoundController::PlayEffect(path.c_str());
        }
    }
}


void _ShowKey(hd::Entity *item, const Vec3 &startPosition, const Vec3 &endPosition, const Quaternion &rotation) {
    auto timer = item->Edit<hd::animation::AnimationTimer>();
    auto positionBlock = item->Edit<hd::animation::PositionBlock>();
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

        auto transform = item->Edit<hd::graphics::Transform>();
        transform->SetRotation(rotation);

        positionBlock->SetStartPosition(startPosition);
        positionBlock->SetEndPosition(endPosition);
       // opacityBlock->SetStartOpacity(0.0f);
      //  opacityBlock->SetEndOpacity(1.0f);

        timer->SetDuration(0.3f);
        timer->Start();
    }
}


void Keyboard::Show(hd::EntityPool *entityPool, hd::Entity *camera) {
    EntityQuery *blocks = entityPool->QueryByName(KEYBOARD_PANEL_NAME);

    auto headset = camera->Get<hd::input::Headset>();

    Quaternion rotation = headset->GetWorldOrientation();

    Vec3 rayDirection(0.0f, 0.0f, 1.0f);
    Vec3 up(0.0f, 1.0f, 0.0f);

    float MAX_RADIUS = 400.0f;
    float MIN_RADIUS = 300.0f;
    Vec3 origin(0, 0, 0), startPos, endPos;

    RayIntersectsSphere(origin - Vec3(0, 5000.0, 0), rayDirection, origin - Vec3(0, 5000.0, 0), MAX_RADIUS, &startPos);
    RayIntersectsSphere(origin, rayDirection, origin, MIN_RADIUS, &endPos);

    Quaternion rot = Quaternion::LookAt(hd::normalize(endPos), origin, up);
    rot = rotation * rot;
    rot.Invert();

    startPos = headset->GetWorldPosition() + (rotation * startPos);
    endPos = headset->GetWorldPosition();

    for (int i = 0; i < blocks->Size(); ++i) {
        _ShowKey(blocks->At(i), startPos, endPos, rot);
    }
}


void _HideKey(hd::Entity *item) {
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


void Keyboard::Hide(hd::EntityPool *entityPool, hd::Entity *camera) {
    EntityQuery *blocks = entityPool->QueryByName(KEYBOARD_PANEL_NAME);

    for (int i = 0; i < blocks->Size(); ++i) {
        _HideKey(blocks->At(i));
    }
}
