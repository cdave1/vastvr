// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#include "sound/SoundController.h"
#import <AppKit/AppKit.h>
#include <dispatch/dispatch.h>

@interface VastEffectPlayer : NSObject <NSSoundDelegate> {
    NSMutableDictionary *sounds;
    unsigned int nextIdentifier;
}
+ (VastEffectPlayer *)shared;
- (unsigned int)play:(const char *)path gain:(float)gain loops:(bool)loops;
- (void)stop:(unsigned int)identifier;
@end

@implementation VastEffectPlayer
+ (VastEffectPlayer *)shared {
    static VastEffectPlayer *player;
    static dispatch_once_t once;
    dispatch_once(&once, ^{ player = [[VastEffectPlayer alloc] init]; });
    return player;
}
- (id)init {
    if ((self = [super init])) {
        sounds = [[NSMutableDictionary alloc] init];
        nextIdentifier = 1;
    }
    return self;
}
- (unsigned int)play:(const char *)path gain:(float)gain loops:(bool)loops {
    if (!path) return 0;
    NSString *file = [NSString stringWithUTF8String:path];
    if (!file) return 0;
    NSSound *sound = [[NSSound alloc] initWithContentsOfFile:file byReference:NO];
    if (!sound) {
        hd::log::e("[SoundController]: Could not load %s\n", path);
        return 0;
    }
    [sound setVolume:gain];
    [sound setLoops:loops];
    [sound setDelegate:self];
    @synchronized (sounds) {
        while (nextIdentifier == 0 || [sounds objectForKey:@(nextIdentifier)]) ++nextIdentifier;
        unsigned int identifier = nextIdentifier++;
        [sounds setObject:sound forKey:@(identifier)];
        bool started = [sound play];
        if (!started) [sounds removeObjectForKey:@(identifier)];
        [sound release];
        return started ? identifier : 0;
    }
}
- (void)stop:(unsigned int)identifier {
    @synchronized (sounds) {
        NSSound *sound = [[sounds objectForKey:@(identifier)] retain];
        [sound setDelegate:nil];
        [sound stop];
        [sounds removeObjectForKey:@(identifier)];
        [sound release];
    }
}
- (void)sound:(NSSound *)sound didFinishPlaying:(BOOL)finished {
    @synchronized (sounds) {
        NSArray *keys = [sounds allKeysForObject:sound];
        [sounds removeObjectsForKeys:keys];
    }
}
@end

unsigned int hd::sound::SoundController::PlayEffect(const char *path) {
    return PlayEffect(path, 1.0f, 0.0f, 1.0f, false);
}
unsigned int hd::sound::SoundController::PlayEffectWithLoop(const char *path) {
    return PlayEffect(path, 1.0f, 0.0f, 1.0f, true);
}
void hd::sound::SoundController::StopEffect(unsigned int identifier) {
    [[VastEffectPlayer shared] stop:identifier];
}
unsigned int hd::sound::SoundController::PlayEffect(const char *path, float pitch,
                                                   float pan, float gain, bool loops) {
    // NSSound supports volume and looping; legacy pitch/pan parameters have no
    // macOS implementation. Keep the cross-platform API without guessing units.
    (void)pitch;
    (void)pan;
    return [[VastEffectPlayer shared] play:path gain:gain loops:loops];
}
