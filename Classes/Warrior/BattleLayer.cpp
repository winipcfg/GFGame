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

#include "BattleLayer.h"

#include <set>
#include <GFort/Core/MathHelper.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <Warrior/Model/Struct.h>
#include <Warrior/Model/BattleHelper.h>
#include "Components/CCNodeComponent.h"
#include "Components/Missile.h"
#include "Components/PhysicsComponent.h"
#include "Components/Bot.h"
#include "Units/ArcherBot.h"
#include "Units/WarriorBot.h"
#include "TypeDef.h"
#include "Scene/SceneHelper.h"

using namespace cocos2d;

namespace Warrior 
{

const float kFallSpeed                  = 0.5f;
const int kMinSliceLength               = 20;
const int kNumEnemies                   = 2;//20;
const float kMinSpawnTime               = 0.5f;
const float kMaxSpawnTime               = 1.5f;

const float kCameraChasingDuration      = 0.8f;

#define PLAY_BG_MUSIC                   0//1
#define SPAWN_ONE_SIDE                  0

enum 
{
    kTagParallax    = -5,
    kTagBackground  = -1,
	kTagTileMap     = 1,    
	kTagBatchNode   = 3,
	kTagAnimation1  = 4,
    kTagEmitter     = -10,
    kTagSliceLayer  = 0,
};

BattleLayer::BattleLayer()
    : Cistron::Component("CCLayer"),
      next_enemy_(0),
      next_enemy_spawn_(0),
      duration_(0),
      hud_layer_(NULL),
      slice_layer_(NULL),
      landscape_(NULL)
{
    SetupGame();
    
    audio_handler_.Initialize();
    emitter_handler_.Initialize();

    scheduleUpdate();

    cocos2d::CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(
        this, 
        0, 
        false);

    schedule(schedule_selector(BattleLayer::UpdateNode)); 
        
#if (PLAY_BG_MUSIC)
    audio_handler_.PlayBackgroundMusic();
#endif
}

BattleLayer::~BattleLayer()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
    this->setIsTouchEnabled(false);
    this->removeAllChildrenWithCleanup(true);
    cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(hud_layer_);
    cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(slice_layer_);
}

void BattleLayer::SetViewer(BattleHUD* viewer)
{
    hud_layer_ = viewer;
}

bool BattleLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    touch_start_position_ = touch_end_position_ = touchLocation;
    battle_.MouseDown(b2Vec2(touchLocation.x, touchLocation.y));    
    return true;
}

void BattleLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    Point touchLocation = convertTouchToNodeSpace(pTouch);
    Point oldTouchLocation = pTouch->previousLocationInView(pTouch->view());
    oldTouchLocation = director->convertToGL(oldTouchLocation);
    oldTouchLocation = convertToNodeSpace(oldTouchLocation);

    battle_.MouseMove(b2Vec2(touchLocation.x, touchLocation.y));
    touch_end_position_ = touchLocation;    
}

void BattleLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(pTouch);

    float d = slice_layer_->SliceLength();
    if (d <= kMinSliceLength)
    {
        CCLOG("[%s][%d] - Touch Ended: Not an attack", __FUNCTION__, __LINE__);
        
        UnitAction action(kUnitActionTypeWalk,
                          touch_start_position_.x,
                          touch_start_position_.y,
                          true,
                          NULL);
        player_->IssueCommand(action, true);

        UpdateCamera(touch_start_position_);
    }
    else
    {
        if (player_->CanPerformAttack())
        {
            CCLOG("[%s][%d] - Touch Ended: Is an attack", __FUNCTION__, __LINE__);

            Point sliceStartPoint = slice_layer_->GetStartPosition();
            sliceStartPoint.y = player_node_->getPosition().y;
            Point sliceEndPoint = slice_layer_->GetEndPosition();
            if (sliceEndPoint.y < battle_.Map().Landscape())
                sliceEndPoint.y = player_node_->getPosition().y;

            UnitAction action(kUnitActionTypeRun,
                              sliceStartPoint.x,
                              sliceStartPoint.y,
                              true,
                              NULL);
            player_->IssueCommand(action, true);

            action.Reset();
            action.ActionType = kUnitActionTypeAttack;
            action.PositionX = sliceEndPoint.x;
            action.PositionY = sliceEndPoint.y;
            action.Interruptible = false;
            player_->IssueCommand(action, false);
        }
    }
    //this->ShootMissile(player_node_->getPosition(), touchLocation);
    //this->ShootMissile(touchLocation, player_node_->getPosition());

    battle_.MouseUp(b2Vec2(touchLocation.x, touchLocation.y));
    touch_start_position_ = touch_end_position_ = cocos2d::CCPointZero;    
}

