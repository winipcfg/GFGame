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

#include "SceneManager.h"

#define kTransitionDuration				1.0f

void SceneManager::GoScene(cocos2d::CCScene* scene)
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    if (director->getRunningScene() != NULL)
    {        
        director->replaceScene(cocos2d::CCTransitionSplitCols::transitionWithDuration(kTransitionDuration, scene));
    }
    else
    {
        director->runWithScene(scene);
    }
}

void SceneManager::GoScene(cocos2d::CCLayer* layer)
{    
    cocos2d::CCScene* newScene = WrapLayer(layer);
    GoScene(newScene);
}

cocos2d::CCScene* SceneManager::WrapLayer(cocos2d::CCLayer* layer)
{
    cocos2d::CCScene* newScene = cocos2d::CCScene::node();
    newScene->addChild(layer);
    return newScene;
}
