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

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

#define PLAY_BG_MUSIC           0//1
#define kNumAsteroids           15
#define kNumLasers              5
#define kNumLives               3
#define kNumShootsPerSecond     0.2

#define kSpriteBatchNode        "Assets/Shmup/Sprites.pvr.ccz"
#define kSpriteFrames           "Assets/Shmup/Sprites.plist"
#define kSpriteShip             "SpaceFlier_sm_1.png"
#define kSpriteLaser            "laserbeam_blue.png"

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
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    // Setup graphics
    batchNode_ = cocos2d::CCSpriteBatchNode::batchNodeWithFile(kSpriteBatchNode); 
    this->addChild(batchNode_);
    cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(kSpriteFrames);
        
    shipSprite_ = cocos2d::CCSprite::spriteWithSpriteFrameName(kSpriteShip);
    shipSprite_->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    batchNode_->addChild(shipSprite_, 1);
    
    //// 1) Create the CCParallaxNode
    //_backgroundNode = [CCParallaxNode node];
    //[self addChild:_backgroundNode z:-1];
    //
    //// 2) Create the sprites we'll add to the CCParallaxNode
    //_spacedust1 = [CCSprite spriteWithFile:@"bg_front_spacedust.png"];
    //_spacedust2 = [CCSprite spriteWithFile:@"bg_front_spacedust.png"];
    //_planetsunrise = [CCSprite spriteWithFile:@"bg_planetsunrise.png"];
    //_galaxy = [CCSprite spriteWithFile:@"bg_galaxy.png"];
    //_spacialanomaly = [CCSprite spriteWithFile:@"bg_spacialanomaly.png"];
    //_spacialanomaly2 = [CCSprite spriteWithFile:@"bg_spacialanomaly2.png"];


    // 3) Determine relative movement speeds for space dust and background
    cocos2d::CCPoint dustSpeed = ccp(0.1, 0.1);
    cocos2d::CCPoint bgSpeed = ccp(0.05, 0.05);
                     
    //// 4) Add children to CCParallaxNode
    //[_backgroundNode addChild:_spacedust1 z:0 parallaxRatio:dustSpeed positionOffset:ccp(0,winSize.height/2)];
    //[_backgroundNode addChild:_spacedust2 z:0 parallaxRatio:dustSpeed positionOffset:ccp(_spacedust1.contentSize.width,winSize.height/2)];        
    //[_backgroundNode addChild:_galaxy z:-1 parallaxRatio:bgSpeed positionOffset:ccp(0,winSize.height * 0.7)];
    //[_backgroundNode addChild:_planetsunrise z:-1 parallaxRatio:bgSpeed positionOffset:ccp(600,winSize.height * 0)];        
    //[_backgroundNode addChild:_spacialanomaly z:-1 parallaxRatio:bgSpeed positionOffset:ccp(900,winSize.height * 0.3)];        
    //[_backgroundNode addChild:_spacialanomaly2 z:-1 parallaxRatio:bgSpeed positionOffset:ccp(1500,winSize.height * 0.9)];
    //
    //NSArray *starsArray = [NSArray arrayWithObjects:@"Stars1.plist", @"Stars2.plist", @"Stars3.plist", nil];
    //for(NSString *stars in starsArray) {        
    //    CCParticleSystemQuad *starsEffect = [CCParticleSystemQuad particleWithFile:stars];        
    //    [self addChild:starsEffect z:1];
    //}
    //_asteroids = [[CCArray alloc] initWithCapacity:kNumAsteroids];
    //for(int i = 0; i < kNumAsteroids; ++i) {
    //    CCSprite *asteroid = [CCSprite spriteWithSpriteFrameName:@"asteroid.png"];
    //    asteroid.visible = NO;
    //    [batchNode_ addChild:asteroid];
    //    [_asteroids addObject:asteroid];
    //}

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
 

    //game_->GetShip(0).AddWeapon(myWeapon_);    
    //game_->GetShip(0).SetWeapon(0);

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
    CC_SAFE_RELEASE(render_texture_)
}

bool ShmupLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    touch_start_position_ = touch_end_position_ = touchLocation;
    //battle_.MouseDown(b2Vec2(touchLocation.x, touchLocation.y));    

    is_shooting_ = true;
    last_shoot_time_ = duration_;

    return true;
}

void ShmupLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    Point oldTouchLocation = pTouch->previousLocationInView(pTouch->view());
    oldTouchLocation = director->convertToGL(oldTouchLocation);
    oldTouchLocation = convertToNodeSpace(oldTouchLocation);

    //battle_.MouseMove(b2Vec2(touchLocation.x, touchLocation.y));
    touch_end_position_ = touchLocation;    
}

void ShmupLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    is_shooting_ = false;
    last_shoot_time_ = duration_;
    touch_start_position_ = touch_end_position_ = cocos2d::CCPointZero;    
}


