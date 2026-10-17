// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "PhysicsSandbox.h"
#include "entitysystems/Cameras.h"
#include <algorithm>
#include <cmath>

using namespace hd;
using namespace hd::graphics;
using namespace hd::input;
namespace {
const float units = 100.0f;
const size_t maxShapes = 64;
const double step = 1.0 / 60.0;
void color(Entity *entity, const Color4f &value) {
    auto material = entity->Edit<Material>();
    material->SetAmbient(value);
    material->SetDiffuse(value);
    material->SetEmission(Color4f(value[0] * 0.25f, value[1] * 0.25f, value[2] * 0.25f, 1.0f));
}
}

PhysicsSandbox::PhysicsSandbox(hd::application::Client *client) : AppPrototype(client),
    m_world(new b2World(b2Vec2(0, -10))), m_anchor(nullptr), m_drag(nullptr),
    m_status(nullptr), m_paused(false), m_gravity(true), m_steps(0), m_accumulator(0),
    m_lastUpdate(std::chrono::steady_clock::now()) {
    m_name = "PhysicsHotel";
    b2BodyDef anchor; m_anchor = m_world->CreateBody(&anchor);
    color(AddCube(5000, 3000, 40, Vec3(0, 280, -120)), Color4f(0.055f, 0.085f, 0.13f, 1.0f));
    AddWall(1104, 40, Vec3(0, -20, 0));
    AddWall(40, 512, Vec3(-532, 256, 0));
    AddWall(40, 512, Vec3(532, 256, 0));
    AddWall(1104, 40, Vec3(0, 532, 0));
    AddLabel(Vec3(0, 655, 50), Vec3(), "PhysicsHotel");
    AddLabel(Vec3(0, 610, 50), Vec3(), "A playground for gravity, collisions and dragging");
    AddLabel(Vec3(0, -82, 50), Vec3(), "Drag shapes with the mouse   |   Space: add   Backspace: remove");
    AddLabel(Vec3(0, -126, 50), Vec3(), "P: pause   G: gravity   R: reset");
    m_status = AddLabel(Vec3(0, 575, 50), Vec3(), "");
    Reset();
}

PhysicsSandbox::~PhysicsSandbox() { EndDrag(); }

void PhysicsSandbox::AddWall(float width, float height, Vec3 center) {
    b2BodyDef definition; definition.position.Set(center[0] / units, center[1] / units);
    auto body = m_world->CreateBody(&definition);
    b2PolygonShape box; box.SetAsBox(width / (2 * units), height / (2 * units));
    body->CreateFixture(&box, 0);
    color(AddCube(width, height, 80, center), Color4f(0.17f, 0.29f, 0.39f, 1.0f));
}

Entity *PhysicsSandbox::CreateVisual(size_t index) {
    auto visual = AddCube(46, 46, 46, Vec3());
    if (index % 2 == 0) {
        auto mesh = MeshLoader::Instance()->GetSphere(26, 16);
        auto buffer = visual->Edit<VertexBuffer>();
        buffer->Reset(); buffer->AddMesh(mesh->GetMeshes()->At(0));
        buffer->SetDefaultAttributeLocations(visual->Edit<Shader>());
        visual->Edit<MeshReference>()->SetMeshReference(mesh);
        visual->Edit<CollisionData>()->SetMeshData(mesh);
    }
    const Color4f palette[] = {Color4f(0.95f, 0.43f, 0.23f, 1.0f), Color4f(0.17f, 0.72f, 0.77f, 1.0f),
        Color4f(0.98f, 0.73f, 0.25f, 1.0f), Color4f(0.62f, 0.47f, 0.90f, 1.0f)};
    color(visual, palette[index % 4]);
    return visual;
}

void PhysicsSandbox::Spawn() {
    if (m_bodies.size() == maxShapes) return;
    const size_t index = m_bodies.size();
    if (index == m_visuals.size()) m_visuals.push_back(CreateVisual(index));
    b2BodyDef definition; definition.type = b2_dynamicBody;
    definition.position.Set((-350.0f + (index % 8) * 100.0f) / units,
                            (330.0f + ((index / 8) % 2) * 100.0f) / units);
    definition.angle = index % 2 ? 0.12f * float(index % 5) : 0;
    auto body = m_world->CreateBody(&definition);
    b2FixtureDef fixture; fixture.density = 0.8f; fixture.friction = 0.4f; fixture.restitution = 0.55f;
    b2CircleShape circle; circle.m_radius = 26 / units;
    b2PolygonShape box; box.SetAsBox(23 / units, 23 / units);
    fixture.shape = index % 2 ? static_cast<b2Shape *>(&box) : static_cast<b2Shape *>(&circle);
    body->CreateFixture(&fixture);
    m_bodies.push_back(body);
    m_visuals[index]->Edit<Transform>()->SetOrigin(ShapePosition(index));
    UpdateStatus();
}

void PhysicsSandbox::Remove() {
    if (m_bodies.empty()) return;
    EndDrag();
    m_visuals[m_bodies.size() - 1]->Edit<Transform>()->SetOrigin(Vec3(0, 0, -50000));
    m_world->DestroyBody(m_bodies.back()); m_bodies.pop_back();
    UpdateStatus();
}

void PhysicsSandbox::Reset() {
    EndDrag();
    while (!m_bodies.empty()) Remove();
    m_paused = false; m_gravity = true; m_accumulator = 0;
    m_world->SetGravity(b2Vec2(0, -10));
    for (int i = 0; i < 16; ++i) Spawn();
    m_lastUpdate = std::chrono::steady_clock::now();
}

void PhysicsSandbox::OnActivate(Entity *camera) {
    auto headset = camera->Edit<Headset>();
    headset->SetPosition(Vec3(0, 270, 1000));
    headset->SetOrientationOffset(Quaternion());
    m_lastUpdate = std::chrono::steady_clock::now();
}

