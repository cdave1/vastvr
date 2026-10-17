#include "Console.h"
#include <sstream>

static AABB worldSize(0, 0, 1024, 512);

static const uint32_t MAX_LOG_LINES = 16;
static const float FIXED_LINE_HEIGHT = 30.0f;

Console::Console(hd::EntityPool *entityPool) : m_logEntries(MAX_LOG_LINES) {
    m_console = entityPool->Create();
    m_console->SetName("DashboardItem");

    m_console->Add<hd::graphics::Transform>();
    m_console->Add<hd::animation::AnimationTimer>();
    m_console->Add<hd::animation::PositionBlock>();
    m_console->Add<hd::animation::OpacityBlock>();
    m_console->Add<hd::animation::ScaleBlock>();

    {
        m_log = entityPool->Create();
        m_log->SetName("label");

        auto transform = m_log->Add<hd::graphics::Transform>();
        auto shader = m_log->Add<hd::graphics::Shader>();
        auto material = m_log->Add<hd::graphics::Material>();
		auto font = m_log->Add<hd::ui::Font>();
        auto vertexBuffer = m_log->Add<hd::graphics::VertexBuffer>();
		auto text = m_log->Add<hd::ui::Text>();
		auto fontMesh = m_log->Add<hd::ui::FontMesh>();
        auto parent = m_log->Add<hd::data::Parent>();
        auto layout = m_log->Add<hd::ui::FontLayout>();
        layout->SetAlignment(hd::ui::TextAlignment::LEFT);
        layout->SetLineLength(700.0f);

        shader->LoadShader("standardGeometry.vert", "standardGeometry.frag");

        material->SetAmbient(Color4f(1.0f, 1.0f, 1.0f, 1.0f));

        difont::Font *ffont = hd::ui::FontLoader::Instance()->FindTextureFont("resources/fonts/Carlito-Bold.ttf", 32);
        font->SetFont(ffont);
        font->SetFaceSize(32.0f);

        text->SetText("Whatever");

        fontMesh->RenderMesh(font, "Whatever");
        vertexBuffer->AddMeshes(fontMesh->GetMeshData());
        vertexBuffer->SetDefaultAttributeLocations(shader);

        AABB fontSize = font->GetSize(text->GetText());
        Vec3 fontSizeOffset = fontSize.Half(); //Vec3(0.0f, 0.5f * fontSize.Height(), 0.5f * fontSize.Width());

        transform->SetOrigin(Vec3(0.0f, 0.0f, 1.0f) - fontSizeOffset);

        parent->SetId(m_console->GetID());
        parent->SetEntity(m_console);

    }

    auto meshData = hd::graphics::MeshLoader::Instance()->GetBoxWithCurvedCorners(Size3f(450, MAX_LOG_LINES * FIXED_LINE_HEIGHT, 1), 45);
    {
		hd::Entity *background = entityPool->Create();
        background->SetName("DashboardButton");

        auto transform = background->Add<hd::graphics::Transform>();
        auto shader = background->Add<hd::graphics::Shader>();
        auto mesh = background->Add<hd::graphics::MeshReference>();
        auto material = background->Add<hd::graphics::Material>();
        auto opacityData = background->Add<hd::graphics::OpacityData>();
        auto parent = background->Add<hd::data::Parent>();

        transform->SetOrigin(Vec3(0.0f, 0.0f, 1.0f));

        shader->LoadShader("blinnPhongGeometry.vert", "blinnPhongGeometry.frag");

        mesh->SetMeshReference(meshData);
        mesh->GetMeshData()->SetDefaultAttributeLocations(shader);

        opacityData->SetOpacity(1.0f);

        material->SetAmbient(Color4f(13.0f/255.0f, 140.0f/255.0f, 255.0f/255.0f, 1.0f));
        material->SetDiffuse(Color4f(0.4f, 0.4f, 0.5f, 1.0f));
        material->SetSpecular(Color4f(0.6f, 0.6f, 0.6f, 1.0f));
        material->SetEmission(Color4f(0.7f, 0.7f, 0.7f, 1.0f));
        material->SetShininess(0.3f);

        parent->SetId(m_console->GetID());
        parent->SetEntity(m_console);
    }

    auto alarm = m_console->Add<hd::input::Alarm>();
    alarm->SetDuration(0.25f);
    alarm->SetLoops(true);
    alarm->Start();
    alarm->SetCallback([this](const hd::input::Alarm *alarm) {
        this->OnLogAlarm();
    });

    hd::log::SetLogCallback([this](const std::string &log) {
        this->OnLog(log);
    });
}