void CreateBullet(b2Vec2 start)
{	
	//float32 radius = 10.0;
	//
 //   b2World* world = (b2World*) controller_.World();
	//b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateCircle(
 //       world, 
	//	start,
	//	radius);	
 //   body->SetLinearVelocity(b2Vec2(10.0, 0));
 //   
 //   // Set the dynamic body fixture.
	//b2Fixture* fixture = body->GetFixtureList();	
	//fixture[0].SetDensity(1.0f);
	//fixture[0].SetFriction(0.3f);
	//fixture[0].SetRestitution(0.4f);	
	//body->ResetMassData();
}


void ShmupLayer::DoFire()
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    //    GFort::Games::Shmup::Ship& ship = game_.GetShip(0);
    //ship.DoAttack();
    //
    //[[SimpleAudioEngine sharedEngine] playEffect:@"laser_ship.caf"];
    //
    //
    CCSprite *shipLaser = static_cast<CCSprite*>(_shipLasers[_nextShipLaser]);
    _nextShipLaser = (_nextShipLaser + 1) % _shipLasers.size();

    
    CCPoint position = ccpAdd(shipSprite_->getPosition(), ccp(shipLaser->getContentSize().width/2, 0));
    shipLaser->setPosition(position);
    shipLaser->setIsVisible(true);
    shipLaser->stopAllActions();
    shipLaser->runAction(cocos2d::CCSequence::actions(
        CCMoveBy::actionWithDuration(0.5, ccp(winSize.width, 0)),
        //CCCallFuncN::actionWithTarget(this, ShmupLayer::setInvisible))
        NULL));

    //b2Vec2 pos = b2Vec2(position.x, position.y);
    //[self CreateBullet:pos];
}

