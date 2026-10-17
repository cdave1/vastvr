#include "ComponentTest.h"

#include <Vast/Vast.h>

void hd::test::components::TestComponentSpeed() {
    auto c1 = new ComponentPool<hd::graphics::Material>(1024);
    auto c2 = new ComponentPool<hd::graphics::Transform>(1024);
    auto c3 = new ComponentPool<hd::graphics::Curve>(1024);

    ComponentTypeValue c1ValueType = ComponentType::value<hd::graphics::Material>();
    ComponentTypeValue c2ValueType = ComponentType::value<hd::graphics::Transform>();
    ComponentTypeValue c3ValueType = ComponentType::value<hd::graphics::Curve>();

    hdAssert(c1ValueType == ComponentType::value<hd::graphics::Material>());
    hdAssert(c2ValueType == ComponentType::value<hd::graphics::Transform>());
    hdAssert(c3ValueType == ComponentType::value<hd::graphics::Curve>());

    auto components = new ComponentPool<hd::graphics::Material>(1024);
    auto entities = new hd::EntityPool(4096);

    hdTimestamp start = hd::Device::GetMediaTime();

    const unsigned int ITERATIONS = 10000;
    for (int i = 0; i < ITERATIONS; ++i) {
        unsigned int j = 0;
        while (j < entities->GetEntities()->Capacity()) {
            auto ent = entities->Create();
            hdAssert(ent != nullptr);

            auto renderer = components->Get();
            ent->AddComponent(renderer);

            if (ent->ContainsComponent<hd::graphics::Material>()) {
                auto com = ent->GetComponent<hd::graphics::Material>();
                hdAssert(com != nullptr);
            }
            ++j;
        }

        entities->FreeAll();
    }

    hdTimestamp end = hd::Device::GetMediaTime();

    hd::log::i("Took %3.9f\n", end - start);

   // hdAssert((end - start) < 5.0);

    hdDeleteSafely(entities);
    hdDeleteSafely(components);
}
