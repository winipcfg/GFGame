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

#ifndef WARRIOR_SKIRMISH_SCENE_H_
#define WARRIOR_SKIRMISH_SCENE_H_

#include "cocos2d.h"
#include <Viewer/Box2dDebugViewer.h>
#include <Scene/GameScene.h>
#include <Scene/PauseGameLayer.h>
#include "SkirmishHUD.h"
#include "SkirmishLayer.h"

namespace Warrior 
{

class SkirmishScene : public GFGame::Scene::GameScene
{
public:
    
public:
    /// Constructor.
    SkirmishScene();

    /// Destructor.
    ~SkirmishScene();
    
    CREATE_FUNC(SkirmishScene)

protected:
private:
    // Stores layer reference
    SkirmishHUD*                        hud_layer_;
    cocos2d::CCLayer*                   game_layer_;    
    SkirmishLayer*                      layer_;
    GFGame::Scene::PauseGameLayer*      pause_game_layer_;
};

} // namespace

#endif // WARRIOR_SKIRMISH_SCENE_H_
