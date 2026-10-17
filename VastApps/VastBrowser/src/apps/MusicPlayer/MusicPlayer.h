#ifndef _APPS_MUSICPLAYER_H_
#define _APPS_MUSICPLAYER_H_
#include "common/AppPrototype.h"

class MusicPlayer : public AppPrototype {
public:
    explicit MusicPlayer(hd::application::Client *client);
    bool PlayAudio(const std::string &path);
    void StopAudio();
    bool HasActiveAudio() const { return m_effect != 0; }
    void OnDeactivate() override;
private:
    unsigned int m_effect;
    hd::Entity *m_status;
    void SetStatus(const std::string &text);
};
#endif
