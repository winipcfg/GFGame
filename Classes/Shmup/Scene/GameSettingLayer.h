//Copyright (C) 2012 by Gavin Fong
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

#pragma once
#ifndef GFGAME_SHMUP_GAMESETTINGLAYER_H_
#define GFGAME_SHMUP_GAMESETTINGLAYER_H_

#include "cocos2d.h"
#include "cocos2dExt.h"
#include <Scene/GameScene.h>
#include "../Game.h"

namespace GFort { namespace Games { namespace Shmup 
{

class GameSettingLayer : public GFGame::Scene::GameScene
{
public:
    
public:
    /// Constructor.
    GameSettingLayer();

    /// Destructor.
    ~GameSettingLayer();

    void GameSettingLayer::ValueChanged(cocos2d::extension::CCControlSlider* sender);
    void GameSettingLayer::ButtonClicked(cocos2d::extension::CCControlButton* sender);
        
    LAYER_CREATE_FUNC(GameSettingLayer)

private:
    /// Load default setting.
    void LoadUserDefault();
    
    /// Save default setting.
    void SaveUserDefault();

private:
    cocos2d::CCLabelTTF*                        display_value_label_;
    cocos2d::extension::CCControlSlider*        music_volume_control_;
    cocos2d::extension::CCControlSlider*        audio_volume_control_;
    cocos2d::extension::CCControlSwitch*        show_hints_control_;
};

} } } // namespace

#endif // GFGAME_SHMUP_GAMESETTINGLAYER_H_