void BattleLayer::SetupGame()
{
    // Landscape    
    landscape_ = Landscape::node();
    this->addChild(landscape_);

    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

    slice_layer_ = SliceLayer::node();
    addChild(slice_layer_, kTagSliceLayer, kTagSliceLayer);
    
    cocos2d::CCSize mapSize = cocos2d::CCSize(battle_.Map().Width(), battle_.Map().Height());
        
    Point p = ccp(winSize.width/2, battle_.Map().Landscape());
    SpawnPlayer(p);
}

void BattleLayer::SpawnPlayer(const Point& position)
{
    CCLOG("[%s][%d] - Add sprite %0.2f x %02.f", __FUNCTION__, __LINE__, position.x, position.y);
    
    Cistron::ObjectId objId = battle_.SpawnEntity();
    player_ = new Player();
    battle_.addComponent(objId, player_);

    player_node_ = new PlayerNode(player_);  
    player_node_->ResetState();
    player_node_->init();
    player_node_->setPosition(position);    
    battle_.addComponent(objId, player_node_);
    this->addChild(player_node_);        

    // Create body
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        battle_.World(),
        b2Vec2(player_node_->getPosition().x, player_node_->getPosition().y + player_node_->Size().height / 2),
        player_node_->Size().width,
        player_node_->Size().height);

    body->SetFixedRotation(true);

    // Set the dynamic body fixture.
	b2Fixture* fixture = body->GetFixtureList();	
	fixture[0].SetDensity(1.0f);
	fixture[0].SetFriction(1.0f);
	fixture[0].SetRestitution(0.0f);	
	body->ResetMassData();

    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = 0x0002;
    filter.maskBits = 0x0001;
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    player_->SetBody(body);
    battle_.addComponent(objId, physics);
}

void BattleLayer::SpawnEnemy()
{
    cocos2d::CCSize winSize = cocos2d::CCSize(battle_.Map().Width(), battle_.Map().Height());
    
#if SPAWN_ONE_SIDE
    bool spawnAtRight = false;
#else
    bool spawnAtRight = (GFort::Core::MathHelper::RandomBetween(0.0, 1.0) > 0.5);
#endif

    //cocos2d::CCPoint position;
    //if (spawnAtRight)
    //    position = ccp(winSize.width + enemy->boundingBox().size.width/2, battle_.Map().Landscape());
    //else
    //    position = ccp(0 - enemy->boundingBox().size.width/2, battle_.Map().Landscape());
    
    cocos2d::CCPoint position = (spawnAtRight) ? 
        ccp(winSize.width, battle_.Map().Landscape())               
        : ccp(0, battle_.Map().Landscape());

    SpawnEnemy(position);
}

