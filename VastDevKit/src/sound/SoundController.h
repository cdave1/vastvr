
#ifndef _VAST_SOUND_MANAGER_H_
#define _VAST_SOUND_MANAGER_H_

namespace hd {
    namespace sound {
        class SoundController {
        public:
            
            static unsigned int PlayEffect(const char *filePath);
            
            static unsigned int PlayEffectWithLoop(const char *filePath);
            
            static void StopEffect(unsigned int soundId);
            
            static unsigned int PlayEffect(const char *filePath, const float pitch, const float pen, const float gain, bool loops);
            
            
        };
    }
}

#endif

