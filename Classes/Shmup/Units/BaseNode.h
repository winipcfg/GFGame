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
#ifndef GFGAME_SHMUP_BASE_NODE_H_
#define GFGAME_SHMUP_BASE_NODE_H_

#include <Box2D/Box2D.h>
#include <Cistron/Cistron.h>
#include "cocos2d.h"
#include "Components/PhysicsComponent.h"
#include "Components/RenderComponent.h"
#include <CCExtensions/AdvanceSprite.h>

namespace GFort { namespace Games { namespace Shmup 
{

class BaseNode 
    : public cocos2d::CCNode,
      public Cistron::Component
{    
public:
    /// Constructor.
    /// @param spriteFile
    BaseNode(const std::string& spriteFile);

    /// Destructor.
    ~BaseNode();
    
    /// Gets the bounding box.
    cocos2d::CCRect boundingBox(void);

    /// Gets the content size.
    cocos2d::CCSize getContentSize(void) ;

    /// Assign physics body to the unit.
    /// @param body
    void SetBody(b2Body* body)                              { body_ = body; }
       
protected:
    /// Update the node.
    /// @param dt
    virtual void UpdateNode(cocos2d::CCFloat dt);

    /// Destroy itself.
    virtual void Destroy();

private:
    // Main node and sprite for actions and animations
    cocos2d::CCSprite*                      sprite_;

public:
    // Physics body
    b2Body*                                 body_;
    GFGame::Components::PhysicsComponent*   physics_component_;
};

} } } // namespace

#endif // GFGAME_SHMUP_BASE_NODE_H_
