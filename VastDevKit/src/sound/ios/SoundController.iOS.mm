#include "SoundController.h"
#import "CDAudioManager.h"

#pragma mark -
#pragma mark Singleton Functions

static NSObject * lock = [[NSObject alloc] init];
static CDSoundEngine * soundEngine = nil;
static CDAudioManager * am = nil;
static CDBufferManager * bufferManager = nil;
static bool m_mute;
static bool m_enabled;

/*
void hd::sound::SoundController::InitSoundManager()
{    
    @synchronized(lock)     
    {
        am = [CDAudioManager sharedManager];
        soundEngine = am.soundEngine;
        bufferManager = [[CDBufferManager alloc] initWithEngine:soundEngine];
        m_mute = NO;
        m_enabled = YES;
    }
}


void hd::sound::SoundController::SetProxyPlayer(void * proxy)
{
}


void hd::sound::SoundController::End()
{
    am = nil;
    [CDAudioManager end];
    ReleaseSafely(bufferManager);
    soundEngine = nil;
    bufferManager = nil;
    ReleaseSafely(lock);
}


#pragma mark -
#pragma mark Public Functions

void hd::sound::SoundController::PreloadBackgroundMusic(const char * filePath)
{
    [am preloadBackgroundMusic:[NSString stringWithUTF8String:filePath]];
}


void hd::sound::SoundController::PlayBackgroundMusic(const char * filePath)
{
    [am playBackgroundMusic:[NSString stringWithUTF8String:filePath] loop:TRUE];
}


void hd::sound::SoundController::PlayBackgroundMusic(const char * filePath, const bool loops)
{
    [am playBackgroundMusic:[NSString stringWithUTF8String:filePath] loop:loops];
}


void hd::sound::SoundController::StopBackgroundMusic()
{
    [am stopBackgroundMusic];
}


void hd::sound::SoundController::PauseBackgroundMusic()
{
    [am pauseBackgroundMusic];
}


void hd::sound::SoundController::ResumeBackgroundMusic()
{
    [am resumeBackgroundMusic];
}


void hd::sound::SoundController::RewindBackgroundMusic()
{
    [am rewindBackgroundMusic];
}


bool hd::sound::SoundController::IsBackgroundMusicPlaying()
{
    return [am isBackgroundMusicPlaying];
}


void hd::sound::SoundController::PreloadSounds(const char * folderPath)
{
}*/


unsigned int hd::sound::SoundController::PlayEffect(const char * filePath)
{
    return hd::sound::SoundController::PlayEffect(filePath, 1.0f, 0.0f, 1.0f, false);
}


unsigned int hd::sound::SoundController::PlayEffectWithLoop(const char * filePath)
{
    return hd::sound::SoundController::PlayEffect(filePath, 1.0f, 0.0f, 1.0f, true);    
}


void hd::sound::SoundController::StopEffect(unsigned int soundId)
{
    [soundEngine stopSound:soundId];
}


unsigned int hd::sound::SoundController::PlayEffect(const char * filePath, const float pitch, const float pan, const float gain, bool loops)
{
    int soundId = [bufferManager bufferForFile:[NSString stringWithUTF8String:filePath] create:YES];
    if (soundId != kCDNoBuffer) {
        return [soundEngine playSound:soundId sourceGroupId:0 pitch:pitch pan:pan gain:gain loop:loops];
    } else {
        return CD_MUTE;
    }    
}

/*
void hd::sound::SoundController::PreloadEffect(const char * filePath)
{
    int soundId = [bufferManager bufferForFile:[NSString stringWithUTF8String:filePath] create:YES];
    if (soundId == kCDNoBuffer) {
        cprintf("Denshion::SimpleAudioEngine sound failed to preload %s",filePath);
    } else {
        cprintf("Denshion::SimpleAudioEngine preloaded %s",filePath);
    }
}


void hd::sound::SoundController::UnloadEffect(const char * filePath)
{
    cprintf("Denshion::SimpleAudioEngine unloadedEffect %s",filePath);
    [bufferManager releaseBufferForFile:[NSString stringWithUTF8String:filePath]];
}


float hd::sound::SoundController::GetBackgroundMusicVolume()
{
    return am.backgroundMusic.volume;
}


void hd::sound::SoundController::SetBackgroundMusicVolume(const float volume)
{
    am.backgroundMusic.volume = volume;
}


float hd::sound::SoundController::GetEffectsVolume()
{
    return am.soundEngine.masterGain;
}


void hd::sound::SoundController::SetEffectsVolume(const float volume)
{
    am.soundEngine.masterGain = volume;
}


bool hd::sound::SoundController::IsMute()
{
    return m_mute;
}


void hd::sound::SoundController::SetMute(const bool mute)
{
    if (m_mute != mute) {
        m_mute = mute;
        am.mute = mute;
    }    
}


bool hd::sound::SoundController::IsEnabled()
{
    return m_enabled;
}


void hd::sound::SoundController::SetEnabled(const bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        am.enabled = m_enabled;
    }    
}


// Readonly
bool hd::sound::SoundController::GetWillPlayBackgroundMusic()
{
    return [am willPlayBackgroundMusic];
}*/

