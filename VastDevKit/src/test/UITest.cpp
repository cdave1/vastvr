#include "UITest.h"

#include <Vast/VastUI.h>
#include <assert.h>
#include <float.h>

void hd::test::ui::TestSphereLayout() {
    const uint32_t rows = 10;
    const uint32_t columns = 5;
    const float sphereRadius = 100.0f;
    const float arcWidth = 0.2f * hd_pi;

    hd::ui::SphereLayout sphereLayout(rows, columns, sphereRadius, arcWidth);
    Vec2 nodeSize1 = sphereLayout.GetGridNodeSize();

    hd::log::i("%3.3f %3.3f\n", nodeSize1[0], nodeSize1[1]);
}