void ShmupLayer::UpdateNode(cocos2d::ccTime dt)
{
    if (!game_->GameOver())
    {
        // If number of lives if larger than 0
        if (game_->NumLives() > 0) 
        {
            cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
            float maxY = winSize.height - shipSprite_->getContentSize().height/2;
            float minY = shipSprite_->getContentSize().height/2;
        
            float newY = shipSprite_->getPosition().y + (_shipPointsPerSecY * dt);
            newY = MIN(MAX(newY, minY), maxY);
            shipSprite_->setPosition(ccp(shipSprite_->getPosition().x, newY));

            if (is_shooting_)
            {
                while ((duration_ - last_shoot_time_) > kNumShootsPerSecond) 
                {
                    this->DoFire();
                    last_shoot_time_ += kNumShootsPerSecond;
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

 //   // Instruct the world to perform a single step of simulation. It is
	//// generally best to keep the time step and iterations fixed.
	//controller_.Step(&phys_settings_, dt);
 //   
 //   CGPoint backgroundScrollVel = ccp(-1000, 0);
 //   _backgroundNode.position = ccpAdd(_backgroundNode.position, ccpMult(backgroundScrollVel, dt));
 //   
 //   NSArray *spaceDusts = [NSArray arrayWithObjects:_spacedust1, _spacedust2, nil];
 //   for (CCSprite *spaceDust in spaceDusts) {
 //       if ([_backgroundNode convertToWorldSpace:spaceDust.position].x < -spaceDust.contentSize.width) {
 //           [_backgroundNode incrementOffset:ccp(2*spaceDust.contentSize.width,0) forChild:spaceDust];
 //       }
 //   }
 //   
 //   NSArray *backgrounds = [NSArray arrayWithObjects:_planetsunrise, _galaxy, _spacialanomaly, _spacialanomaly2, nil];
 //   for (CCSprite *background in backgrounds) {
 //       if ([_backgroundNode convertToWorldSpace:background.position].x < -background.contentSize.width) {
 //           [_backgroundNode incrementOffset:ccp(2000,0) forChild:background];
 //       }
 //   }
 //   
 //   double curTime = CACurrentMediaTime();
 //   if (_lives > 0) 
 //   { 
 //       CGSize winSize = [CCDirector sharedDirector].winSize;
 //       float maxY = winSize.height - shipSprite_.contentSize.height/2;
 //       float minY = shipSprite_.contentSize.height/2;
 //       
 //       float newY = shipSprite_.position.y + (_shipPointsPerSecY * dt);
 //       newY = MIN(MAX(newY, minY), maxY);
 //       shipSprite_.position = ccp(shipSprite_.position.x, newY);
 //               
 //       if (curTime > _nextAsteroidSpawn) 
 //       {
 //           
 //           [self spawnAsteroid];        
 //           float randSecs = [self randomValueBetween:0.20 andValue:1.0];
 //           _nextAsteroidSpawn = randSecs + curTime;        
 //       }
 //   
 //      

 //       
 //       for (CCSprite *asteroid in _asteroids) 
 //       {
 //           if (!asteroid.visible) continue;
 //           
 //           for (CCSprite *shipLaser in _shipLasers) 
 //           {
 //               if (!shipLaser.visible) continue;
 //               
 //               if (CGRectIntersectsRect(shipLaser.boundingBox, asteroid.boundingBox)) 
 //               {
 //                   [[SimpleAudioEngine sharedEngine] playEffect:@"explosion_large.caf"];
 //                   shipLaser.visible = NO;
 //                   asteroid.visible = NO;                
 //                   continue;
 //               }
 //           }
 //           
 //           if (CGRectIntersectsRect(shipSprite_.boundingBox, asteroid.boundingBox)) 
 //           {
 //               [[SimpleAudioEngine sharedEngine] playEffect:@"explosion_large.caf"];
 //               asteroid.visible = NO;
 //               [shipSprite_ runAction:[CCBlink actionWithDuration:1.0 blinks:9]];            
 //               _lives--;
 //           }
 //       }
 //   }
}

} } } // namespace

/*
// Import the interfaces
#import "ShmupLayer.h"
#import "CCParallaxNode-Extras.h"
#import "SimpleAudioEngine.h"
#import "AccelerometerSimulation.h"

#include <GFort/Core/Physics/PhysicsHelper.h>
#include <GFort/Games/Shmup/Weapon.h>

// HelloWorldLayer implementation
@implementation ShmupLayer

GFort::Games::Shmup::WeaponA myWeapon_;


- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
    
#define kFilteringFactor 0.1
#define kRestAccelX -0.6
#define kShipMaxPointsPerSec (winSize.height*0.5)        
#define kMaxDiffX 0.2
    
    UIAccelerationValue rollingX, rollingY, rollingZ;
    
    rollingX = (acceleration.x * kFilteringFactor) + (rollingX * (1.0 - kFilteringFactor));    
    rollingY = (acceleration.y * kFilteringFactor) + (rollingY * (1.0 - kFilteringFactor));    
    rollingZ = (acceleration.z * kFilteringFactor) + (rollingZ * (1.0 - kFilteringFactor));
    
    float accelX = acceleration.x - rollingX;
    //float accelY = acceleration.y - rollingY;
    //float accelZ = acceleration.z - rollingZ;
    
    CGSize winSize = [CCDirector sharedDirector].winSize;
    
    float accelDiff = accelX - kRestAccelX;
    float accelFraction = accelDiff / kMaxDiffX;
    float pointsPerSec = kShipMaxPointsPerSec * accelFraction;
    
    _shipPointsPerSecY = pointsPerSec;
    
}

- (float)randomValueBetween:(float)low andValue:(float)high {
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

- (void)restartTapped:(id)sender {
    [[CCDirector sharedDirector] replaceScene:[CCTransitionZoomFlipX transitionWithDuration:0.5 
                                                                                      scene:[ShmupLayer scene]]];   
}

- (void)endScene:(EndReason)endReason 
{    
    if (!game_.GameOver())
    {
        game_.SetGameOver(true);
        
        CGSize winSize = [CCDirector sharedDirector].winSize;
        
        NSString *message;
        if (endReason == kEndReasonWin) {
            message = @"You win!";
        } else if (endReason == kEndReasonLose) {
            message = @"You lose!";
        }
        
        CCLabelBMFont *label;
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            label = [CCLabelBMFont labelWithString:message fntFile:@"Arial-hd.fnt"];
        } else {
            label = [CCLabelBMFont labelWithString:message fntFile:@"Arial.fnt"];
        }
        label.scale = 0.1;
        label.position = ccp(winSize.width/2, winSize.height * 0.6);
        [self addChild:label];
        
        CCLabelBMFont *restartLabel;
        if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
            restartLabel = [CCLabelBMFont labelWithString:@"Restart" fntFile:@"Arial-hd.fnt"];    
        } else {
            restartLabel = [CCLabelBMFont labelWithString:@"Restart" fntFile:@"Arial.fnt"];    
        }
        
        CCMenuItemLabel *restartItem = [CCMenuItemLabel itemWithLabel:restartLabel target:self selector:@selector(restartTapped:)];
        restartItem.scale = 0.1;
        restartItem.position = ccp(winSize.width/2, winSize.height * 0.4);
        
        CCMenu *menu = [CCMenu menuWithItems:restartItem, nil];
        menu.position = CGPointZero;
        [self addChild:menu];
        
        [restartItem runAction:[CCScaleTo actionWithDuration:0.5 scale:1.0]];
        [label runAction:[CCScaleTo actionWithDuration:0.5 scale:1.0]];
    }
}

- (void)spawnAsteroid
{
    CGSize winSize = [CCDirector sharedDirector].winSize;
    
    float randY = [self randomValueBetween:0.0 andValue:winSize.height];
    float randDuration = [self randomValueBetween:2.0 andValue:10.0];
    
    CCSprite *asteroid = [_asteroids objectAtIndex:_nextAsteroid];
    _nextAsteroid++;
    if (_nextAsteroid >= _asteroids.count) _nextAsteroid = 0;
    
    [asteroid stopAllActions];    
    asteroid.position = ccp(winSize.width+asteroid.contentSize.width/2, randY);
    asteroid.visible = YES;
    asteroid.scale = [self randomValueBetween:0.2 andValue:1.0];
    [asteroid runAction:[CCSequence actions:
                         [CCMoveBy actionWithDuration:randDuration 
                                             position:ccp(-winSize.width-asteroid.contentSize.width, 0)],
                         [CCCallFuncN actionWithTarget:self selector:@selector(setInvisible:)],
                         nil]];
}


*/