void Console::OnLog(const std::string &log) {
    if (log.length() == 0) {
        return;
    }

    if (m_logEntries.IsFull()) {
        m_logEntries.Pop();
    }
    m_logEntries.Enqueue(log.substr(0, log.length() - 1));

    std::stringstream logString("");
    for (int i = 0; i < m_logEntries.Size(); ++i) {
        std::string logEntry = m_logEntries.At(i);
        logString << logEntry;
        logString << "\r\n";
    }

    if (m_log) {
        auto text = m_log->Edit<hd::ui::Text>();
        text->SetText(logString.str());
    }
}


void Console::OnLogAlarm() {
    auto text = m_log->Get<hd::ui::Text>();
	auto font = m_log->Get<hd::ui::Font>();
    auto layout = m_log->Get<hd::ui::FontLayout>();
    auto shader = m_log->Get<hd::graphics::Shader>();

	auto fontMesh = m_log->Edit<hd::ui::FontMesh>();
	auto vertexBuffer = m_log->Edit<hd::graphics::VertexBuffer>();

    if (layout) {
        fontMesh->RenderMesh(font, layout, text->GetText());
    } else {
        fontMesh->RenderMesh(font, text->GetText());
    }
    vertexBuffer->SetDynamic();
    vertexBuffer->AddMeshes(fontMesh->GetMeshData());
    vertexBuffer->SetDefaultAttributeLocations(shader);
}


static bool isVisible = false;
void Console::ToggleConsole(hd::Entity *camera) {
    if (isVisible) {
        HideConsole(camera);
        isVisible = false;
    } else {
        ShowConsole(camera);
        isVisible = true;
    }
}


void Console::ShowConsole(hd::Entity *camera) {

    auto headset = camera->Get<hd::input::Headset>();
    auto rayTracer = camera->GetComponent<hd::graphics::RayTracer>();

    Vec3 lookAtPosition(0.5f * headset->GetViewport().Width(), 0.5f * headset->GetViewport().Height(), 0.0f);

    Vec3 output = rayTracer->PointerToWorld(lookAtPosition,
                                            headset->GetViewport(),
                                            headset->GetProjection(),
                                            headset->GetLookAtMatrix());

    Vec3 origin, endPosition(0.0f, 0.0f, 400.0f);

        Quaternion rotation = headset->GetWorldOrientation();
        Quaternion rot = Quaternion::LookAt(hd::normalize(endPosition), origin, Vec3(0.0f, 1.0f, 0.0f));
        rot = rotation * rot;
        rot.Invert();

    endPosition = headset->GetWorldPosition() + (rotation * endPosition);


        auto transform = m_console->Edit<hd::graphics::Transform>();
        transform->SetOrigin(endPosition);
        transform->SetRotation(rot);
        transform->Update();
}


void Console::HideConsole(hd::Entity *camera) {
    auto timer = m_console->Edit<hd::animation::AnimationTimer>();
    auto scaleBlock = m_console->Edit<hd::animation::ScaleBlock>();

    if (timer) {
        if (timer->IsActive()) {
            timer->Stop();
        }

        //scaleBlock->SetStartScale(Vec3(1.0f));
        //scaleBlock->SetEndScale(Vec3(0.0f));

        timer->SetDuration(0.25f);
        timer->Start();
    }
}
