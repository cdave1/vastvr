// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import <AppKit/AppKit.h>
#include "LocalAudio.h"

std::string ChooseLocalAudioFile() {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.title = @"Choose your own audio file";
    panel.canChooseDirectories = NO;
    panel.allowsMultipleSelection = NO;
    panel.allowedFileTypes = @[@"wav", @"aif", @"aiff", @"mp3", @"m4a", @"caf"];
    panel.level = NSMainMenuWindowLevel + 2;
    if ([panel runModal] != NSModalResponseOK) return std::string();
    return std::string(panel.URL.path.UTF8String);
}
