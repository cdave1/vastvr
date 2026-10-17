#include "MusicPlayer.h"
#include <Vast/VastMath.h>
#include "SDL2/SDL.h"
//#include "SDL2/SDL_mixer.h"

#define BAR_DIVISIONS 120
#define MAX_AMPLITUDES 512

hd::sound::MusicPlayer::MusicPlayer() :
    div_size(0),
    m_largestAmplitude(0.0f),
    m_averageAmplitude(0.0f),
    m_amplitudes(NULL),
    m_isActive(false) {
        m_amplitudes = new hd::Vector<float>(MAX_AMPLITUDES);

        m_amplitudeHistory = new hd::Vector<float>(128);
}


hd::sound::MusicPlayer::~MusicPlayer() {
}


void hd::sound::MusicPlayer::Init() {
	/*
    int result = 0;
    int flags = MIX_INIT_MP3;
    
    div_size = (int)((short)0x7FFF)/BAR_DIVISIONS;
    
    if (flags != (result = Mix_Init(flags))) {
        hd::log::e("Could not initialize mixer (result: %d).\n", result);
        hd::log::e("Mix_Init: %s\n", Mix_GetError());
        return;
    }
    
	//128 is a 512 byte buffer
	//assume 128 refers to a single, stereo "sample"
//    Mix_OpenAudio(22050, ,2,512);
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        hd::log::e("Mix_Init: %s\n", Mix_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        hd::log::e("Mix_Init: %s\n", Mix_GetError());
    } */
}


void hd::sound::MusicPlayer::Stop() {
    m_isActive = false;
    //Mix_PauseMusic();
}


void hd::sound::MusicPlayer::PlayMusic(const std::string &path) {
    
	/*
    Mix_Music *music = Mix_LoadMUS(path.c_str());
    Mix_SetPostMix(&hd::sound::MusicPlayer::PostMix, this);
    Mix_PlayMusic(music, 1);
    m_isActive = true;*/
}


void hd::sound::MusicPlayer::PostMix(void *udata, uint8_t *stream, int len) {
    hd::sound::MusicPlayer *player = (hd::sound::MusicPlayer *)udata;
    player->HandlePostMix(stream, len);
}


void hd::sound::MusicPlayer::HandlePostMix(uint8_t *stream, int len) {
	union exchange {
		Uint8 bytes[2];
        short sample;
	} dat;
    
    const float maxAmplitude = float(pow(2, 15));
    
	int amp_value = 0;
    
    m_amplitudes->Clear();
	for (int i = 0; i < len; i++) {
		if (i%2 == 0) {
			dat.bytes[0] = stream[i];
			dat.bytes[1] = stream[i+1];
			amp_value = abs((short)dat.sample);
            
            m_amplitudes->Add(float(amp_value) / maxAmplitude);
		}
	}

    float sum = 0.0f;
    if (m_amplitudes->GetSize() == 0) {
        m_averageAmplitude = 0.0f;
    }

    for (int i = 0; i < m_amplitudes->GetSize(); ++i) {
        float v = m_amplitudes->At(i);
        sum += v;
    }

    m_averageAmplitude = sum / float(m_amplitudes->GetSize());
    if (m_amplitudeHistory->IsFull()) {
        m_amplitudeHistory->Pop();
    }
    m_amplitudeHistory->Enqueue(m_averageAmplitude);
}


hd::Vector<float> * hd::sound::MusicPlayer::GetAmplitudeHistory() const {
    return m_amplitudeHistory;
}


float hd::sound::MusicPlayer::GetAverageAmplitude() const {
    return m_averageAmplitude;
}


float hd::sound::MusicPlayer::GetLargestAmplitude() const {
    return m_largestAmplitude;
}


unsigned int hd::sound::MusicPlayer::GetAmplitudesCount() const {
    return m_amplitudes->GetSize();
}


float * hd::sound::MusicPlayer::GetAmplitudesPointer() const {
    float *data = m_amplitudes->GetItems();
    return data;
}


bool hd::sound::MusicPlayer::IsActive() const {
    return m_isActive;
}


void hd::sound::MusicPlayer::PrintAmplitudeBars(int amplitude) {
    int bars = hd::min(BAR_DIVISIONS, amplitude/div_size);
    
    static char bar_chars[BAR_DIVISIONS];
    memset(bar_chars, 0, BAR_DIVISIONS);
	for (int i = 0; i < bars; i++) {
        bar_chars[i] = '#';
	}
	hd::log::e("%s\n", bar_chars);
    fflush(stdout);
}
