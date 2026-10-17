// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#ifndef VAST_PHYSICS_SANDBOX_H
#define VAST_PHYSICS_SANDBOX_H
#include "common/AppPrototype.h"
#include <Box2D/Box2D.h>
#include <chrono>
#include <memory>
#include <vector>

class PhysicsSandbox : public AppPrototype {
public:
    explicit PhysicsSandbox(hd::application::Client *client);
    ~PhysicsSandbox();
    void OnActivate(hd::Entity *camera) override;
    void OnDeactivate() override;
    void ProcessInput();
    size_t ShapeCount() const { return m_bodies.size(); }
    bool IsPaused() const { return m_paused; }
    bool HasGravity() const { return m_gravity; }
    bool IsDragging() const { return m_drag != nullptr; }
    Vec3 ShapePosition(size_t index) const;
    float ShapeAngle(size_t index) const;
    unsigned int PhysicsSteps() const { return m_steps; }
private:
    void Update(hd::Entity *camera) override;
    void Reset();
    void Spawn();
    void Remove();
    void EndDrag();
    void UpdateStatus();
    Vec3 PointerOnBoard(const Vec3 &pointer) const;
    hd::Entity *CreateVisual(size_t index);
    void AddWall(float width, float height, Vec3 center);
    std::unique_ptr<b2World> m_world;
    b2Body *m_anchor;
    b2MouseJoint *m_drag;
    std::vector<b2Body *> m_bodies;
    std::vector<hd::Entity *> m_visuals;
    hd::Entity *m_status;
    bool m_paused, m_gravity;
    unsigned int m_steps;
    double m_accumulator;
    std::chrono::steady_clock::time_point m_lastUpdate;
};
#endif
