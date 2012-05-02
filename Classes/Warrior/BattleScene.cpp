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

#include "BattleScene.h"

using namespace cocos2d;

namespace Warrior 
{

enum 
{
    kTagBattleLayer = 0,
    kTagGameLayer   = 10,
    kTagHUD         = 20,
    kTagBox2dDebug  = 30,
    kTagPauseGame   = 100,
};

BattleScene::BattleScene()
    : hud_layer_(NULL)
    , game_layer_(NULL)
    , physics_debug_viewer(NULL)
    , battle_layer_(NULL)
    , pause_game_layer_(NULL)
{
    ////////////////////////////////////////////////////////////
    ///
    /// Scene
    /// |-- HUD Layer
    /// |-- Game Layer
    ///     |-- Battle Layer
    ///     |-- Physics Debug Layer
    ///
    ////////////////////////////////////////////////////////////
    hud_layer_ = Warrior::BattleHUD::node();
    addChild(hud_layer_, kTagHUD, kTagHUD);

    game_layer_ = cocos2d::CCLayer::node();
    this->addChild(game_layer_, kTagGameLayer, kTagGameLayer);

    battle_layer_ = Warrior::BattleLayer::node();
    battle_layer_->SetViewer(hud_layer_);
    game_layer_->addChild(battle_layer_, kTagBattleLayer, kTagBattleLayer);

    physics_debug_viewer = GFGame::Viewer::Box2dDebugViewer::node();
    physics_debug_viewer->SetWorld(battle_layer_->GetBattle()->World(), &battle_layer_->GetBattle()->PhysicsSettings());
    physics_debug_viewer->setIsVisible(false);
    game_layer_->addChild(physics_debug_viewer, kTagBox2dDebug, kTagBox2dDebug);
}

BattleScene::~BattleScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void BattleScene::TogglePhysicsDebugViewer()
{
    physics_debug_viewer->setIsVisible(!physics_debug_viewer->getIsVisible());
}


} // namespace
