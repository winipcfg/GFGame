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
#ifndef GFGAME_SHMUP_SHMUPSCENE_H_
#define GFGAME_SHMUP_SHMUPSCENE_H_

#include "cocos2d.h"
#include <Viewer/Box2dDebugViewer.h>
#include <Scene/GameScene.h>
#include "Shmup/Viewer/ShmupHUD.h"
#include "ShmupLayer.h"
#include "Game.h"

namespace GFort { namespace Games { namespace Shmup 
{

class ShmupScene : public GFGame::Scene::GameScene
{
public:
    
public:
    /// Constructor.
    ShmupScene();

    /// Destructor.
    ~ShmupScene();

    /// Toggles whether the physics debug viewer is visible or not.
    void TogglePhysicsDebugViewer();
    
    SCENE_NODE_FUNC(ShmupScene)

protected:
private:
    // Game itself
    Game            game_;

    // Stores layer reference
    ShmupHUD*                           hud_layer_;
    GFGame::Viewer::Box2dDebugViewer*   physics_debug_viewer;
    ShmupLayer*                         shmup_layer_;
};

} } } // namespace

#endif // GFGAME_SHMUP_SHMUPSCENE_H_
