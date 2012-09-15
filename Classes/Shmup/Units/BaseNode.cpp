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

#include "BaseNode.h"
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "CCExtensions/CCSpriteHelper.h"
#include "Missile.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

BaseNode::BaseNode(const std::string& spriteFile)
    : Cistron::Component("Node"),
      sprite_(NULL),
      body_(NULL)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(BaseNode::UpdateNode)); 

    //---------------------------------------------------------------
    // Sprites
    //---------------------------------------------------------------    
    sprite_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(spriteFile.c_str());
    this->addChild(sprite_);
}

BaseNode::~BaseNode()
{
}

cocos2d::CCRect BaseNode::boundingBox(void) 
{ 
    if (sprite_)
    {
        cocos2d::CCRect region = sprite_->boundingBox();
        return CCRectMake(
            this->getPosition().x + region.origin.x,
            this->getPosition().y + region.origin.y,
            region.size.width,
            region.size.height);
    }
    else
    {
        return cocos2d::CCRectZero; 
    }
} 

const cocos2d::CCSize& BaseNode::getContentSize(void) 
{
    return (sprite_) ? sprite_->getContentSize() : cocos2d::CCSizeZero; 
}

void BaseNode::UpdateNode(float dt)
{    
    if (body_ && sprite_)
    {
        cocos2d::CCPoint position = cocos2d::CCPoint(
            body_->GetPosition().x * PTM_RATIO, 
            body_->GetPosition().y * PTM_RATIO);
        float rotation = CC_RADIANS_TO_DEGREES(-1 * body_->GetAngle());
        this->setRotation(rotation);        
        this->setPosition(position);
    }
}

void BaseNode::Destroy() 
{ 
    if (body_)
    {
        body_->GetWorld()->DestroyBody(body_);
    }

    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);    
    
    // CANNOT destroy object here
    //this->getObjectManager()->destroyObject(this->getOwnerId());
    
}


} } } // namespace
