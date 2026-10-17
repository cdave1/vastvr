#include "SphereLayout.h"
#include <Vast/VastGraphics.h>

hd::ui::SphereLayout::SphereLayout(const uint32_t rows, const uint32_t columns,
                                   float sphereRadius, const float arcWidth) :
m_rows(rows), m_columns(columns), m_sphereRadius(sphereRadius), m_arcWidth(arcWidth) {}


void hd::ui::SphereLayout::PositionOnSphere(const uint32_t row, const uint32_t col,
                                            Vec3 *_pos, Quaternion *_rotation, Vec2 offset) const {
    float arcWidth = m_arcWidth;
    float arcHeight = (m_arcWidth / hd::max(uint32_t(1), m_columns)) * m_rows;

    float horizontalStepRadians = (arcWidth / hd::max(uint32_t(1), m_columns));
    float verticalStepRadians = (arcHeight / hd::max(uint32_t(1), m_rows));

    Quaternion horizontalStep(Vec3(0, -1, 0), (col * horizontalStepRadians) - (0.5f * arcWidth) + (offset[0] * horizontalStepRadians));
    Quaternion verticalStep(Vec3(1, 0, 0), (row * verticalStepRadians) - (0.5f * arcHeight) + (offset[1] * verticalStepRadians));

    Vec3 position = (Vec3(0, 0, -1) * m_sphereRadius);

    Quaternion rotation = Quaternion::LookAt(hd::normalize(position), Vec3(), Vec3(0, 1, 0));
    rotation = rotation * horizontalStep;
    rotation = rotation * verticalStep;
    position = rotation * position;
    rotation.Invert();

    *_pos = position;
    *_rotation = rotation;
}


void hd::ui::SphereLayout::PositionOnSphere(const uint32_t row, const uint32_t col,
                                            hd::graphics::Transform *transform, Vec2 offset) const {
    Vec3 position;
    Quaternion rotation;

    this->PositionOnSphere(row, col, &position, &rotation, offset);

    transform->SetOrigin(position);
    transform->SetRotation(rotation);
}


void hd::ui::SphereLayout::PositionOnSphere(const uint32_t row, const uint32_t col,
                                            const hd::Entity *entity, Vec2 offset) const {
    auto transform = entity->Get<hd::graphics::Transform>();
    if (transform) {
        Vec3 position;
        Quaternion rotation;

        this->PositionOnSphere(row, col, &position, &rotation, offset);

        transform->SetOrigin(position);
        transform->SetRotation(rotation);
    }
}

Vec2 hd::ui::SphereLayout::GetGridNodeSize(const uint32_t row, const uint32_t col) const {
    Vec3 p0, p1, p2;
    Quaternion q0, q1, q2;

    PositionOnSphere(0, 0, &p0, &q0);
    PositionOnSphere(1, 0, &p1, &q1);
    PositionOnSphere(0, 1, &p2, &q2);

    Vec3 horizontalDiff = p0 - p1;
    Vec3 verticalDiff = p0 - p2;

    return Vec2(horizontalDiff.Length(), verticalDiff.Length());
}
