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

#include "SkirmishScene.h"
//#include "GameLayer.h"

using namespace cocos2d;

namespace Warrior 
{

enum 
{
    kTagSkirmishLayer   = 0,
    kTagGameLayer       = 10,
    kTagHUD             = 20,
    kTagPauseGame       = 100,
};

SkirmishScene::SkirmishScene()
    : hud_layer_(NULL)
    , game_layer_(NULL)
    , layer_(NULL)
    , pause_game_layer_(NULL)
{
    ////////////////////////////////////////////////////////////
    ///
    /// Scene
    /// |-- HUD Layer
    /// |-- Game Layer
    ///     |-- Skirmish Layer
    ///
    ////////////////////////////////////////////////////////////
    hud_layer_ = Warrior::SkirmishHUD::create();
    addChild(hud_layer_, kTagHUD, kTagHUD);

    game_layer_ = cocos2d::CCLayer::create();
    this->addChild(game_layer_, kTagGameLayer, kTagGameLayer);

    layer_ = new Warrior::SkirmishLayer();
    //layer_ = Warrior::SkirmishLayer::create();
    game_layer_->addChild(layer_, kTagSkirmishLayer, kTagSkirmishLayer);

    //cocos2d::CCFollow* follow = cocos2d::CCFollow::create(layer_->player_node_);
    //game_layer_->runAction(follow);
}

SkirmishScene::~SkirmishScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

} // namespace
