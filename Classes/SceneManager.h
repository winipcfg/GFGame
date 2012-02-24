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

#ifndef GFGAME_SCENE_MANAGER_H_
#define GFGAME_SCENE_MANAGER_H_

#include "cocos2d.h"
#include "Scene.h"

class SceneManager
{
public:
    /// Go default scene.
    virtual void GoDefault() = 0;
    
protected:
    /// Go to scene.
    /// @param scene
    void GoScene(cocos2d::CCScene* scene); 
        
    /// Go to scene that is CClayer.
    /// @param layer
    void GoScene(cocos2d::CCLayer* layer);   

private:   
    /// Creates a scene that has given layer as the child.
    /// @param layer
    cocos2d::CCScene* WrapLayer(cocos2d::CCLayer* layer);
};

#endif // GFGAME_SCENE_MANAGER_H_
