//Copyright (C) 2011 by Gavin Fong
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#ifndef WARRIOR_SKIRMISH_HUD_H_
#define WARRIOR_SKIRMISH_HUD_H_

#include <string>
#include <Cistron/Cistron.h>
#include "cocos2d.h"

namespace Warrior 
{

/// Class for HUD in Skirmish.
class SkirmishHUD : public cocos2d::CCLayer, public Cistron::Component
{
public:
    typedef cocos2d::CCPoint     Point;
    
public:
    /// Constructor.
    SkirmishHUD();

    /// Destructor.
    ~SkirmishHUD();
    
    /// Sets title of the screen.
    /// @param value
    void SetTitle(const int& value);
    
    LAYER_NODE_FUNC(SkirmishHUD)
   
protected:
private:
    void SetupViewer();

    void ButtonPauseCallback(CCObject* pSender);
    void ButtonToggleDebugCallback(CCObject* pSender);
        
private:
    // Stores data for display
    int                             stage_;
    std::string                     title_;

    // Stores CCNode objects
    cocos2d::CCSprite*              label_stage_background_;
    cocos2d::CCLabelTTF*            label_stage_;
    cocos2d::CCMenuItemImage*       button_pause_;
    cocos2d::CCMenuItemImage*       button_debug_;
};

} // namespace

#endif // WARRIOR_SKIRMISH_HUD_H_
