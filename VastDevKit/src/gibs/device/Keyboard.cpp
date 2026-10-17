#include "Keyboard.h"
#include "SDL2/SDL.h"

using namespace hd::input;

device::Keyboard::Keyboard(unsigned int ID) : Component(ID),
  m_deviceId(hd::input::UnspecifiedDeviceId),
  m_keys()
{}


int device::Keyboard::GetDeviceId() const {
    return m_deviceId;
}


void device::Keyboard::Reset() {
    m_keys.clear();
}


void device::Keyboard::Update() {
}


void device::Keyboard::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void device::Keyboard::AddKey(hd::Device::Keycode keycode) {
    m_keys[keycode] = 1;
}


void device::Keyboard::RemoveKey(hd::Device::Keycode keycode) {
    m_keys.erase(keycode);
}


bool device::Keyboard::IsKeyDown(const hd::Device::Keycode keycode) const {
    return m_keys.count(keycode) > 0;
    
    
    
	/*
	SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    SDL_Scancode code = SDL_SCANCODE_UNKNOWN;
    
    switch (keycode) {
        case hd::Device::Keycode::W:
            code = SDL_SCANCODE_W;
            break;
        case hd::Device::Keycode::A:
            code = SDL_SCANCODE_A;
            break;
        case hd::Device::Keycode::S:
            code = SDL_SCANCODE_S;
            break;
        case hd::Device::Keycode::D:
            code = SDL_SCANCODE_D;
            break;
        default:
            break;
    }
    
    if (code == SDL_SCANCODE_UNKNOWN) {
        return false;
    } else {
        hd::log::i("Char for value: %d\n", keycode);
        return state[code] == 1;
    }*/
}


void device::Keyboard::CopyKeyboard(const Keyboard *keyboard) {
    /*
    std::map<hd::Device::Keycode, int> keys = keyboard->GetKeys();
    std::map<hd::Device::Keycode, int>::iterator iter;
    for (iter = keys.begin(); iter != keys.end(); ++iter) {
        this->AddKey(iter->first);
    }*/
    m_keys = keyboard->GetKeys();
}
