#ifndef _VAST_MUSIC_PLAYER_H_
#define _VAST_MUSIC_PLAYER_H_

#include "core/Collections/Collections.h"

namespace hd {
    namespace sound {
        class MusicPlayer {
        private:
            int div_size;

            float m_largestAmplitude;

            float m_averageAmplitude;
            
            Vector<float> *m_amplitudes;

            hd::Vector<float> *m_amplitudeHistory;
            
            bool m_isActive;
            
        public:
            
            MusicPlayer();
            
            ~MusicPlayer();
            
            void Init();
            
            void Stop();
            
            void PlayMusic(const std::string &path);
            
            static void PostMix(void *udata, uint8_t *stream, int len);

            float GetLargestAmplitude() const;

            float GetAverageAmplitude() const;

            ///
            /// Returns history of average amplitudes.
            ///
            hd::Vector<float> * GetAmplitudeHistory() const;
            
            unsigned int GetAmplitudesCount() const;
            
            float * GetAmplitudesPointer() const;
            
            bool IsActive() const;
            
        private:
            
            void HandlePostMix(uint8_t *stream, int len);
            
            void PrintAmplitudeBars(int amplitude);
            
        };
    }
}

#endif
