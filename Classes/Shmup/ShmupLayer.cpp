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

#include "ShmupLayer.h"
#include <CCExtensions/CCSpriteHelper.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <GFort/Core/MathHelper.h>
#include "Units/ShipNode.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

#define PLAY_BG_MUSIC           0//1
#define kNumAsteroids           20
#define kNumLasers              5
#define kNumLives               3
#define kNumShootsPerSecond     0.2

#define kSpriteBatchNode        "Assets/Shmup/Sprites.pvr.ccz"
#define kSpriteFrames           "Assets/Shmup/Sprites.plist"
#define kSpriteShip             "SpaceFlier_sm_1.png"
#define kSpriteLaser            "laserbeam_blue.png"
#define kSpriteDust1            "Assets/Shmup/bg_front_spacedust.png"
#define kSpriteDust2            "Assets/Shmup/bg_front_spacedust.png"
#define kSpriteSunrise          "Assets/Shmup/bg_planetsunrise.png"
#define kSpriteGalaxy           "Assets/Shmup/bg_galaxy.png"
#define kSpriteSpacialAnomaly   "Assets/Shmup/bg_spacialanomaly.png"
#define kSpriteSpacialAnomaly2  "Assets/Shmup/bg_spacialanomaly2.png"

const std::string   kSprite             = "Assets/Shmup/Sprites.pvr.ccz";
const std::string   kSpriteShipFrame    = "Assets/Shmup/Sprites.plist";

enum 
{
    kTagParallax    = -5,
    kTagBackground  = -1,
    kTagJoystick    = 2,
};

typedef enum {
    kEndReasonWin,
    kEndReasonLose
} EndReason;

ShmupLayer::ShmupLayer(GFort::Games::Shmup::Game* game)
    : Cistron::Component("CCLayer")
    , duration_(0)
    , touch_start_position_(cocos2d::CCPointZero)
    , touch_end_position_(cocos2d::CCPointZero)
    , game_(game)
    , is_shooting_(true)
    , last_shoot_time_(0)
    , _nextShipLaser(0)
    , _shipLasers(NULL)
    , joystick_(NULL)
    , _nextAsteroid(0)
    , _nextAsteroidSpawn(0)
    , ship_(NULL)
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        
    // Ship
    Cistron::ObjectId objId = game_->SpawnPlayerShip(b2Vec2(winSize.width * 0.5, winSize.height * 0.5));
    std::list<Cistron::Component*> list = game_->getComponents(objId, "RenderComponent");
    std::list<Cistron::Component*>::iterator it;
    for (it = list.begin(); it != list.end(); ++it)
    {
        GFGame::Components::RenderComponent* node = (GFGame::Components::RenderComponent*)(*it);
        ship_node_ = (ShipNode*)node->Node();
        this->addChild(node->Node());
    }
    list = game_->getComponents(objId, "Ship");
    for (it = list.begin(); it != list.end(); ++it)
    {
        ship_ = (Ship*)(*it);
        break;
    }
    

    // Create the CCParallaxNode
    _backgroundNode = CCParallaxNode::node();
    this->addChild(_backgroundNode, -1);

    _spacedust1 = cocos2d::CCSprite::spriteWithFile(kSpriteDust1);
    _spacedust2 = cocos2d::CCSprite::spriteWithFile(kSpriteDust2);
    _planetsunrise = cocos2d::CCSprite::spriteWithFile(kSpriteSunrise);
    _galaxy = cocos2d::CCSprite::spriteWithFile(kSpriteGalaxy);
    _spacialanomaly = cocos2d::CCSprite::spriteWithFile(kSpriteSpacialAnomaly);
    _spacialanomaly2 = cocos2d::CCSprite::spriteWithFile(kSpriteSpacialAnomaly2);
    
    // 3) Determine relative movement speeds for space dust and background
    cocos2d::CCPoint dustSpeed = ccp(0.1, 0.1);
    cocos2d::CCPoint bgSpeed = ccp(0.05, 0.05);
                     
    //// 4) Add children to CCParallaxNode
    _backgroundNode->addChild(_spacedust1, 0, dustSpeed, ccp(0,winSize.height/2));
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(_spacedust1->getContentSize().width,winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0,winSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, ccp(600,winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900,winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500,winSize.height * 0.9));

    // Particles
    CCParticleSystemQuad* starsEffect = CCParticleSystemQuad::particleWithFile("Assets/Shmup/Stars1.plist");
    this->addChild(starsEffect, 1);
    starsEffect = CCParticleSystemQuad::particleWithFile("Assets/Shmup/Stars2.plist");
    this->addChild(starsEffect, 1);
    starsEffect = CCParticleSystemQuad::particleWithFile("Assets/Shmup/Stars3.plist");
    this->addChild(starsEffect, 1);

    // Asteroids
    batchNode_ = cocos2d::CCSpriteBatchNode::batchNodeWithFile(kSpriteBatchNode); 
    this->addChild(batchNode_);
    for(int i = 0; i < kNumAsteroids; ++i) 
    {
        CCSprite *asteroid = CCSprite::spriteWithSpriteFrameName("asteroid.png");
        asteroid->setIsVisible(false);
        batchNode_->addChild(asteroid);
        _asteroids.push_back(asteroid);
    }

    for(int i = 0; i < kNumLasers; ++i) 
    {
        CCSprite *shipLaser = cocos2d::CCSprite::spriteWithSpriteFrameName(kSpriteLaser);
        shipLaser->setIsVisible(false);
        batchNode_->addChild(shipLaser);
        _shipLasers.push_back(shipLaser);
    }

    // Render Texture
    render_texture_ = cocos2d::CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
    render_texture_->retain();
    render_texture_->autorelease();
    render_texture_->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(render_texture_); 
    
    // Add joystick
    joystick_ =  HSJoystick::node();
    joystick_->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(joystick_, kTagJoystick, kTagJoystick);

    this->setIsTouchEnabled(true);
    this->setIsAccelerometerEnabled(true);
    this->scheduleUpdate();
    this->schedule(schedule_selector(ShmupLayer::UpdateNode)); 

