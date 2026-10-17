#ifndef _APPS_PIANO_H_
#define _APPS_PIANO_H_
#include "common/AppPrototype.h"

class Piano : public AppPrototype {
public:
    explicit Piano(hd::application::Client *client);
    ~Piano();
    void OnActivate(hd::Entity *camera) override;
    void OnDeactivate() override;
    int LastPlayedNote() const { return m_lastNote; }
    unsigned int QueuedAudioBytes() const;
private:
    unsigned int m_audioDevice;
    int m_sampleRate;
    int m_lastNote;
    void OnKeySelect(hd::Entity *element, const Collision3 &collision);
    void OnKeyRelease(hd::Entity *element, const Collision3 &collision);
    bool PlayNote(int note);
};
#endif
