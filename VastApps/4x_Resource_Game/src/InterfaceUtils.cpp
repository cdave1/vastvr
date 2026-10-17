#include "InterfaceUtils.h"
#include "Game.h"
#include <array>

hd::Entity * InterfaceUtils::AddMenuItem(hd::EntityPool *entityPool, const std::string labelText, const Vec2 &size) {
    Entity *menuItem = entityPool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto label = entityPool->Create();
    auto button = entityPool->Create();
    {
        label->SetName(DASHBOARD_LABEL_NAME);

        auto transform = label->Add<hd::graphics::Transform>();
        auto opacity = label->Add<hd::graphics::OpacityData>();
        auto shader = label->Add<hd::graphics::Shader>();
        auto material = label->Add<hd::graphics::Material>();
        auto font = label->Add<ui::Font>();
        auto vertexBuffer = label->Add<hd::graphics::VertexBuffer>();
        auto text = label->Add<ui::Text>();
        auto fontMesh = label->Add<ui::FontMesh>();
        auto parent = label->Add<hd::data::Parent>();

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 24);
        font->SetFont(ffont);
        font->SetFaceSize(7.0f);

        text->SetText(labelText);

        fontMesh->RenderMesh(font, labelText);
        vertexBuffer->AddMeshes(fontMesh->GetMeshData());
        vertexBuffer->SetDefaultAttributeLocations(shader);

        AABB fontSize = font->GetSize(text->GetText());
        Vec3 fontSizeOffset = fontSize.Half(); //Vec3(0.0f, 0.5f * fontSize.Height(), 0.5f * fontSize.Width());

        transform->SetOrigin(Vec3(0.0f, 0.0f, 2.0f) - fontSizeOffset);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(size[0], size[1], 2.0f), 2.0f);
    {
        button->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = button->Add<hd::graphics::Transform>();
        auto shader = button->Add<hd::graphics::Shader>();
        auto mesh = button->Add<hd::graphics::MeshReference>();
        auto material = button->Add<hd::graphics::Material>();
        auto opacityData = button->Add<hd::graphics::OpacityData>();
        auto parent = button->Add<hd::data::Parent>();
        auto collisionData = button->Add<hd::graphics::CollisionData>();

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);
        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(51.0f/255.0f, 51.0f/255.0f, 51.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(0.3f);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto moveResponder = menuItem->Add<hd::input::pointer::MoveResponder>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto collisionData = menuItem->Add<hd::graphics::CollisionData>();
    auto identifier = menuItem->Add<ui::Identifier>();
    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();

    collisionData->SetMeshData(meshData);
    collisionData->OnGetTransform([button]() {
        return button->Get<hd::graphics::Transform>()->GetMatrix();
    });

    return menuItem;
}