void PhysicsSandbox::OnDeactivate() { EndDrag(); }

Vec3 PhysicsSandbox::ShapePosition(size_t index) const {
    if (index >= m_bodies.size()) return Vec3();
    const auto p = m_bodies[index]->GetPosition(); return Vec3(p.x * units, p.y * units, 0);
}
float PhysicsSandbox::ShapeAngle(size_t index) const {
    return index < m_bodies.size() ? m_bodies[index]->GetAngle() : 0;
}

Vec3 PhysicsSandbox::PointerOnBoard(const Vec3 &pointer) const {
    auto camera = Cameras::mainCamera;
    auto headset = camera->Get<Headset>();
    Vec3 far = camera->Get<RayTracer>()->PointerToWorld(pointer,
        AABB(0, 0, Device::GetDisplayScreenWidth(), Device::GetDisplayScreenHeight()),
        headset->GetProjection(), headset->GetLookAtMatrix());
    Vec3 origin = headset->GetWorldPosition(), ray = far - origin;
    if (std::fabs(ray[2]) < 0.00001f) return Vec3(0, 0, -50000);
    Vec3 point = origin + ray * (-origin[2] / ray[2]);
    return point;
}

void PhysicsSandbox::EndDrag() {
    if (m_drag) { m_world->DestroyJoint(m_drag); m_drag = nullptr; }
}

void PhysicsSandbox::ProcessInput() {
    const auto events = m_client->GetEventQueue()->GetCurrentEvents();
    for (int i = 0; i < events->Size(); ++i) {
        auto event = events->At(i);
        auto base = event->Edit<BaseEvent>();
        if (base->IsConsumed()) continue;
        if (event->ContainsComponent<keyboard::KeyRelease>()) {
            const wchar_t key = event->Get<keyboard::KeyRelease>()->GetButtonValue();
            switch (key) {
                case ' ': Spawn(); break;
                case 8: case 127: Remove(); break;
                case 'p': case 'P': m_paused = !m_paused; m_accumulator = 0; UpdateStatus(); break;
                case 'g': case 'G':
                    m_gravity = !m_gravity; m_world->SetGravity(b2Vec2(0, m_gravity ? -10 : 0));
                    for (auto body : m_bodies) body->SetAwake(true);
                    UpdateStatus(); break;
                case 'r': case 'R': Reset(); break;
            }
        }
        if (event->ContainsComponent<pointer::Position>()) {
            Vec3 point = PointerOnBoard(event->Get<pointer::Position>()->GetPosition());
            b2Vec2 target(point[0] / units, point[1] / units);
            if (event->ContainsComponent<pointer::Select>()) {
                EndDrag();
                for (auto body : m_bodies) {
                    if (!body->GetFixtureList()->TestPoint(target)) continue;
                    b2MouseJointDef joint; joint.bodyA = m_anchor; joint.bodyB = body;
                    joint.target = target; joint.maxForce = 1500 * body->GetMass();
                    joint.frequencyHz = 6; joint.dampingRatio = 0.8f;
                    m_drag = static_cast<b2MouseJoint *>(m_world->CreateJoint(&joint));
                    body->SetAwake(true); break;
                }
            } else if (event->ContainsComponent<pointer::Drag>() && m_drag) {
                target.x = hd::clamp(target.x, -4.85f, 4.85f);
                target.y = hd::clamp(target.y, 0.28f, 4.84f);
                m_drag->SetTarget(target);
            } else if (event->ContainsComponent<pointer::Release>()) {
                bool wasDragging = m_drag != nullptr; EndDrag();
                if (wasDragging) hd::sound::SoundController::PlayEffect(FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav").c_str());
            }
        }
        // Keep this planar sandbox's pointer and keyboard input out of the free-flight camera.
        if (event->ContainsComponent<pointer::Position>() || event->ContainsComponent<keyboard::KeySelect>() ||
            event->ContainsComponent<keyboard::KeyRelease>() || event->ContainsComponent<device::Keyboard>())
            base->ConsumeEvent(0);
    }
}

void PhysicsSandbox::UpdateStatus() {
    if (!m_status) return;
    std::string value = std::to_string(m_bodies.size()) + " / 64 shapes   |   " +
        (m_paused ? "Paused" : "Running") + "   |   Gravity " + (m_gravity ? "on" : "off");
    auto font = m_status->Edit<ui::Font>();
    m_status->Edit<ui::Text>()->SetText(value);
    auto mesh = m_status->Edit<ui::FontMesh>(); mesh->RenderMesh(font, value);
    auto buffer = m_status->Edit<VertexBuffer>(); buffer->Reset(); buffer->AddMeshes(mesh->GetMeshData());
    buffer->SetDefaultAttributeLocations(m_status->Edit<Shader>());
    m_status->Edit<Transform>()->SetOrigin(Vec3(0, 575, 50) - font->GetSize(value).Half());
}

void PhysicsSandbox::Update(Entity *camera) {
    auto headset = camera->Edit<Headset>();
    const float distance = std::max(1000.0f, 650.0f * headset->GetProjection()[0]);
    headset->SetPosition(Vec3(0, 270, distance));
    const auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - m_lastUpdate).count(); m_lastUpdate = now;
    if (!m_paused) {
        m_accumulator += std::min(elapsed, 0.1);
        while (m_accumulator >= step) { m_world->Step(float(step), 8, 3); m_accumulator -= step; ++m_steps; }
    }
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        auto transform = m_visuals[i]->Edit<Transform>();
        transform->SetOrigin(ShapePosition(i));
        transform->SetRotation(Quaternion(Axis_Z, ShapeAngle(i), Rotate_CCW));
    }
    AppPrototype::Update(camera);
}
