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
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "CCExtensions/CCSpriteHelper.h"
#include "Components/Missile.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kUnitSprite         = "Assets/Test/spell.png";

const std::string   kUnitSprite2                        = "Assets/Test/grossini-aliases.png";
const std::string   kUnitSpriteFrame                    = "Assets/Test/grossini-aliases.plist";

MissileNode::MissileNode()
    : Cistron::Component("MissileNode"),
      sprite_(NULL),
      //sprite_new_(NULL),
      body_(NULL)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(MissileNode::UpdateNode)); 
}

MissileNode::~MissileNode()
{
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
    //// Sprites Test
    ////---------------------------------------------------------------
    //sprite_ = new cocos2d::AdvanceSprite();
    //sprite_->init();
    //sprite_->setPosition(ccp(20, 0));
    //this->addChild(sprite_);
    //sprite_->addFrames(kUnitSpriteFrame.c_str(), kUnitSprite2.c_str());
    //sprite_->startAnimation(1, 14, -1, 0, this, 5, false, false);
        
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

        //std::list<Cistron::Component*> missiles = this->getComponents("Missxzcile", this->getOwnerId());
        //Missile* missile = static_cast<Missile*>(missiles.front());
        //int i = missile->GetDamage();
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
    }

    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);    
    
    // CANNOT destroy object here
    //this->getObjectManager()->destroyObject(this->getOwnerId());
    
}


} // namespace