hd::Entity * InterfaceUtils::AddTextureMenuItem(hd::EntityPool *pool, hd::Texture *texture, const std::string labelText, const Vec2 &size) {
    Entity *menuItem = pool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto label = pool->Create();
    auto textureDecal = pool->Create();
    auto button = pool->Create();

    {

        label->SetName(DASHBOARD_LABEL_NAME);

        auto transform = label->Add<hd::graphics::Transform>();
        auto opacity = label->Add<hd::graphics::OpacityData>();
        auto shader = label->Add<hd::graphics::Shader>();
        auto material = label->Add<hd::graphics::Material>();
        auto font = label->Add<ui::Font>();
        auto vertexBuffer = label->Add<hd::graphics::VertexBuffer>();
        auto text = label->Add<ui::Text>();
        auto fontMesh = label->Add<ui::FontMesh>();
        auto parent = label->Add<hd::data::Parent>();

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 24);
        font->SetFont(ffont);
        font->SetFaceSize(7.0f);

        text->SetText(labelText);

        fontMesh->RenderMesh(font, labelText);
        vertexBuffer->AddMeshes(fontMesh->GetMeshData());
        vertexBuffer->SetDefaultAttributeLocations(shader);

        AABB fontSize = font->GetSize(text->GetText());
        Vec3 fontSizeOffset = fontSize.Half(); //Vec3(0.0f, 0.5f * fontSize.Height(), 0.5f * fontSize.Width());

        transform->SetOrigin(Vec3(0.0f, 0.0f, 2.0f) - fontSizeOffset);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    {
        textureDecal->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = textureDecal->Add<hd::graphics::Transform>();
        auto shader = textureDecal->Add<hd::graphics::Shader>();
        auto mesh = textureDecal->Add<hd::graphics::MeshReference>();
        auto material = textureDecal->Add<hd::graphics::Material>();
        auto opacity = textureDecal->Add<hd::graphics::OpacityData>();
        auto parent = textureDecal->Add<hd::data::Parent>();
        auto collisionData = textureDecal->Add<hd::graphics::CollisionData>();

        transform->SetRotation(Quaternion(Vec3(0, 0, 1), hd_pi));

        AABB cubeSize(0.8f * size[0], 0.8f * size[1], 2.0f);

        shader->LoadShader("decal.vert", "decal.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(cubeSize.Size());
        mesh->SetMeshReference(meshData);
        meshData->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);
        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        material->SetTexture(texture);
        
        opacity->SetOpacity(1.0f);
        transform->SetOrigin(Vec3(0.0f, 0.0f, 1.8f));

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(size[0], size[1], 2.0f), 2.0f);
    {

        button->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = button->Add<hd::graphics::Transform>();
        auto shader = button->Add<hd::graphics::Shader>();
        auto mesh = button->Add<hd::graphics::MeshReference>();
        auto material = button->Add<hd::graphics::Material>();
        auto opacityData = button->Add<hd::graphics::OpacityData>();
        auto parent = button->Add<hd::data::Parent>();
        auto collisionData = button->Add<hd::graphics::CollisionData>();

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);

        collisionData->SetMeshData(meshData);
        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(51.0f/255.0f, 51.0f/255.0f, 51.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(0.3f);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto collisionData = menuItem->Add<hd::graphics::CollisionData>();
    auto identifier = menuItem->Add<ui::Identifier>();
    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();
    collisionData->SetMeshData(meshData);

    collisionData->OnGetTransform([button]() {
        return button->Get<hd::graphics::Transform>()->GetMatrix();
    });
    
    return menuItem;
}


hd::Entity * InterfaceUtils::AddBooleanOptionButton(hd::EntityPool *pool, const std::string labelText, const Vec2 &size) {
    Entity *menuItem = pool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto booleanOption = menuItem->Add<hd::ui::BooleanOption>();

    auto label = pool->Create();
    auto button = pool->Create();

    {
        label->SetName(DASHBOARD_LABEL_NAME);

        auto transform = label->Add<hd::graphics::Transform>();
        auto opacity = label->Add<hd::graphics::OpacityData>();
        auto shader = label->Add<hd::graphics::Shader>();
        auto material = label->Add<hd::graphics::Material>();
        auto font = label->Add<ui::Font>();
        auto vertexBuffer = label->Add<hd::graphics::VertexBuffer>();
        auto text = label->Add<ui::Text>();
        auto fontMesh = label->Add<ui::FontMesh>();
        auto parent = label->Add<hd::data::Parent>();

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 24);
        font->SetFont(ffont);
        font->SetFaceSize(7.0f);

        text->SetText(labelText);

        fontMesh->RenderMesh(font, labelText);
        vertexBuffer->AddMeshes(fontMesh->GetMeshData());
        vertexBuffer->SetDefaultAttributeLocations(shader);

        AABB fontSize = font->GetSize(text->GetText());
        Vec3 fontSizeOffset = fontSize.Half(); //Vec3(0.0f, 0.5f * fontSize.Height(), 0.5f * fontSize.Width());

        transform->SetOrigin(Vec3(0.0f, 0.0f, 2.0f) - fontSizeOffset);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(size[0], size[1], 2.0f), 2.0f);
    {
        button->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = button->Add<hd::graphics::Transform>();
        auto shader = button->Add<hd::graphics::Shader>();
        auto mesh = button->Add<hd::graphics::MeshReference>();
        auto material = button->Add<hd::graphics::Material>();
        auto opacityData = button->Add<hd::graphics::OpacityData>();
        auto parent = button->Add<hd::data::Parent>();
        auto collisionData = button->Add<hd::graphics::CollisionData>();

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);

        collisionData->SetMeshData(meshData);

        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(51.0f/255.0f, 51.0f/255.0f, 51.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(0.3f);

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    {
        auto textureDecal = pool->Create();
        textureDecal->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = textureDecal->Add<hd::graphics::Transform>();
        auto shader = textureDecal->Add<hd::graphics::Shader>();
        auto mesh = textureDecal->Add<hd::graphics::MeshReference>();
        auto material = textureDecal->Add<hd::graphics::Material>();
        auto opacity = textureDecal->Add<hd::graphics::OpacityData>();
        auto parent = textureDecal->Add<hd::data::Parent>();
        auto collisionData = textureDecal->Add<hd::graphics::CollisionData>();

        transform->SetRotation(Quaternion(Vec3(0, 0, 1), hd_pi));

        AABB cubeSize(0.8f * size[1], 0.8f * size[1], 2.0f);

        shader->LoadShader("decal.vert", "decal.frag");

        auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(cubeSize.Size());
        mesh->SetMeshReference(meshData);
        meshData->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        auto texture = hd::TextureManager::Instance()->FindTexture("resources/images/fa-check.png");
        material->SetTexture(texture);

        booleanOption->OnOptionChanged([opacity](bool state, hd::Entity *sender) {
            if (state) {
                opacity->SetOpacity(1.0f);
            } else {
                opacity->SetOpacity(0.0f);
            }
        });

        opacity->SetOpacity(1.0f);
        transform->SetOrigin(Vec3(-0.4f * size[0], 0.0f, 1.8f));

        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto collisionData = menuItem->Add<hd::graphics::CollisionData>();
    auto identifier = menuItem->Add<ui::Identifier>();
    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();

    collisionData->SetMeshData(meshData);
    collisionData->OnGetTransform([button]() {
        return button->Get<hd::graphics::Transform>()->GetMatrix();
    });
    
    return menuItem;
}



hd::Entity * InterfaceUtils::AddBackground(hd::EntityPool *entityPool, const float radius, const Vec2 &gridSize) {
    Entity *background = entityPool->Create();
    background->SetName(DASHBOARD_MENU_ITEM_NAME);

    background->Add<hd::graphics::Transform>();
    background->Add<hd::graphics::OpacityData>();
    background->Add<hd::animation::AnimationTimer>();
    background->Add<hd::animation::PositionBlock>();
    background->Add<hd::animation::ScaleBlock>();
    background->Add<hd::data::Parent>();

    auto meshData = hd::graphics::MeshLoader::Instance()->GetCurvedQuad(1.8f, radius + 40, gridSize[0] * 6.0f, 24);
    auto backgroundMesh = entityPool->Create();

    {
        backgroundMesh->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = backgroundMesh->Add<hd::graphics::Transform>();
        auto shader = backgroundMesh->Add<hd::graphics::Shader>();
        auto mesh = backgroundMesh->Add<hd::graphics::MeshReference>();
        auto material = backgroundMesh->Add<hd::graphics::Material>();
        auto opacityData = backgroundMesh->Add<hd::graphics::OpacityData>();
        auto parent = backgroundMesh->Add<hd::data::Parent>();
        auto collisionData = backgroundMesh->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        Quaternion qx(Vec3(1, 0, 0), hd_pi * 0.5f);
        Quaternion qy(Vec3(0, -1, 0), hd_pi * 0.5f);
        transform->SetRotation(qx * qy);
        transform->SetOrigin(Vec3(0, 0, radius - 40.0f));

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(102.0f/255.0f, 102.0f/255.0f, 102.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        material->SetSpecular(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(16.0f);

        parent->SetId(background->GetID());
        parent->SetEntity(background);
    }

    auto responder = background->Add<hd::input::pointer::ResponderState>();
    auto collisionData = background->Add<hd::graphics::CollisionData>();
    auto identifier = background->Add<ui::Identifier>();
    auto pointerMoveResponder = background->Add<hd::input::pointer::MoveResponder>();
    auto lookAtResponder = background->Add<hd::input::LookAtResponder>();

    collisionData->SetMeshData(meshData);
    collisionData->OnGetTransform([backgroundMesh]() {
        return backgroundMesh->Get<hd::graphics::Transform>()->GetMatrix();
    });

    return background;
}


hd::Entity * InterfaceUtils::AddColorPicker(hd::EntityPool *pool, const Vec2 &gridSize, hd::Entity **colorBrightnessPanel) {
    Entity *menuItem = pool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::OpacityBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(2.0f * gridSize[0], 2.0f * gridSize[1], 2.0f), 0.5f);

    auto _colorBrightnessPanel = pool->Create();
    {
        _colorBrightnessPanel->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = _colorBrightnessPanel->Add<hd::graphics::Transform>();
        auto shader = _colorBrightnessPanel->Add<hd::graphics::Shader>();
        auto mesh = _colorBrightnessPanel->Add<hd::graphics::MeshReference>();
        auto material = _colorBrightnessPanel->Add<hd::graphics::Material>();
        auto opacityData = _colorBrightnessPanel->Add<hd::graphics::OpacityData>();
        auto parent = _colorBrightnessPanel->Add<hd::data::Parent>();
        auto collisionData = _colorBrightnessPanel->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        *colorBrightnessPanel = _colorBrightnessPanel;

        shader->LoadShader("colorPicker.vert", "colorPicker.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        opacityData->SetOpacity(1.0f);
        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto collisionData = menuItem->Add<hd::graphics::CollisionData>();
    auto identifier = menuItem->Add<ui::Identifier>();
    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();

    collisionData->OnGetTransform([_colorBrightnessPanel]() {
        return _colorBrightnessPanel->Get<hd::graphics::Transform>()->GetMatrix();
    });
    collisionData->SetMeshData(meshData);

    return menuItem;
}


hd::Entity * InterfaceUtils::AddHuePicker(hd::EntityPool *pool, const Vec2 &gridSize) {
    Entity *menuItem = pool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(10.0f, 3.0f * gridSize[1], 2.0f), 0.5f);
    auto button = pool->Create();

    {
        button->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = button->Add<hd::graphics::Transform>();
        auto shader = button->Add<hd::graphics::Shader>();
        auto mesh = button->Add<hd::graphics::MeshReference>();
        auto material = button->Add<hd::graphics::Material>();
        auto opacityData = button->Add<hd::graphics::OpacityData>();
        auto parent = button->Add<hd::data::Parent>();
        auto collisionData = button->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        shader->LoadShader("huePicker.vert", "huePicker.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        opacityData->SetOpacity(1.0f);
        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    auto responder = menuItem->Add<hd::input::pointer::ResponderState>();
    auto selectResponder = menuItem->Add<hd::input::pointer::SelectResponder>();
    auto dragResponder = menuItem->Add<hd::input::pointer::DragResponder>();
    auto releaseResponder = menuItem->Add<hd::input::pointer::ReleaseResponder>();
    auto lookAtResponder = menuItem->Add<hd::input::LookAtResponder>();
    auto collisionData = menuItem->Add<hd::graphics::CollisionData>();
    auto identifier = menuItem->Add<ui::Identifier>();

    collisionData->OnGetTransform([button]() {
        return button->Get<hd::graphics::Transform>()->GetMatrix();
    });
    collisionData->SetMeshData(meshData);

    return menuItem;
}


hd::Entity * InterfaceUtils::AddColorPanel(hd::EntityPool *pool, hd::Entity **colorPickerPanel) {
    Entity *menuItem = pool->Create();
    menuItem->SetName(DASHBOARD_MENU_ITEM_NAME);

    menuItem->Add<hd::graphics::Transform>();
    menuItem->Add<hd::graphics::OpacityData>();
    menuItem->Add<hd::animation::AnimationTimer>();
    menuItem->Add<hd::animation::PositionBlock>();
    menuItem->Add<hd::animation::ScaleBlock>();
    menuItem->Add<hd::data::Parent>();

    auto meshData = hd::graphics::MeshLoader::Instance()->GetCube(Vec3(20.0f, 20.0f, 2.0f));
    {
        Entity *_colorPickerPanel = pool->Create();
        _colorPickerPanel->SetName(DASHBOARD_BUTTON_NAME);

        auto transform = _colorPickerPanel->Add<hd::graphics::Transform>();
        auto shader = _colorPickerPanel->Add<hd::graphics::Shader>();
        auto mesh = _colorPickerPanel->Add<hd::graphics::MeshReference>();
        auto material = _colorPickerPanel->Add<hd::graphics::Material>();
        auto opacityData = _colorPickerPanel->Add<hd::graphics::OpacityData>();
        auto parent = _colorPickerPanel->Add<hd::data::Parent>();
        auto collisionData = _colorPickerPanel->Add<hd::graphics::CollisionData>();

        collisionData->OnGetTransform([transform]() {
            return transform->GetMatrix();
        });

        *colorPickerPanel = _colorPickerPanel;

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);
        collisionData->SetMeshData(meshData);

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        opacityData->SetOpacity(1.0f);
        parent->SetId(menuItem->GetID());
        parent->SetEntity(menuItem);
    }

    return menuItem;
}