void BattleLayer::SpawnEnemy(const Point& position)
{
    b2Vec2 pos(position.x, position.y);

    // Entity
    Cistron::ObjectId objId = battle_.SpawnEntity();
    Enemy* unit = new Enemy();
    battle_.addComponent(objId, unit);

    // Render
    EnemyNode* enemy = new EnemyNode(unit);    
    enemy->ResetState();    
    enemy->init();
    enemy->setPosition(position);      

    enemies_nodes_.push_back(enemy);
    battle_.addComponent(objId, enemy);
    this->addChild(enemy);       

    // AI
    Bot* bot;
    //if (GFort::Core::MathHelper::RandomBetween(0.0, 1.0) > 0.5) 
        //bot = new WarriorBot(enemy);
    //else
        bot = new ArcherBot(enemy);
    enemy->SetBot(bot);
    battle_.addComponent(objId, bot);

    battle_.registerName(bot, "Bot");
    std::list<Cistron::Component*> bots = battle_.getComponentsByName("Bot");

    // Create body
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        battle_.World(),
        b2Vec2(enemy->getPosition().x, enemy->getPosition().y + enemy->boundingBox().size.height / 2),
        enemy->boundingBox().size.width,
        enemy->boundingBox().size.height);

    body->SetFixedRotation(true);

    // Set the dynamic body fixture.
	b2Fixture* fixture = body->GetFixtureList();	
	fixture[0].SetDensity(1.0f);
	fixture[0].SetFriction(0.9f);
	fixture[0].SetRestitution(0.0f);	
	body->ResetMassData();

    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = 0x0002;
    filter.maskBits = 0x0001;
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    unit->SetBody(body);
    battle_.addComponent(objId, physics);
}

void BattleLayer::DoSlice()
{
    audio_handler_.PlaySliceEffect();

    //std::vector<Unit> affectedUnits;
    //battle_.DoSlice(slice_layer_->Slice(), affectedUnits);
    //std::set<Unit* > units = BattleHelper::Collide(slice_layer_->Slice(), enemies_nodes_);
	
    std::set<EnemyNode* > damaged_enemies; 
    EnemyNode* enemy;
    std::vector<BTurnInfo> turns;
    for (unsigned int j = 0; j < enemies_nodes_.size(); ++j)
    {
        enemy = static_cast<EnemyNode*>(enemies_nodes_[j]);
        if (enemy->getIsVisible() && enemy->GetState()->Alive())
        {         
            turns.clear();
            if (slice_layer_->Collide(enemy->GetBoundingRegion(), turns))
            {
                battle_.ResolveAttack(*player_, *enemy->GetState());
                if (!enemy->GetState()->Alive())
                {
                    damaged_enemies.insert(enemy);
        
                    if (enemy->GetState()->Body())
                        enemy->GetState()->Body()->SetFixedRotation(false);
                    const float kImpulseStrength        = 2000.0f;
                    
                    b2Vec2 impulse(
                        slice_layer_->Slice().GetEndPosition().x() - turns[0].point.x(), 
                        slice_layer_->Slice().GetEndPosition().y() - turns[0].point.y());
                    impulse.Normalize();
                    impulse *= kImpulseStrength;
                    BattleHelper::ApplyForceOnUnit(enemy->GetState(), turns[0].point, impulse);

                    //std::list<Cistron::Component*> component = battle_.getComponents(enemy->getOwnerId(), "PhysicsComponent");
                    //PhysicsComponent* component = static_cast<PhysicsComponent*>(component.front());                    
                }
            }
        }
    }

    if (damaged_enemies.size() > 0)
    {
        std::set<EnemyNode* >::iterator it;
        for (it = damaged_enemies.begin(); it != damaged_enemies.end(); ++it)
        {
            (*it)->Die();
        }

        //std::for_each(damaged_enemies.begin(), damaged_enemies.end(), boost::mem_fn(&EnemyNode::Die)); 
        battle_.Stat().TotalKills += damaged_enemies.size();
        audio_handler_.PlayKillEffect();
    }

    if (hud_layer_)
    {
        hud_layer_->SetCombo(damaged_enemies.size());
        hud_layer_->SetTotalKills(battle_.Stat().TotalKills);
    }

    //emitter_handler_.AddEmitter(this, player_node_->getPosition());
    //emitter_handler_.AddEmitter(player_node_);

    UpdateCamera(player_node_->getPosition());
}

