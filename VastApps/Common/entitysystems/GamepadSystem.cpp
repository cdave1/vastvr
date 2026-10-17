#include "GamepadSystem.h"

using namespace hd;
using namespace hd::input;
using namespace hd::graphics;
using namespace hd::application;

GamepadSystem::GamepadSystem(EntityPool *entityPool,
                                   Client *client,
                                   const std::string entityName) :
EntitySystem(entityPool),
m_client(client),
m_entityName(entityName) {}





void GamepadSystem::HandleInput() {
	hdAssert(m_client);
	hdAssert(m_client->GetEventQueue());

	const hd::Vector<Entity *> *events = m_client->GetEventQueue()->GetCurrentEvents();

	for (int i = 0; i < events->Size(); ++i) {
		Entity *event = events->At(i);

		auto baseEvent = event->Get<BaseEvent>();
		hdAssert(baseEvent);
		hdAssert(baseEvent->GetFrame() == m_client->GetTimer()->GetCurrentFrame());
		if (baseEvent->IsConsumed()) {
			continue;
		}

		if (event->ContainsComponent<device::Gamepad>()) {
			GamepadQuery(event);
		}
	}
}


void GamepadSystem::GamepadQuery(hd::Entity *event) {
	auto gamepad = event->Get<device::Gamepad>();
	auto transform = mainCamera->Get<Transform>();
	auto headset = mainCamera->Edit<Headset>();

	Vec3 position = headset->GetPosition();
	Quaternion rotation = headset->GetWorldOrientation();

	const float ROTATION_SPEED = 0.025f;
	const float MOVEMENT_SPEED = 10.0f;

	if (gamepad->IsControlActive(gamepad::Control::LEFT_THUMBSTICK)) {
		auto axis = gamepad->GetAxis(gamepad::Control::LEFT_THUMBSTICK);

		Vec3 forward = Vec3(0, 0, -axis->GetDirection()[1]);
		Vec3 sideways = Vec3(-axis->GetDirection()[0], 0, 0);

		Vec3 result = forward + sideways;
		hd::normalize(result);
		result = rotation * result;
		headset->SetPosition(position + (MOVEMENT_SPEED * result));
	}

	if (gamepad->IsControlActive(gamepad::Control::RIGHT_THUMBSTICK)) {
		auto axis = gamepad->GetAxis(gamepad::Control::RIGHT_THUMBSTICK);

		Quaternion vert(Vec3(0, 1, 0), -axis->GetDirection()[0] * ROTATION_SPEED);
		//Quaternion horiz(Vec3(1, 0, 0), axis->GetDirection()[1] * ROTATION_SPEED);

		Quaternion rotXYZ = vert; // * horiz;
		headset->RotateBy(rotXYZ);
	}
}
