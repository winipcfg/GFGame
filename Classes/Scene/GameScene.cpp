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

#include "GameScene.h"

using namespace cocos2d;

namespace GFGame { namespace Scene  
{

enum 
{
    kTagPauseGame   = 999,
};

GameScene::GameScene()
    : pause_game_layer_(NULL)
    , paused_(false)
{
}

GameScene::~GameScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void GameScene::PauseSchedulerAndActionsRecursive(cocos2d::CCNode* node)
{
    if (node)
    {
        node->pauseSchedulerAndActions();
        
        cocos2d::CCObject* child;
        CCARRAY_FOREACH(node->getChildren(), child)
        {
            PauseSchedulerAndActionsRecursive((cocos2d::CCNode*) child);
        }
    }
}

void GameScene::ResumeSchedulerAndActionsRecursive(cocos2d::CCNode* node)
{
    if (node)
    {
        node->resumeSchedulerAndActions();
        
        cocos2d::CCObject* child;
        CCARRAY_FOREACH(node->getChildren(), child)
        {
            ResumeSchedulerAndActionsRecursive((cocos2d::CCNode*) child);
        }
    }
}

void GameScene::PauseGame()
{
    if (!paused_)
    {
        CCLOG("[%s][%d] - The game changes to Paused state", __FUNCTION__, __LINE__);
        PauseSchedulerAndActionsRecursive(this);

        pause_game_layer_ = GFGame::Scene::PauseGameLayer::create();
        addChild(pause_game_layer_, kTagPauseGame, kTagPauseGame);
        paused_ = true;        
    }
}

void GameScene::ResumeGame()
{
    if (paused_)
    {
        CCLOG("[%s][%d] - The game resumes to Normal state", __FUNCTION__, __LINE__);
        ResumeSchedulerAndActionsRecursive(this);
        paused_ = false;
    }
}

} } // namespace
