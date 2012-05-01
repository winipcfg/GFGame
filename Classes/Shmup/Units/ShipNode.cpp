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

#include "ShipNode.h"
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "CCExtensions/CCSpriteHelper.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

//const std::string   kSprite                     = "Assets/Shmup/Ship/ShipCore.png";
const std::string   kSprite                     = "Assets/Shmup/ship_4.png";
//const std::string   kSprite                     = "Assets/Shmup/Sprites.pvr.ccz";
//const std::string   kSpriteShipFrame            = "Assets/Shmup/Sprites.plist";
const short         kAnimationStart             = 1;
const short         kAnimationEnd               = 3;
const short         kNumberOfFramePerSecond     = 10;

//const std::string   kSprite             = "Assets/Test/Dummy/M/dummy.png";
//const std::string   kSpriteShipFrame    = "Assets/Test/Dummy/M/dummy.plist";

ShipNode::ShipNode()
    : sprite_(NULL)
    , body_(NULL)
    , physics_component_(NULL)
{
    this->schedule(schedule_selector(ShipNode::UpdateNode)); 
}

ShipNode::~ShipNode()
{
    CC_SAFE_RELEASE(sprite_);
}

bool ShipNode::init()
{
    //---------------------------------------------------------------
    // Sprites
    //---------------------------------------------------------------
    sprite_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kSprite.c_str());
    this->addChild(sprite_);

    //sprite_ = new AdvanceSprite();
    //sprite_->init();
    //sprite_->addFrames(kSpriteShipFrame.c_str(), kSprite.c_str());
    //sprite_->startAnimation(kAnimationStart, kAnimationEnd, -1, 0, this, kNumberOfFramePerSecond, false, false);
    //sprite_->setIsVisible(true);
    //this->addChild(sprite_);

    return true;
}

cocos2d::CCRect ShipNode::boundingBox(void) 
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

void ShipNode::UpdateNode(cocos2d::ccTime dt)
{    
    //UpdatePositionAndRotation();

    //if (body_ && !body_->IsAwake())
    //{
    //    Destroy();
    //}
}

void ShipNode::UpdatePositionAndRotation()
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

void ShipNode::Destroy() 
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
