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

#include "ShmupScene.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

enum 
{
    kTagBattleLayer = 0,
    kTagHUD         = 20,
    kTagBox2dDebug  = 30,
};

ShmupScene::ShmupScene()
    : hud_layer_(NULL)
    , physics_debug_viewer(NULL)
    , shmup_layer_(NULL)
{
    hud_layer_ = Shmup::ShmupHUD::node();
    addChild(hud_layer_, kTagHUD, kTagHUD);

    game_.Initialize();
    game_.SetNumLives(1);

    shmup_layer_ = new ShmupLayer(&game_);
    //shmup_layer_->SetViewer(hud_layer_);
    addChild(shmup_layer_, kTagBattleLayer, kTagBattleLayer);

    physics_debug_viewer = GFGame::Viewer::Box2dDebugViewer::node();
    physics_debug_viewer->SetWorld(game_.World(), &game_.PhysicsSettings());
    physics_debug_viewer->setIsVisible(false);
    addChild(physics_debug_viewer, kTagBox2dDebug, kTagBox2dDebug);
}

ShmupScene::~ShmupScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void ShmupScene::TogglePhysicsDebugViewer()
{
    CCLOG("[%s][%d] - Toggle Physics debug viewer", __FUNCTION__, __LINE__);
    physics_debug_viewer->setIsVisible(!physics_debug_viewer->getIsVisible());
}


} } } // namespace
