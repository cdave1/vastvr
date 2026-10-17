//
//  hdUILabel.h
//  hdk.ios
//
//  Created by David Petrie on 23/07/11.
//  Copyright (c) 2011 David Petrie. SPDX-License-Identifier: MIT
//

#ifndef HD_UI_LABEL_H
#define HD_UI_LABEL_H

#include "UI/hdUIReceiver.h"
#include <string>

using namespace std;


class hdUILabel : public hdUIReceiver
{
public:
    
    hdUILabel(const char *fontName, const float faceSize);
    
    ~hdUILabel();
    
    void SetFontScale(const float scale);
    
    void SetFaceSize(const float faceSize);
    
    void SetText(const char *text);
    
    void SetTextFormatted(const char *format, ... );
    
    
protected:
    
    virtual void UpdateFont();
    
    float m_scale;
    
    float m_scaleInverse;
    
    float m_faceSize;
    
    string m_text;
    
    FTFont *m_font;
    
    
private:
    
    void Init(const char *fontName, const float faceSize);
    
};


inline void hdUILabel::SetFontScale(const float scale)
{
    m_scale = scale;
    UpdateFont();
}


inline void hdUILabel::SetFaceSize(const float faceSize)
{
    m_faceSize = faceSize;
    UpdateFont();
} 


#endif