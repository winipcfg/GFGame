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

#include "MissileNode.h"
#include <GFort/Core/Macros.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <Warrior/Units/Missile.h>
#include "CCExtensions/CCSpriteHelper.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kUnitSprite         = "Assets/Test/spell.png";

const std::string   kSprite             = "Assets/Shmup/Sprites.pvr.ccz";
const std::string   kSpriteShipFrame    = "Assets/Shmup/Sprites.plist";

MissileNode::MissileNode()
    : Cistron::Component("MissileNode"),
      sprite_(NULL),
      body_(NULL)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(MissileNode::UpdateNode)); 
}

MissileNode::~MissileNode()
{
    //SAFE_DELETE(sprite_);
}

bool MissileNode::init()
{
    //cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    //cache->addSpriteFramesWithFile(kEnemySpriteFrame.c_str());

    // Create a sprite sheet with image
    //cocos2d::CCSpriteBatchNode* spriteSheet = cocos2d::CCSpriteBatchNode::batchNodeWithFile(kEnemySprite.c_str());
    //addChild(spriteSheet);

    //---------------------------------------------------------------
    // Sprites
    //---------------------------------------------------------------
    char frameName[100] = {0};
    sprite_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kUnitSprite.c_str());
    this->addChild(sprite_);

    ////---------------------------------------------------------------
    //// Sprites Test - Cannot use AdvanceSprite due to bounding box issue
    ////---------------------------------------------------------------
    //sprite_ = new AdvanceSprite();
    //sprite_->init();
    //sprite_->addFrames(kSpriteShipFrame.c_str(), kSprite.c_str());
    //sprite_->startAnimation(1, 3, -1, 0, this, 10, false, false);
    //sprite_->setIsVisible(true);
    //this->addChild(sprite_);
        
    return true;
}

cocos2d::CCRect MissileNode::boundingBox(void) 
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

void MissileNode::UpdateNode(cocos2d::ccTime dt)
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

    if (body_ && !body_->IsAwake())
    {
        Destroy();
    }
}

void MissileNode::Destroy() 
{ 
    if (body_)
    {
        body_->GetWorld()->DestroyBody(body_);
        body_ = NULL;
    }

    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);    
    sprite_ = NULL;
    
    // CANNOT destroy object here
    //this->getObjectManager()->destroyObject(this->getOwnerId());
}


} // namespace