void BattleLayer::ResolveAttack(UnitNode& attacker, UnitNode& target)
{
    CCLOG("[%s][%d] - Resolving Attack", __FUNCTION__, __LINE__);
    audio_handler_.PlaySliceEffect();
    if (attacker.GetState()->Alive() && target.GetState()->Alive())
    {
        cocos2d::CGFloat diffSQ = cocos2d::ccpLengthSQ(cocos2d::ccpSub(target.getPosition(), attacker.getPosition()));
        if (diffSQ < 900)
        {
            // The target is inside range
            target.GetState()->TakeDamage(attacker.GetState(), 1);
            if (!target.GetState()->Alive())
            {
                CCLOG("[%s][%d] - The target is dead", __FUNCTION__, __LINE__);
                audio_handler_.PlayKillEffect();
            }
            else
            {
                CCLOG("[%s][%d] - The target is hit", __FUNCTION__, __LINE__);
            }                

            if (hud_layer_)
            {
                float percent = (float) target.GetState()->Lives() * 100.0 / (float) target.GetState()->MaxLives(); 
                hud_layer_->SetHealth(percent);
            }
        }
        else
        {
            // The target is out of range
            CCLOG("[%s][%d] - Unit does not hit the target Attacker - (%02f, %02f), Target - (%02f, %02f)", __FUNCTION__, __LINE__, 
                attacker.getPosition().x, 
                attacker.getPosition().y, 
                target.getPosition().x, 
                target.getPosition().y);
        }
    }
}

void BattleLayer::UpdateNode(cocos2d::ccTime dt)
{
    if (!battle_.BattleEnded())
    {
        if (!player_->Alive() ||
            battle_.Stat().TotalKills >= kNumEnemies)
            battle_.Terminate();
    }

    if (battle_.BattleEnded())
    {
        this->unscheduleUpdate();
        this->unscheduleAllSelectors();
        this->setIsTouchEnabled(false);
        cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
        cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(hud_layer_);
        cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(slice_layer_);
        Warrior::SceneHelper manager;
        manager.GoBattleScene(1);
    }
    else
    {
        battle_.Update(dt);

        //std::list<Cistron::Component*> bots = battle_.getComponentsByName("Bot");
        //std::list<Cistron::Component*>::iterator it;
        //for (it = bots.begin(); it != bots.end(); ++it)
        //{
        //    static_cast<Bot*>(*it)->Update(*this);
        //}

        duration_ = duration_ + dt;
           
        {         
            if (duration_ > next_enemy_spawn_ && enemies_nodes_.size() < kNumEnemies) 
            {
                SpawnEnemy();       
                next_enemy_spawn_ = duration_ + GFort::Core::MathHelper::RandomBetween(kMinSpawnTime, kMaxSpawnTime);
            }
        } 

        //{
            Point position = player_node_->getPosition();
            if (position.y > battle_.Map().Landscape())
            {
                player_->SetFloatingInTheAir(true);
                player_node_->setPosition(Point(position.x, position.y + battle_.Map().Gravity() * kFallSpeed));
            }
            else if (position.y < battle_.Map().Landscape())
            {
                player_->SetFloatingInTheAir(false);
                player_node_->setPosition(Point(position.x, static_cast<float>(battle_.Map().Landscape())));
            }

    }
}

void BattleLayer::UpdateCamera(const Point& position)
{
    // INCORRECT! All box2d go to wrong location
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::CCRect boundary(0, 0, 960, 320);
    float minX = 0;
    float maxX = boundary.size.width / 2;

    Point cameraPosition;
    cameraPosition.x = -(position.x - (float)winSize.width * 0.5f);
    cameraPosition.y = 0;
    cameraPosition.x = max(min(cameraPosition.x, minX), -maxX);

    CCLOG("[%s][%d] - Camera move to %0.2f x %02.f", __FUNCTION__, __LINE__, cameraPosition.x, cameraPosition.y);

    //cocos2d::CCMoveTo* moveAction = cocos2d::CCMoveTo::actionWithDuration(kCameraChasingDuration, cameraPosition);
    //this->runAction(moveAction);
    //float cx,cy,cz;
    //this->getCamera()->getCenterXYZ(&cx,&cy,&cz);
    //this->getCamera()->setEyeXYZ(position.x - winSize.width/2, 0, 1.0);
    //this->getCamera()->setCenterXYZ(position.x - winSize.width/2, 0, 0.0);
}


} // namespace