#if (PLAY_BG_MUSIC)
    audio_handler_.PlayBackgroundMusic();
#endif
}

ShmupLayer::~ShmupLayer()
{
    CC_SAFE_RELEASE(render_texture_);
}

void ShmupLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    // Handle first touch
    Point touchLocation = convertTouchToNodeSpace(static_cast<cocos2d::CCTouch*>(pTouches->anyObject()));
    touch_start_position_ = touch_end_position_ = touchLocation;
    //battle_.MouseDown(b2Vec2(touchLocation.x, touchLocation.y));    
    
    //is_shooting_ = true;
    last_shoot_time_ = duration_;
}

void ShmupLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}

void ShmupLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(static_cast<cocos2d::CCTouch*>(pTouches->anyObject()));
    //is_shooting_ = false;
    last_shoot_time_ = duration_;
    touch_start_position_ = touch_end_position_ = cocos2d::CCPointZero; 
}

void ShmupLayer::CreateBullet(b2Vec2 start)
{	
	float32 radius = 10.0;
	
    b2World* world = (b2World*) game_->World();
	b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateCircle(world, b2_dynamicBody, start, radius);	
    body->SetLinearVelocity(b2Vec2(10.0, 0));
    
    // Set the dynamic body fixture.
	b2Fixture* fixture = body->GetFixtureList();	
	fixture[0].SetDensity(1.0f);
	fixture[0].SetFriction(0.3f);
	fixture[0].SetRestitution(0.4f);	
	body->ResetMassData();
}


void ShmupLayer::DoFire()
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    CCSprite *shipLaser = static_cast<CCSprite*>(_shipLasers[_nextShipLaser]);
    _nextShipLaser = (_nextShipLaser + 1) % _shipLasers.size();

    
    CCPoint position = ccpAdd(ship_node_->getPosition(), ccp(shipLaser->getContentSize().width/2, 0));
    shipLaser->setPosition(position);
    shipLaser->setIsVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(cocos2d::CCSequence::actions(
        CCMoveBy::actionWithDuration(0.5, ccp(winSize.width, 0)),
        //CCCallFuncN::actionWithTarget(this, ShmupLayer::setInvisible))
        NULL));

    //b2Vec2 pos = b2Vec2(position.x, position.y);
    //this->CreateBullet(pos);

    //// Laser
    //Cistron::ObjectId objId = game_->SpawnLaser(b2Vec2(position.x, position.y), 0);
    //std::list<Cistron::Component*> list = game_->getComponents(objId, "RenderComponent");
    //std::list<Cistron::Component*>::iterator it;
    //for (it = list.begin(); it != list.end(); ++it)
    //{
    //    GFGame::Components::RenderComponent* node = (GFGame::Components::RenderComponent*)(*it);
    //    this->addChild(node->Node());
    //}
}

