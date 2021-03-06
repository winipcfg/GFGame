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
#include "../Units/ShipNode.h"
#include "../Units/Asteroid.h"
#include "../Units/Ship.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

#define PLAY_BG_MUSIC           0//1
#define kNumShootsPerSecond     1
#define kTagJoystick            2
    
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
    , is_shooting_(false)
    , last_shoot_time_(0)
    , joystick_(NULL)
    , next_asteroid_(0)
    , ship_(NULL)
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
        
    // Ship
    std::list<Cistron::Component*> list;
    std::list<Cistron::Component*>::iterator it;
    
    ship_ = game_->SpawnPlayerShip(b2Vec2(0, 0));
    //ship_ = game_->SpawnPlayerShip(b2Vec2(winSize.width * 0.25, winSize.height * 0.5));

    AttachRenderComponents(ship_->getOwnerId());
    list = game_->getComponents(ship_->getOwnerId(), "RenderComponent");  
    GFGame::Components::RenderComponent* node = (GFGame::Components::RenderComponent*)(*list.begin());
    ship_node_ = (ShipNode*)node->Node();
    
    // Another Ship
    Ship* ship = game_->SpawnPlayerShip(game_->GetBattlefield()->GetRandomSpawnPosition(), 1);
    AttachRenderComponents(ship->getOwnerId());
    
    //// Particles
    //CCParticleSystemQuad* starsEffect = CCParticleSystemQuad::particleWithFile("Assets/Shmup/Stars1.plist");
    //this->addChild(starsEffect, 1);
        
    // Add joystick
    joystick_ =  HSJoystick::create();
    joystick_->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(joystick_, kTagJoystick, kTagJoystick);
    
    this->setTouchEnabled(true);
    this->setAccelerometerEnabled(true);
    this->scheduleUpdate();
    this->schedule(schedule_selector(ShmupLayer::UpdateNode)); 

#if (PLAY_BG_MUSIC)
    audio_handler_.PlayBackgroundMusic();
#endif
}

ShmupLayer::~ShmupLayer()
{
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

void ShmupLayer::DoFire()
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    CCPoint position = ship_node_->getPosition();
    b2Vec2 pos = b2Vec2(position.x, position.y);

    ObjectId objId = game_->SpawnLaser(pos, 0);
    AttachRenderComponents(objId);

    std::list<Cistron::Component*> list = game_->getComponents(objId, "Unit");
    Ship* obj = ((Ship*)(*list.begin()));
    obj->Move(pos.x, pos.y);
}

void ShmupLayer::UpdateNode(float dt)
{
    game_->Update(dt);

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

            if (ship_)
                ship_->Update(dt);
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
            if (duration_ > next_asteroid_) 
            {
                this->SpawnAsteroid();
                float randSecs = GFort::Core::MathHelper::RandomBetween(1.5f, 2.2f);
                next_asteroid_ = randSecs + duration_;        
            }
        }

        // If number of lives if less than 0, do game over
        if (game_->NumLives() <= 0) 
        {
            CCLOG("[%s][%d] - Game is Over", __FUNCTION__, __LINE__);
            game_->SetGameOver(true);
            if (is_shooting_)
            {
                is_shooting_ = false;    
                last_shoot_time_ = duration_;
            }
        } 
        else
        {
            duration_ += dt;
        }
    }
}

void ShmupLayer::AttachRenderComponents(const Cistron::ObjectId& objId)
{
    std::list<Cistron::Component*>::iterator it;
    std::list<Cistron::Component*> list = game_->getComponents(objId, "RenderComponent");    
    for (it = list.begin(); it != list.end(); ++it)
    {
        GFGame::Components::RenderComponent* node = (GFGame::Components::RenderComponent*)(*it);
        this->addChild(node->Node());
    }
}

void ShmupLayer::SpawnAsteroid()
{
    //cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    //{
    //    Asteroid* obj = game_->SpawnAsteroid(b2Vec2(winSize.width/2, 0), 0);
    //    AttachRenderComponents(obj->getOwnerId());
    //    obj->Move(0, 5);
    //}
    //{
    //    Asteroid* obj = game_->SpawnAsteroid(b2Vec2(winSize.width/2, winSize.height), 0);
    //    AttachRenderComponents(obj->getOwnerId());
    //    obj->Move(0, -5);
    //}
}

//void ccDrawEllipse(cocos2d::CCPoint center, float rx, float ry, float a, int segs, BOOL drawLineToCenter)
//{
//    int additionalSegment = 1;
//    if (drawLineToCenter)
//        additionalSegment++;
//
//    const float coef = 2.0f * (float)M_PI / segs;
//
//    float* vertices = new float[2 * (segs + 2)]; 
//    if(!vertices)
//    {
//        return;
//    }
//    memset(vertices, 0, sizeof(float) * 2 * (segs + 2));
//
//    float rads, distance, angle, j, k;
//    for(int i = 0; i <= segs; ++i)
//    {
//        rads = i * coef;
//        distance = sqrt(pow(sinf(rads) * rx, 2) + pow(cosf(rads) * ry, 2));
//        angle = atan2(sinf(rads) * rx, cosf(rads) * ry);
//        j = distance * cosf(angle + a) + center.x;
//        k = distance * sinf(angle + a) + center.y;
//
//        vertices[i*2] = j * CC_CONTENT_SCALE_FACTOR();
//        vertices[i*2+1] = k * CC_CONTENT_SCALE_FACTOR();
//    }
//    vertices[(segs+1)*2] = center.x * CC_CONTENT_SCALE_FACTOR();
//    vertices[(segs+1)*2+1] = center.y * CC_CONTENT_SCALE_FACTOR();
//
//    // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
//    // Needed states: GL_VERTEX_ARRAY, 
//    // Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY, GL_COLOR_ARRAY	
//    glDisable(GL_TEXTURE_2D);
//    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//    glDisableClientState(GL_COLOR_ARRAY);
//
//    glVertexPointer(2, GL_FLOAT, 0, vertices);	
//    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) segs+additionalSegment);
//
//    // restore default state
//    glEnableClientState(GL_COLOR_ARRAY);
//    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//    glEnable(GL_TEXTURE_2D);	
//
//    delete[] vertices;
//}

void ShmupLayer::draw(void)
{
    //cocos2d::ccDrawCircle(ship_node_->getPosition(), 50, ship_node_->Body()->GetAngle() + M_PI/2, 20, true);

    //ccDrawEllipse(ship_node_->getPosition(), 20, 100, ship_node_->Body()->GetAngle() + M_PI/2, 20, true);
    
}

} } } // namespace
