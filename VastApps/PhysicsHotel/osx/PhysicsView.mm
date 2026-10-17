// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import "PhysicsView.h"
#include "entitysystems/Cameras.h"
@implementation PhysicsView
- (BOOL)acceptsFirstResponder { return YES; }
- (BOOL)acceptsFirstMouse:(NSEvent *)event { return YES; }
- (void)syncBounds {
    NSSize size = self.bounds.size;
    if (size.width <= 0 || size.height <= 0) return;
    hd::Device::SetDisplayScreenSize(size.width, size.height);
    if (!m_client || !Cameras::mainCamera) return;
    auto headset = Cameras::mainCamera->Edit<hd::input::Headset>();
    Matrix projection = headset->GetProjection();
    projection[0] = projection[5] * size.height / size.width;
    headset->SetProjection(hd::input::EyePosition::Center, projection);
    headset->SetViewport(hd::input::EyePosition::Center, AABB(0, 0, size.width, size.height));
}
- (void)reshape { [super reshape]; [self syncBounds]; }
- (void)render { [self syncBounds]; [super render]; }
@end