void ShmupLayer::UpdateNode(cocos2d::ccTime dt)
{
    game_->Update(dt);

    cocos2d::CCPoint backgroundScrollVel(ccp(-1000, 0));
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVel, dt)));

    //std::vector<cocos2d::CCSprite*> spaceDusts;
    //spaceDusts.push_back(_spacedust1);
    //spaceDusts.push_back(_spacedust2);
    //std::vector<cocos2d::CCSprite*>::iterator it;
    //for (it = spaceDusts.begin(); it != spaceDusts.end(); ++it)
    //{
    //    if (_backgroundNode->convertToWorldSpace((*it)->getPosition()).x < (*it)->getContentSize().width)
    //    {
    //        (*it)->setPosition(ccpAdd((*it)->getPosition(), ccp(2 * (*it)->getContentSize().width, 0)));
    //    }
    //} 

    //std::vector<cocos2d::CCSprite*> backgrounds;
    //backgrounds.push_back(_planetsunrise);
    //backgrounds.push_back(_galaxy);
    //backgrounds.push_back(_spacialanomaly);
    //backgrounds.push_back(_spacialanomaly2);
    //for (it = backgrounds.begin(); it != backgrounds.end(); ++it)
    //{
    //    if (_backgroundNode->convertToWorldSpace((*it)->getPosition()).x < -(*it)->getContentSize().width)
    //    {
    //        (*it)->setPosition(ccpAdd((*it)->getPosition(), ccp(2000, 0)));
    //    }
    //} 
    
    if (!game_->GameOver())
    {
        // If number of lives if larger than 0
        if (game_->NumLives() > 0) 
        {            
            if (is_shooting_)
            {
                while ((duration_ - last_shoot_time_) > kNumShootsPerSecond) 
                {
                    this->DoFire();
                    last_shoot_time_ += kNumShootsPerSecond;
                }        
            }

            if (ship_ && joystick_ && !cocos2d::CCPoint::CCPointEqualToPoint(joystick_->getVelocity(), cocos2d::CCPointZero))
            {
                float speed = 3.0f;
                ship_->Move(joystick_->getVelocity().x * speed, joystick_->getVelocity().y * speed);
            }
            else
            {
                ship_->StopMove();
            }

            // Asteroids
            if (duration_ > _nextAsteroidSpawn) 
            {
                this->SpawnAsteroid();
                float randSecs = GFort::Core::MathHelper::RandomBetween(0.1f, 0.8f);
                _nextAsteroidSpawn = randSecs + duration_;        
            }

            std::vector<cocos2d::CCSprite*>::iterator it;
            std::vector<cocos2d::CCSprite*>::iterator lit;
            for (it = _asteroids.begin(); it != _asteroids.end(); ++it)
            {
                if (!(*it)->getIsVisible()) 
                    continue;

                // If laser hits asteroid, destroy both of them
                for (lit = _shipLasers.begin(); lit != _shipLasers.end(); ++lit)
                {
                    if (!(*lit)->getIsVisible()) 
                        continue;

                    if (CCRect::CCRectIntersectsRect((*it)->boundingBox(), (*lit)->boundingBox())) 
                    {
                        (*it)->setIsVisible(false);
                        (*lit)->setIsVisible(false);
                        continue;
                    }
                }

                if (CCRect::CCRectIntersectsRect((*it)->boundingBox(), ship_node_->boundingBox())) 
                {
                    (*it)->setIsVisible(false);
                    ship_node_->runAction(CCBlink::actionWithDuration(1.0, 9));
                    game_->SetNumLives(game_->NumLives() - 1);
                    continue;
                }
            }
        }

        // If number of lives if less than 0, do game over
        if (game_->NumLives() <= 0) 
        {
            game_->SetGameOver(true);
            if (is_shooting_)
            {
                is_shooting_ = false;    
                last_shoot_time_ = duration_;
            }
            //[shipSprite_ stopAllActions];
            //shipSprite_.visible = FALSE;
            //[self endScene:kEndReasonLose];
        } 
        else
        {
            duration_ += dt;
        }
    }
}

void ShmupLayer::SpawnAsteroid()
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    float randY = GFort::Core::MathHelper::RandomBetween(0.0f, winSize.height);
    float randDuration = GFort::Core::MathHelper::RandomBetween(2.0f, 10.0f);
    
    CCSprite *asteroid = _asteroids[_nextAsteroid];
    _nextAsteroid = (_nextAsteroid + 1) % _asteroids.size();
    
    float scale = GFort::Core::MathHelper::RandomBetween(0.4f, 0.5f);
    asteroid->setScale(scale);
    asteroid->setIsVisible(true);

    float x, y, dx, dy;
    dx = 0;
    if (GFort::Core::MathHelper::RandomDouble<float>() > 0.5f)
    {
        x = winSize.width * 0.5f - asteroid->getContentSize().width * scale * 0.5;
        y = 0 - asteroid->getContentSize().height;
        dy = winSize.height + asteroid->getContentSize().height * 2;
    }
    else
    {        
        x = winSize.width * 0.5f + asteroid->getContentSize().width * scale * 0.5;
        y = winSize.height + asteroid->getContentSize().height;
        dy = -(winSize.height + asteroid->getContentSize().height * 2);
    }
    asteroid->setPosition(ccp(x, y));
    asteroid->stopAllActions();
    asteroid->runAction(CCSequence::actions(
        CCMoveBy::actionWithDuration(randDuration, ccp(dx, dy)),
        NULL));
}

} } } // namespace
